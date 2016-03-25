﻿#include "../config.h"
#include "application.h"
#include "dbService.h"
#include "userMgr.h"
#include <ProtoUser.h>
int g_closeState = 0;

Application::Application()
{

}

bool Application::init(const std::string & config, ClusterID idx)
{
    if (!ServerConfig::getRef().parse(config, idx))
    {
        LOGE("Application::init error. parse config error. config path=" << config << ", cluster ID = " << idx);
        return false;
    }
    if (idx == InvalidClusterID)
    {
        LOGE("Application::init error. current cluster id invalid. config path=" << config << ", cluster ID = " << idx);
        return false;
    }
    const auto & clusters = ServerConfig::getRef().getClusterConfig();
    auto founder = std::find_if(clusters.begin(), clusters.end(), [](const ClusterConfig& cc){return cc._cluster == ServerConfig::getRef().getClusterID(); });
    if (founder == clusters.end())
    {
        LOGE("Application::init error. current cluster id not found in config file.. config path=" << config << ", cluster ID = " << idx);
        return false;
    }
    LOGA("Application::init  success. clusterID=" << idx);
    SessionManager::getRef().createTimer(1000, std::bind(&Application::checkServiceState, Application::getPtr()));
    return true;
}
void sigInt(int sig)
{
    if (g_closeState == 0)
    {
        g_closeState = 1;
        SessionManager::getRef().post(std::bind(&Application::stop, Application::getPtr()));
    }
}

void Application::onCheckSafeExit()
{
    LOGA("Application::onCheckSafeExit. checking.");
    if (_wlisten != InvalidAccepterID)
    {
        auto &options = SessionManager::getRef().getAccepterOptions(_wlisten);
        if (options._currentLinked != 0)
        {
            SessionManager::getRef().createTimer(1000, std::bind(&Application::onCheckSafeExit, this));
            return;
        }
    }
    if (g_closeState == 1)
    {
        g_closeState = 2;
        for (auto &second : _services)
        {
            if (second.first == ServiceClient || second.first == ServiceInvalid)
            {
                continue;
            }
            for (auto & svc : second.second)
            {
                if (!svc.second->isShell())
                {
                    svc.second->onStop();
                }
            }
        }
    }

    bool safe =  true;
    for(auto &second : _services)
    {
        if (second.first == ServiceClient || second.first == ServiceUser || second.first == ServiceInvalid)
        {
            continue;
        }
        for (auto & svc : second.second)
        {
            if (!svc.second->isShell() && svc.second->isWorked())
            {
                safe = false;
            }
        }
    }
    if(safe)
    {
        LOGA("Application::onNetworkStoped. service all closed.");
        SessionManager::getRef().stopAccept();
        SessionManager::getRef().kickClientSession();
        SessionManager::getRef().kickConnect();
        SessionManager::getRef().stop();
    }
    else
    {
        SessionManager::getRef().createTimer(1000, std::bind(&Application::onCheckSafeExit, this));
    }
}
bool Application::run()
{
    SessionManager::getRef().run();
    LOGA("Application::run exit!");
    return true;
}

bool Application::isStoping()
{
    return g_closeState != 0;
}

void Application::stop()
{
    SessionManager::getRef().stopAccept();
    if (_wlisten != InvalidAccepterID)
    {
        SessionManager::getRef().kickClientSession(_wlisten);
    }
    onCheckSafeExit();
    return ;
}

bool Application::startClusterListen()
{
    const auto & clusters = ServerConfig::getRef().getClusterConfig();
    ServerConfig::getRef().getClusterID();
    auto founder = std::find_if(clusters.begin(), clusters.end(), [](const ClusterConfig& cc){return cc._cluster == ServerConfig::getRef().getClusterID(); });
    if (founder == clusters.end())
    {
        LOGE("Application::startClusterListen error. current cluster id not found in config file." );
        return false;
    }
    const ClusterConfig & cluster = *founder;
    if (cluster._serviceBindIP.empty() || cluster._servicePort == 0)
    {
        LOGE("Application::startClusterListen check config error. bind ip=" << cluster._serviceBindIP << ", bind port=" << cluster._servicePort);
        return false;
    }
    AccepterID aID = SessionManager::getRef().addAccepter(cluster._serviceBindIP, cluster._servicePort);
    if (aID == InvalidAccepterID)
    {
        LOGE("Application::startClusterListen addAccepter error. bind ip=" << cluster._serviceBindIP << ", bind port=" << cluster._servicePort);
        return false;
    }
    auto &options = SessionManager::getRef().getAccepterOptions(aID);
    options._whitelistIP = cluster._whiteList;
    options._maxSessions = 1000;
    options._sessionOptions._sessionPulseInterval = 5000;
    options._sessionOptions._onSessionPulse = [](TcpSessionPtr session)
    {
        ClusterPulse pulse;
        WriteStream ws(pulse.getProtoID());
        ws << pulse;
        session->send(ws.getStream(), ws.getStreamLen());
    };
    options._sessionOptions._onBlockDispatch = std::bind(&Application::event_onServiceMessage, this, _1, _2, _3);
    if (!SessionManager::getRef().openAccepter(aID))
    {
        LOGE("Application::startClusterListen openAccepter error. bind ip=" << cluster._serviceBindIP << ", bind port=" << cluster._servicePort);
        return false;
    }
    LOGA("Application::startClusterListen openAccepter success. bind ip=" << cluster._serviceBindIP << ", bind port=" << cluster._servicePort <<", aID=" << aID);
    return true;
}
bool Application::startClusterConnect()
{
    const auto & clusters = ServerConfig::getRef().getClusterConfig();
    for (const auto & cluster : clusters)
    {
        SessionID cID = SessionManager::getRef().addConnecter(cluster._serviceIP, cluster._servicePort);
        if (cID == InvalidSessionID)
        {
            LOGE("Application::startClusterConnect addConnecter error. remote ip=" << cluster._serviceIP << ", remote port=" << cluster._servicePort);
            return false;
        }
        auto session = SessionManager::getRef().getTcpSession(cID);
        if (!session)
        {
            LOGE("Application::startClusterConnect addConnecter error.  not found connect session. remote ip=" << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
            return false;
        }
        auto &options = session->getOptions();
        options._onSessionLinked = std::bind(&Application::event_onServiceLinked, this, _1);
        options._onSessionClosed = std::bind(&Application::event_onServiceClosed, this, _1);
        options._onBlockDispatch = std::bind(&Application::event_onServiceMessage, this, _1, _2, _3);
        options._reconnects = 50;
        options._connectPulseInterval = 5000;
        options._reconnectClean = false;
        options._onSessionPulse = [](TcpSessionPtr session)
        {
            auto last = session->getUserParamNumber(UPARAM_LAST_ACTIVE_TIME);
            if (last != 0 && getNowTime() - (time_t)last > 30000)
            {
                LOGE("options._onSessionPulse check timeout failed. diff time=" << getNowTime() - (time_t)last);
                session->close();
            }
        };

        if (!SessionManager::getRef().openConnecter(cID))
        {
            LOGE("Application::startClusterConnect openConnecter error. remote ip=" << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
            return false;
        }
        LOGA("Application::startClusterConnect success. remote ip=" << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
        session->setUserParam(UPARAM_SESSION_STATUS, SSTATUS_TRUST);
        session->setUserParam(UPARAM_REMOTE_CLUSTER, cluster._cluster);
        _clusterSession[cluster._cluster] = std::make_pair(cID, 0);
        for (auto st : cluster._services)
        {
            auto & second = _services[st];
            auto & servicePtr = second[InvalidServiceID];
            if (servicePtr)
            {
                LOGE("Application::startClusterConnect add service error. service alread exist. type=" << st << ", remote ip="
                    << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
                return false;
            }
            else
            {
                servicePtr = createLocalService(st);
            }
            if (!servicePtr)
            {
                LOGE("Application::startClusterConnect add service error. unknown service type. type=" << st <<", remote ip="
                    << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
                return false;
            }

            servicePtr->setServiceType(st);
            if (cluster._cluster != ServerConfig::getRef().getClusterID())
            {
                LOGA("Application::startClusterConnect add remote service success. type=" << ServiceNames.at(st) << ", remote ip="
                    << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
                servicePtr->setShell(cluster._cluster);
            }
            else
            {
                LOGA("Application::startClusterConnect add local service success. type=" << ServiceNames.at(st) << ", remote ip="
                    << cluster._serviceIP << ", remote port=" << cluster._servicePort << ", cID=" << cID);
            }
        }
    }
    return true;
}
bool Application::startWideListen()
{
    const auto & clusters = ServerConfig::getRef().getClusterConfig();
    ServerConfig::getRef().getClusterID();
    auto founder = std::find_if(clusters.begin(), clusters.end(), [](const ClusterConfig& cc){return cc._cluster == ServerConfig::getRef().getClusterID(); });
    if (founder == clusters.end())
    {
        LOGE("Application::startWideListen error. current cluster id not found in config file.");
        return false;
    }
    const ClusterConfig & cluster = *founder;
    if (!cluster._wideIP.empty() && cluster._widePort != 0)
    {
        AccepterID aID = SessionManager::getRef().addAccepter("0.0.0.0", cluster._widePort);
        if (aID == InvalidAccepterID)
        {
            LOGE("Application::startWideListen addAccepter error. bind ip=0.0.0.0, show wide ip=" << cluster._wideIP << ", bind port=" << cluster._widePort);
            return false;
        }
        auto &options = SessionManager::getRef().getAccepterOptions(aID);
        options._whitelistIP = cluster._whiteList;
        options._maxSessions = 5000;
        options._sessionOptions._sessionPulseInterval = 10000;
        options._sessionOptions._onSessionPulse = [](TcpSessionPtr session)
        {
            auto last = session->getUserParamNumber(UPARAM_LAST_ACTIVE_TIME);
            if (getNowTime() - (time_t)last > 45000)
            {
                LOGE("options._onSessionPulse check timeout failed. diff time=" << getNowTime() - (time_t)last);
                session->close();
            }
        };
        options._sessionOptions._onSessionLinked = std::bind(&Application::event_onClientLinked, this, _1);
        options._sessionOptions._onSessionClosed = std::bind(&Application::event_onClientClosed, this, _1);
        options._sessionOptions._onBlockDispatch = std::bind(&Application::event_onClientMessage, this, _1, _2, _3);
        if (!SessionManager::getRef().openAccepter(aID))
        {
            LOGE("Application::startWideListen openAccepter error. bind ip=0.0.0.0, show wide ip=" << cluster._wideIP << ", bind port=" << cluster._widePort);
            return false;
        }
        LOGA("Application::startWideListen openAccepter success. bind ip=0.0.0.0, show wide ip=" << cluster._wideIP << ", bind port=" << cluster._widePort << ", listen aID=" << aID);
        _wlisten = aID;
    }
    return true;
}

bool Application::start()
{
    if (startClusterListen() && startClusterConnect() && startWideListen())
    {
        return true;
    }
    return false;
}






void Application::event_onServiceLinked(TcpSessionPtr session)
{
    ClusterID ci = (ClusterID)session->getUserParamNumber(UPARAM_REMOTE_CLUSTER);
    auto founder = _clusterSession.find(ci);
    if (founder == _clusterSession.end())
    {
        LOGE("event_onServiceLinked error cID=" << session->getSessionID() << ", clusterID=" << ci);
        return;
    }
    LOGI("event_onServiceLinked cID=" << session->getSessionID() << ", clusterID=" << ci);
    founder->second.second = 1;
    for (auto & second : _services)
    {
        if (second.first == ServiceClient || second.first == ServiceUser || second.first == ServiceInvalid)
        {
            continue;
        }
        for (auto & svc : second.second )
        {
            if (!svc.second->isShell() && svc.second->isInited())
            {
                ClusterServiceInited inited(svc.second->getServiceType(), svc.second->getServiceID());
                Application::getRef().broadcast(inited);
            }
        }
    }
    checkServiceState();
}

void Application::event_onServiceClosed(TcpSessionPtr session)
{
    ClusterID ci = (ClusterID)session->getUserParamNumber(UPARAM_REMOTE_CLUSTER);
    auto founder = _clusterSession.find(ci);
    if (founder == _clusterSession.end())
    {
        LOGE("event_onServiceClosed error cID=" << session->getSessionID() << ", clusterID=" << ci);
        return;
    }
    LOGW("event_onServiceClosed cID=" << session->getSessionID() << ", clusterID=" << ci);
    founder->second.second = 0;
}


ServicePtr Application::createLocalService(ui16 st)
{
    if (st == ServiceDictDBMgr)
    {
        return std::make_shared<DBService>();
    }
    else if (st == ServiceInfoDBMgr)
    {
        return std::make_shared<DBService>();
    }
    else if (st == ServiceLogDBMgr)
    {
        return std::make_shared<DBService>();
    }
    else if (st == ServiceUserMgr)
    {
        return std::make_shared<UserMgr>();
    }
    else
    {
        LOGE("createLocalService invalid service type " << st);
    }
    return nullptr;
}

void Application::checkServiceState()
{
    SessionManager::getRef().createTimer(1000, std::bind(&Application::checkServiceState, Application::getPtr()));
    if (!_clusterNetWorking)
    {
        for (auto & c : _clusterSession)
        {
            if (c.second.second == 0)
            {
                return;
            }
        }
        _clusterNetWorking = true;
        LOGA("cluster net worked");
    }

    for (auto & second : _services)
    {
        for (auto service : second.second)
        {
            if (service.second && !service.second->isShell() && service.second->isInited() && service.second->getServiceType() != ServiceUser)
            {
                service.second->onTick();
            }
        }
    }
    if (!_clusterServiceInited)
    {
        for (ui16 i = ServiceInvalid+1; i != ServiceMax; i++)
        {
            auto founder = _services.find(i);
            if (founder == _services.end())
            {
                LOGE("not found service id=" << i);
                continue;
            }
            for (auto service : founder->second)
            {
                if (!service.second)
                {
                    LOGE("local service nulptr ...");
                    Application::getRef().stop();
                    return;
                }
                if (service.second->isShell() && (!service.second->isInited() || !service.second->isWorked()))
                {
                    return;
                }
                if (!service.second->isShell() && !service.second->isInited())
                {
                    LOGI("local service [" << ServiceNames.at(service.second->getServiceType()) << "] begin init. [" << service.second->getServiceID() << "] ...");
                    service.second->setInited();
                    bool ret = service.second->onInit();
                    if (ret)
                    {
                        LOGI("local service [" << ServiceNames.at(service.second->getServiceType()) << "] inited. [" << service.second->getServiceID() << "] ...");
                    }
                    else
                    {
                        LOGE("local service [" << ServiceNames.at(service.second->getServiceType()) << "]  init error.[" << service.second->getServiceID() << "] ...");
                        Application::getRef().stop();
                        return;
                    }
                    return;
                }
            }
        }
        LOGA("all local service inited");
        _clusterServiceInited = true;
    }
    

    if (!_clusterServiceWorking)
    {
        for (auto & second : _services)
        {
            for (auto & service : second.second)
            {
                if (!service.second || !service.second->isInited() || !service.second->isWorked())
                {
                    return;
                }
            }
        }
        _clusterServiceWorking = true;
        LOGA("all service worked.");
    }
    
    
}
void Application::event_onRemoteShellForward(TcpSessionPtr session, ReadStream & rs)
{
    Tracing trace;
    rs >> trace;
    auto founder = _services.find(trace._toService);
    if (founder == _services.end())
    {
        LOGE("Application::event_onRemoteShellForward error. unknown service. trace=" << trace);
        return;
    }
    auto fder = founder->second.find(trace._toServiceID);
    if (fder == founder->second.end())
    {
        LOGE("Application::event_onRemoteShellForward error. not found service id. trace=" << trace);
        return;
    }
    Service & svc = *fder->second;
    if (svc.isShell())
    {
        LOGE("Application::event_onRemoteShellForward error. service not local. trace=" << trace);
        return;
    }
    svc.process(trace, rs.getStreamUnread(), rs.getStreamUnreadLen());
}

void Application::event_onRemoteServiceInited(TcpSessionPtr session, ReadStream & rs)
{
    ClusterServiceInited service;
    rs >> service;

    auto founder = _services.find(service.serviceType);
    if (founder == _services.end())
    {
        LOGE("event_onServiceMessage can't founder remote service. service=" << ServiceNames.at(service.serviceType));
        return;
    }
    auto fder = founder->second.find(service.serviceID);
    if (fder == founder->second.end() || !fder->second)
    {
        LOGE("event_onServiceMessage can't founder remote service with id. service=" << ServiceNames.at(service.serviceType) << ", id=" << service.serviceID);
        return;
    }
    if (fder->second->isShell() && !fder->second->isInited())
    {
        LOGI("remote service [" << ServiceNames.at(fder->second->getServiceType()) << "]begin init. [" << fder->second->getServiceID() << "] ...");
        fder->second->setInited();
        fder->second->setWorked(true);
        LOGI("remote service [" << ServiceNames.at(fder->second->getServiceType()) << "] inited. [" << fder->second->getServiceID() << "] ...");
        checkServiceState();
    }
}

void Application::event_onServiceMessage(TcpSessionPtr   session, const char * begin, unsigned int len)
{
    ReadStream rsShell(begin, len);
    if (rsShell.getProtoID() == ClusterPulse::getProtoID())
    {
        session->setUserParam(UPARAM_LAST_ACTIVE_TIME, getNowTime());
        return;
    }
    if (rsShell.getProtoID() == ClusterServiceInited::getProtoID())
    {
        event_onRemoteServiceInited(session, rsShell);
        return;
    }
    if (rsShell.getProtoID() == ClusterShellForward::getProtoID())
    {
        event_onRemoteShellForward(session, rsShell);
        return;
    }
    if (rsShell.getProtoID() == ClusterClientForward::getProtoID())
    {
        Tracing trace;
        rsShell >> trace;
        ReadStream rs(rsShell.getStreamUnread(), rsShell.getStreamUnreadLen());
        if (rs.getProtoID() == UserAuthResp::getProtoID())
        {
            UserAuthResp resp;
            rs >> resp;
            auto client = SessionManager::getRef().getTcpSession(resp.clientSessionID);
            if (client)
            {
                if (resp.retCode == EC_SUCCESS)
                {
                    client->setUserParam(UPARAM_SESSION_STATUS, SSTATUS_AUTHED);
                }
                else
                {
                    client->setUserParam(UPARAM_SESSION_STATUS, SSTATUS_UNKNOW);
                }
                ClientAuthResp clientresp;
                clientresp.retCode = resp.retCode;
                clientresp.account = resp.account;
                clientresp.token = resp.token;
                clientresp.previews = resp.previews;
                WriteStream ws(ClientAuthResp::getProtoID());
                ws << clientresp;
                client->send(ws.getStream(), ws.getStreamLen());
            }
            return;
        }
        else
        {
            auto founder = _services.find(ServiceUser);
            if (founder == _services.end())
            {
                LOGW("not have any client.");
                return;
            }
            auto fder = founder->second.find(trace._toServiceID);
            if (fder == founder->second.end())
            {
                LOGW("not have the client.");
                return;
            }
            fder->second->process(trace, rsShell.getStreamUnread(), rsShell.getStreamUnreadLen());
        }
    }
    
}




void Application::event_onClientPulse(TcpSessionPtr session)
{
    if (isSessionID(session->getSessionID()))
    {
        if (time(NULL) - session->getUserParamNumber(UPARAM_LAST_ACTIVE_TIME) > session->getOptions()._sessionPulseInterval / 1000 * 2)
        {
            session->close();
            return;
        }
    }
}






void Application::event_onClientLinked(TcpSessionPtr session)
{
    session->setUserParam(UPARAM_SESSION_STATUS, SSTATUS_UNKNOW);
    LOGD("Application::event_onClientLinked. SessionID=" << session->getSessionID() 
        << ", remoteIP=" << session->getRemoteIP() << ", remotePort=" << session->getRemotePort());
}

void Application::event_onClientClosed(TcpSessionPtr session)
{
    LOGD("Application::event_onClientClosed. SessionID=" << session->getSessionID() 
        << ", remoteIP=" << session->getRemoteIP() << ", remotePort=" << session->getRemotePort());
    if (isConnectID(session->getSessionID()))
    {
    }
    else
    {
        if (session->getUserParamNumber(UPARAM_SESSION_STATUS) == SSTATUS_LOGINED)
        {

        }

    }
}



void Application::event_onClientMessage(TcpSessionPtr session, const char * begin, unsigned int len)
{
    ReadStream rs(begin, len);
    SessionStatus ss = (SessionStatus) session->getUserParamNumber(UPARAM_SESSION_STATUS);
    if (rs.getProtoID() == ClientAuthReq::getProtoID())
    {
        LOGD("ClientAuthReq sID=" << session->getSessionID() << ", block len=" << len);
        if (ss != SSTATUS_UNKNOW)
        {
            LOGE("duplicate ClientAuthReq. sID=" << session->getSessionID());
            return;
        }
        ClientAuthReq careq;
        rs >> careq;
        Tracing trace;
        trace._fromService = ServiceClient;
        trace._fromServiceID = session->getSessionID();
        trace._toService = ServiceUserMgr;
        trace._toServiceID = InvalidServiceID;
        WriteStream ws(UserAuthReq::getProtoID());
        UserAuthReq req;
        req.account = careq.account;
        req.token = careq.token;
        req.clientSessionID = session->getSessionID();
        req.clientClusterID = ServerConfig::getRef().getClusterID();
        ws << req;
        callOtherService(trace, ws.getStream(), ws.getStreamLen());
        session->setUserParam(UPARAM_SESSION_STATUS, SSTATUS_AUTHING);
        return;
    }

}

void Application::callOtherCluster(ClusterID cltID, const char * block, unsigned int len)
{
    auto founder = _clusterSession.find(cltID);
    if (founder == _clusterSession.end())
    {
        LOGF("Application::callOtherCluster fatal error. cltID not found. cltID=" << cltID);
        return;
    }
    if (founder->second.first == InvalidServiceID)
    {
        LOGF("Application::callOtherCluster fatal error. cltID not have session. cltID=" << cltID);
        return;
    }
    if (founder->second.second == 0)
    {
        LOGW("Application::callOtherCluster warning error. session try connecting. cltID=" << cltID << ", client session ID=" << founder->second.first);
    }
    SessionManager::getRef().sendSessionData(founder->second.first, block, len);
}

void Application::callOtherService(Tracing trace, const char * block, unsigned int len)
{
    if (trace._fromService >= ServiceMax || trace._toService >= ServiceMax || trace._toService == ServiceInvalid)
    {
        LOGF("Application::callOtherService Illegality trace. trace=" << trace << ", block len=" << len);
        return;
    }
    if (trace._fromService == ServiceUserMgr
        && (trace._toService == ServiceUser || trace._toService == ServiceClient))
    {
        LOGF("call method to ServiceUser or ServiceClient from ServiceUserMgr is Illegality! trace=" << trace);
        return;
    }

    ui16 toService = trace._toService;
    if (trace._toService == ServiceUser && trace._toService == ServiceClient)
    {
        toService = ServiceUserMgr;
    }

    auto founder = _services.find(toService);
    if (founder == _services.end())
    {
        LOGF("Application::callOtherService can not found _toService type  trace =" << trace << ", block len=" << len);
        return;
    }
    auto fder = founder->second.find(trace._toServiceID);
    if (fder == founder->second.end())
    {
        LOGD("Application::callOtherService can not found _toService ID. trace =" << trace << ", block len=" << len);
        return;
    }
    auto & service = *fder->second;
    if (service.isShell()) //forward 
    {
        WriteStream ws(ClusterShellForward::getProtoID());
        ws << trace;
        ws.appendOriginalData(block, len);
        ClusterID cltID = service.getClusterID();
        auto fsder = _clusterSession.find(cltID);
        if (fsder == _clusterSession.end())
        {
            LOGE("Application::callOtherService not found session by shell service. clusterID=" << cltID << ", tracing=" << trace);
        }
        else
        {
            SessionManager::getRef().sendSessionData(fsder->second.first, ws.getStream(), ws.getStreamLen());
            if (fsder->second.second == 0)
            {
                LOGW("Application::callOtherService session not connected when global call by shell service. sID=" << fsder->second.first
                    << ", clusterID=" << cltID << ", tracing=" << trace);
            }
        }

    }
    else //direct process
    {
        std::string bk;
        bk.assign(block, len);
        SessionManager::getRef().post(std::bind(&Service::process2, fder->second, trace, std::move(bk)));
    }

}







