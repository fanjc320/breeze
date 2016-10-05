 
#ifndef _PROTOCLIENT_H_ 
#define _PROTOCLIENT_H_ 
 
 
struct ClientPulse //客户端脉冲请求  
{ 
    static const unsigned short getProtoID() { return 40034;} 
    static const std::string getProtoName() { return "ClientPulse";} 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ClientPulse & data) 
{ 
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ClientPulse & data) 
{ 
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ClientPulse & info) 
{ 
    stm << "[\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ClientAuthReq //认证请求  
{ 
    static const unsigned short getProtoID() { return 40035;} 
    static const std::string getProtoName() { return "ClientAuthReq";} 
    std::string account;  
    std::string token;  
    ClientAuthReq() 
    { 
    } 
    ClientAuthReq(const std::string & account, const std::string & token) 
    { 
        this->account = account; 
        this->token = token; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ClientAuthReq & data) 
{ 
    ws << data.account;  
    ws << data.token;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ClientAuthReq & data) 
{ 
    rs >> data.account;  
    rs >> data.token;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ClientAuthReq & info) 
{ 
    stm << "[\n"; 
    stm << "account=" << info.account << "\n"; 
    stm << "token=" << info.token << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ClientAuthResp //认证返回  
{ 
    static const unsigned short getProtoID() { return 40036;} 
    static const std::string getProtoName() { return "ClientAuthResp";} 
    unsigned short retCode;  
    std::string account;  
    std::string token;  
    AvatarPreviewArray previews;  
    ClientAuthResp() 
    { 
        retCode = 0; 
    } 
    ClientAuthResp(const unsigned short & retCode, const std::string & account, const std::string & token, const AvatarPreviewArray & previews) 
    { 
        this->retCode = retCode; 
        this->account = account; 
        this->token = token; 
        this->previews = previews; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ClientAuthResp & data) 
{ 
    ws << data.retCode;  
    ws << data.account;  
    ws << data.token;  
    ws << data.previews;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ClientAuthResp & data) 
{ 
    rs >> data.retCode;  
    rs >> data.account;  
    rs >> data.token;  
    rs >> data.previews;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ClientAuthResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "account=" << info.account << "\n"; 
    stm << "token=" << info.token << "\n"; 
    stm << "previews=" << info.previews << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct CreateAvatarReq //创角色请求  
{ 
    static const unsigned short getProtoID() { return 40037;} 
    static const std::string getProtoName() { return "CreateAvatarReq";} 
    std::string accountName; //这个字段会被服务器填充.客户端可以不填写  
    std::string userName;  
    CreateAvatarReq() 
    { 
    } 
    CreateAvatarReq(const std::string & accountName, const std::string & userName) 
    { 
        this->accountName = accountName; 
        this->userName = userName; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const CreateAvatarReq & data) 
{ 
    ws << data.accountName;  
    ws << data.userName;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, CreateAvatarReq & data) 
{ 
    rs >> data.accountName;  
    rs >> data.userName;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const CreateAvatarReq & info) 
{ 
    stm << "[\n"; 
    stm << "accountName=" << info.accountName << "\n"; 
    stm << "userName=" << info.userName << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct CreateAvatarResp //创角色请求返回  
{ 
    static const unsigned short getProtoID() { return 40038;} 
    static const std::string getProtoName() { return "CreateAvatarResp";} 
    unsigned short retCode;  
    unsigned long long avatarID;  
    AvatarPreviewArray previews;  
    CreateAvatarResp() 
    { 
        retCode = 0; 
        avatarID = 0; 
    } 
    CreateAvatarResp(const unsigned short & retCode, const unsigned long long & avatarID, const AvatarPreviewArray & previews) 
    { 
        this->retCode = retCode; 
        this->avatarID = avatarID; 
        this->previews = previews; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const CreateAvatarResp & data) 
{ 
    ws << data.retCode;  
    ws << data.avatarID;  
    ws << data.previews;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, CreateAvatarResp & data) 
{ 
    rs >> data.retCode;  
    rs >> data.avatarID;  
    rs >> data.previews;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const CreateAvatarResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "avatarID=" << info.avatarID << "\n"; 
    stm << "previews=" << info.previews << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct AttachAvatarReq //选角色请求  
{ 
    static const unsigned short getProtoID() { return 40039;} 
    static const std::string getProtoName() { return "AttachAvatarReq";} 
    std::string accountName; //这个字段会被服务器填充.客户端可以不填写  
    unsigned long long avatarID;  
    AttachAvatarReq() 
    { 
        avatarID = 0; 
    } 
    AttachAvatarReq(const std::string & accountName, const unsigned long long & avatarID) 
    { 
        this->accountName = accountName; 
        this->avatarID = avatarID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const AttachAvatarReq & data) 
{ 
    ws << data.accountName;  
    ws << data.avatarID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, AttachAvatarReq & data) 
{ 
    rs >> data.accountName;  
    rs >> data.avatarID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const AttachAvatarReq & info) 
{ 
    stm << "[\n"; 
    stm << "accountName=" << info.accountName << "\n"; 
    stm << "avatarID=" << info.avatarID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct AttachAvatarResp //选角色请求返回  
{ 
    static const unsigned short getProtoID() { return 40040;} 
    static const std::string getProtoName() { return "AttachAvatarResp";} 
    unsigned short retCode;  
    AvatarBaseInfo baseInfo;  
    AvatarPropMap props;  
    AttachAvatarResp() 
    { 
        retCode = 0; 
    } 
    AttachAvatarResp(const unsigned short & retCode, const AvatarBaseInfo & baseInfo, const AvatarPropMap & props) 
    { 
        this->retCode = retCode; 
        this->baseInfo = baseInfo; 
        this->props = props; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const AttachAvatarResp & data) 
{ 
    ws << data.retCode;  
    ws << data.baseInfo;  
    ws << data.props;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, AttachAvatarResp & data) 
{ 
    rs >> data.retCode;  
    rs >> data.baseInfo;  
    rs >> data.props;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const AttachAvatarResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "baseInfo=" << info.baseInfo << "\n"; 
    stm << "props=" << info.props << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct AvatarBaseInfoNotice 
{ 
    static const unsigned short getProtoID() { return 40041;} 
    static const std::string getProtoName() { return "AvatarBaseInfoNotice";} 
    AvatarBaseInfo baseInfo;  
    AvatarBaseInfoNotice() 
    { 
    } 
    AvatarBaseInfoNotice(const AvatarBaseInfo & baseInfo) 
    { 
        this->baseInfo = baseInfo; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const AvatarBaseInfoNotice & data) 
{ 
    ws << data.baseInfo;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, AvatarBaseInfoNotice & data) 
{ 
    rs >> data.baseInfo;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const AvatarBaseInfoNotice & info) 
{ 
    stm << "[\n"; 
    stm << "baseInfo=" << info.baseInfo << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct AvatarPropMapNotice 
{ 
    static const unsigned short getProtoID() { return 40042;} 
    static const std::string getProtoName() { return "AvatarPropMapNotice";} 
    unsigned long long avatarID;  
    AvatarPropMap props;  
    AvatarPropMapNotice() 
    { 
        avatarID = 0; 
    } 
    AvatarPropMapNotice(const unsigned long long & avatarID, const AvatarPropMap & props) 
    { 
        this->avatarID = avatarID; 
        this->props = props; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const AvatarPropMapNotice & data) 
{ 
    ws << data.avatarID;  
    ws << data.props;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, AvatarPropMapNotice & data) 
{ 
    rs >> data.avatarID;  
    rs >> data.props;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const AvatarPropMapNotice & info) 
{ 
    stm << "[\n"; 
    stm << "avatarID=" << info.avatarID << "\n"; 
    stm << "props=" << info.props << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupGetStatusReq //获取当前角色的场景状态数据  
{ 
    static const unsigned short getProtoID() { return 40043;} 
    static const std::string getProtoName() { return "SceneGroupGetStatusReq";} 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupGetStatusReq & data) 
{ 
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupGetStatusReq & data) 
{ 
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupGetStatusReq & info) 
{ 
    stm << "[\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupGetStatusResp 
{ 
    static const unsigned short getProtoID() { return 40044;} 
    static const std::string getProtoName() { return "SceneGroupGetStatusResp";} 
    unsigned short retCode;  
    SceneGroupGetStatusResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupGetStatusResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupGetStatusResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupGetStatusResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupGetStatusResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupAvatarInfo 
{ 
    static const unsigned short getProtoID() { return 40045;} 
    static const std::string getProtoName() { return "SceneGroupAvatarInfo";} 
    AvatarBaseInfo baseInfo;  
    unsigned short powerType; //0普通,1leader,2master  
    SceneGroupAvatarInfo() 
    { 
        powerType = 0; 
    } 
    SceneGroupAvatarInfo(const AvatarBaseInfo & baseInfo, const unsigned short & powerType) 
    { 
        this->baseInfo = baseInfo; 
        this->powerType = powerType; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupAvatarInfo & data) 
{ 
    ws << data.baseInfo;  
    ws << data.powerType;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupAvatarInfo & data) 
{ 
    rs >> data.baseInfo;  
    rs >> data.powerType;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupAvatarInfo & info) 
{ 
    stm << "[\n"; 
    stm << "baseInfo=" << info.baseInfo << "\n"; 
    stm << "powerType=" << info.powerType << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
 
typedef std::vector<SceneGroupAvatarInfo> SceneGroupAvatarInfoArray;  
 
struct SceneGroupInfoNotice //编队数据通知  
{ 
    static const unsigned short getProtoID() { return 40046;} 
    static const std::string getProtoName() { return "SceneGroupInfoNotice";} 
    unsigned short sceneType; //场景类型  
    unsigned long long mapID;  
    unsigned long long groupID;  
    unsigned short sceneStatus; //状态  
    unsigned long long lineID; //分线ID  
    unsigned long long sceneID; //场景实例ID  
    std::string host; //服务器host  
    unsigned short port; //服务器port  
    std::string token; //自己的令牌  
    SceneGroupAvatarInfoArray members; //队友数据  
    SceneGroupInfoNotice() 
    { 
        sceneType = 0; 
        mapID = 0; 
        groupID = 0; 
        sceneStatus = 0; 
        lineID = 0; 
        sceneID = 0; 
        port = 0; 
    } 
    SceneGroupInfoNotice(const unsigned short & sceneType, const unsigned long long & mapID, const unsigned long long & groupID, const unsigned short & sceneStatus, const unsigned long long & lineID, const unsigned long long & sceneID, const std::string & host, const unsigned short & port, const std::string & token, const SceneGroupAvatarInfoArray & members) 
    { 
        this->sceneType = sceneType; 
        this->mapID = mapID; 
        this->groupID = groupID; 
        this->sceneStatus = sceneStatus; 
        this->lineID = lineID; 
        this->sceneID = sceneID; 
        this->host = host; 
        this->port = port; 
        this->token = token; 
        this->members = members; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupInfoNotice & data) 
{ 
    ws << data.sceneType;  
    ws << data.mapID;  
    ws << data.groupID;  
    ws << data.sceneStatus;  
    ws << data.lineID;  
    ws << data.sceneID;  
    ws << data.host;  
    ws << data.port;  
    ws << data.token;  
    ws << data.members;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupInfoNotice & data) 
{ 
    rs >> data.sceneType;  
    rs >> data.mapID;  
    rs >> data.groupID;  
    rs >> data.sceneStatus;  
    rs >> data.lineID;  
    rs >> data.sceneID;  
    rs >> data.host;  
    rs >> data.port;  
    rs >> data.token;  
    rs >> data.members;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupInfoNotice & info) 
{ 
    stm << "[\n"; 
    stm << "sceneType=" << info.sceneType << "\n"; 
    stm << "mapID=" << info.mapID << "\n"; 
    stm << "groupID=" << info.groupID << "\n"; 
    stm << "sceneStatus=" << info.sceneStatus << "\n"; 
    stm << "lineID=" << info.lineID << "\n"; 
    stm << "sceneID=" << info.sceneID << "\n"; 
    stm << "host=" << info.host << "\n"; 
    stm << "port=" << info.port << "\n"; 
    stm << "token=" << info.token << "\n"; 
    stm << "members=" << info.members << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupCreateReq //发起组队请求  
{ 
    static const unsigned short getProtoID() { return 40047;} 
    static const std::string getProtoName() { return "SceneGroupCreateReq";} 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupCreateReq & data) 
{ 
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupCreateReq & data) 
{ 
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupCreateReq & info) 
{ 
    stm << "[\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupCreateResp 
{ 
    static const unsigned short getProtoID() { return 40048;} 
    static const std::string getProtoName() { return "SceneGroupCreateResp";} 
    unsigned short retCode;  
    SceneGroupCreateResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupCreateResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupCreateResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupCreateResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupCreateResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupJoinReq //加入组队  
{ 
    static const unsigned short getProtoID() { return 40049;} 
    static const std::string getProtoName() { return "SceneGroupJoinReq";} 
    unsigned long long groupID;  
    SceneGroupJoinReq() 
    { 
        groupID = 0; 
    } 
    SceneGroupJoinReq(const unsigned long long & groupID) 
    { 
        this->groupID = groupID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupJoinReq & data) 
{ 
    ws << data.groupID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupJoinReq & data) 
{ 
    rs >> data.groupID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupJoinReq & info) 
{ 
    stm << "[\n"; 
    stm << "groupID=" << info.groupID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupJoinResp 
{ 
    static const unsigned short getProtoID() { return 40050;} 
    static const std::string getProtoName() { return "SceneGroupJoinResp";} 
    unsigned short retCode;  
    SceneGroupJoinResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupJoinResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupJoinResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupJoinResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupJoinResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupInviteReq //邀请对方加入自己的编队  
{ 
    static const unsigned short getProtoID() { return 40051;} 
    static const std::string getProtoName() { return "SceneGroupInviteReq";} 
    unsigned long long avatarID;  
    SceneGroupInviteReq() 
    { 
        avatarID = 0; 
    } 
    SceneGroupInviteReq(const unsigned long long & avatarID) 
    { 
        this->avatarID = avatarID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupInviteReq & data) 
{ 
    ws << data.avatarID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupInviteReq & data) 
{ 
    rs >> data.avatarID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupInviteReq & info) 
{ 
    stm << "[\n"; 
    stm << "avatarID=" << info.avatarID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupInviteResp 
{ 
    static const unsigned short getProtoID() { return 40052;} 
    static const std::string getProtoName() { return "SceneGroupInviteResp";} 
    unsigned short retCode;  
    SceneGroupInviteResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupInviteResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupInviteResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupInviteResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupInviteResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupRejectReq //拒绝对方邀请  
{ 
    static const unsigned short getProtoID() { return 40053;} 
    static const std::string getProtoName() { return "SceneGroupRejectReq";} 
    unsigned long long groupID;  
    SceneGroupRejectReq() 
    { 
        groupID = 0; 
    } 
    SceneGroupRejectReq(const unsigned long long & groupID) 
    { 
        this->groupID = groupID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupRejectReq & data) 
{ 
    ws << data.groupID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupRejectReq & data) 
{ 
    rs >> data.groupID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupRejectReq & info) 
{ 
    stm << "[\n"; 
    stm << "groupID=" << info.groupID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupRejectResp 
{ 
    static const unsigned short getProtoID() { return 40054;} 
    static const std::string getProtoName() { return "SceneGroupRejectResp";} 
    unsigned short retCode;  
    SceneGroupRejectResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupRejectResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupRejectResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupRejectResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupRejectResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupLeaveReq //离开编队  
{ 
    static const unsigned short getProtoID() { return 40055;} 
    static const std::string getProtoName() { return "SceneGroupLeaveReq";} 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupLeaveReq & data) 
{ 
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupLeaveReq & data) 
{ 
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupLeaveReq & info) 
{ 
    stm << "[\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupLeaveResp 
{ 
    static const unsigned short getProtoID() { return 40056;} 
    static const std::string getProtoName() { return "SceneGroupLeaveResp";} 
    unsigned short retCode;  
    SceneGroupLeaveResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupLeaveResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupLeaveResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupLeaveResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupLeaveResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupEnterSceneReq //申请进入场景  
{ 
    static const unsigned short getProtoID() { return 40057;} 
    static const std::string getProtoName() { return "SceneGroupEnterSceneReq";} 
    unsigned short sceneType;  
    unsigned long long mapID;  
    SceneGroupEnterSceneReq() 
    { 
        sceneType = 0; 
        mapID = 0; 
    } 
    SceneGroupEnterSceneReq(const unsigned short & sceneType, const unsigned long long & mapID) 
    { 
        this->sceneType = sceneType; 
        this->mapID = mapID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupEnterSceneReq & data) 
{ 
    ws << data.sceneType;  
    ws << data.mapID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupEnterSceneReq & data) 
{ 
    rs >> data.sceneType;  
    rs >> data.mapID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupEnterSceneReq & info) 
{ 
    stm << "[\n"; 
    stm << "sceneType=" << info.sceneType << "\n"; 
    stm << "mapID=" << info.mapID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupEnterSceneResp 
{ 
    static const unsigned short getProtoID() { return 40058;} 
    static const std::string getProtoName() { return "SceneGroupEnterSceneResp";} 
    unsigned short retCode;  
    SceneGroupEnterSceneResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupEnterSceneResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupEnterSceneResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupEnterSceneResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupEnterSceneResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupCancelEnterReq //取消申请(根据状态不同会有取消匹配,重置状态,结束战斗(可中途结束的场景类型)  
{ 
    static const unsigned short getProtoID() { return 40059;} 
    static const std::string getProtoName() { return "SceneGroupCancelEnterReq";} 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupCancelEnterReq & data) 
{ 
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupCancelEnterReq & data) 
{ 
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupCancelEnterReq & info) 
{ 
    stm << "[\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct SceneGroupCancelEnterResp 
{ 
    static const unsigned short getProtoID() { return 40060;} 
    static const std::string getProtoName() { return "SceneGroupCancelEnterResp";} 
    unsigned short retCode;  
    SceneGroupCancelEnterResp() 
    { 
        retCode = 0; 
    } 
    SceneGroupCancelEnterResp(const unsigned short & retCode) 
    { 
        this->retCode = retCode; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const SceneGroupCancelEnterResp & data) 
{ 
    ws << data.retCode;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, SceneGroupCancelEnterResp & data) 
{ 
    rs >> data.retCode;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const SceneGroupCancelEnterResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ChatReq 
{ 
    static const unsigned short getProtoID() { return 40061;} 
    static const std::string getProtoName() { return "ChatReq";} 
    unsigned short channelID;  
    unsigned long long targetID;  
    std::string msg;  
    ChatReq() 
    { 
        channelID = 0; 
        targetID = 0; 
    } 
    ChatReq(const unsigned short & channelID, const unsigned long long & targetID, const std::string & msg) 
    { 
        this->channelID = channelID; 
        this->targetID = targetID; 
        this->msg = msg; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ChatReq & data) 
{ 
    ws << data.channelID;  
    ws << data.targetID;  
    ws << data.msg;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ChatReq & data) 
{ 
    rs >> data.channelID;  
    rs >> data.targetID;  
    rs >> data.msg;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ChatReq & info) 
{ 
    stm << "[\n"; 
    stm << "channelID=" << info.channelID << "\n"; 
    stm << "targetID=" << info.targetID << "\n"; 
    stm << "msg=" << info.msg << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ChatResp 
{ 
    static const unsigned short getProtoID() { return 40062;} 
    static const std::string getProtoName() { return "ChatResp";} 
    unsigned short channelID;  
    unsigned long long sourceID;  
    std::string sourceName;  
    unsigned long long targetID;  
    std::string targetName;  
    std::string msg;  
    unsigned long long chatTime;  
    ChatResp() 
    { 
        channelID = 0; 
        sourceID = 0; 
        targetID = 0; 
        chatTime = 0; 
    } 
    ChatResp(const unsigned short & channelID, const unsigned long long & sourceID, const std::string & sourceName, const unsigned long long & targetID, const std::string & targetName, const std::string & msg, const unsigned long long & chatTime) 
    { 
        this->channelID = channelID; 
        this->sourceID = sourceID; 
        this->sourceName = sourceName; 
        this->targetID = targetID; 
        this->targetName = targetName; 
        this->msg = msg; 
        this->chatTime = chatTime; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ChatResp & data) 
{ 
    ws << data.channelID;  
    ws << data.sourceID;  
    ws << data.sourceName;  
    ws << data.targetID;  
    ws << data.targetName;  
    ws << data.msg;  
    ws << data.chatTime;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ChatResp & data) 
{ 
    rs >> data.channelID;  
    rs >> data.sourceID;  
    rs >> data.sourceName;  
    rs >> data.targetID;  
    rs >> data.targetName;  
    rs >> data.msg;  
    rs >> data.chatTime;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ChatResp & info) 
{ 
    stm << "[\n"; 
    stm << "channelID=" << info.channelID << "\n"; 
    stm << "sourceID=" << info.sourceID << "\n"; 
    stm << "sourceName=" << info.sourceName << "\n"; 
    stm << "targetID=" << info.targetID << "\n"; 
    stm << "targetName=" << info.targetName << "\n"; 
    stm << "msg=" << info.msg << "\n"; 
    stm << "chatTime=" << info.chatTime << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct PingPongReq 
{ 
    static const unsigned short getProtoID() { return 40063;} 
    static const std::string getProtoName() { return "PingPongReq";} 
    std::string msg;  
    PingPongReq() 
    { 
    } 
    PingPongReq(const std::string & msg) 
    { 
        this->msg = msg; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const PingPongReq & data) 
{ 
    ws << data.msg;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, PingPongReq & data) 
{ 
    rs >> data.msg;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const PingPongReq & info) 
{ 
    stm << "[\n"; 
    stm << "msg=" << info.msg << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct PingPongResp 
{ 
    static const unsigned short getProtoID() { return 40064;} 
    static const std::string getProtoName() { return "PingPongResp";} 
    std::string msg;  
    PingPongResp() 
    { 
    } 
    PingPongResp(const std::string & msg) 
    { 
        this->msg = msg; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const PingPongResp & data) 
{ 
    ws << data.msg;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, PingPongResp & data) 
{ 
    rs >> data.msg;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const PingPongResp & info) 
{ 
    stm << "[\n"; 
    stm << "msg=" << info.msg << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ChangeIconIDReq 
{ 
    static const unsigned short getProtoID() { return 40065;} 
    static const std::string getProtoName() { return "ChangeIconIDReq";} 
    int iconID;  
    ChangeIconIDReq() 
    { 
        iconID = 0; 
    } 
    ChangeIconIDReq(const int & iconID) 
    { 
        this->iconID = iconID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ChangeIconIDReq & data) 
{ 
    ws << data.iconID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ChangeIconIDReq & data) 
{ 
    rs >> data.iconID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ChangeIconIDReq & info) 
{ 
    stm << "[\n"; 
    stm << "iconID=" << info.iconID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ChangeIconIDResp 
{ 
    static const unsigned short getProtoID() { return 40066;} 
    static const std::string getProtoName() { return "ChangeIconIDResp";} 
    unsigned short retCode;  
    int iconID;  
    ChangeIconIDResp() 
    { 
        retCode = 0; 
        iconID = 0; 
    } 
    ChangeIconIDResp(const unsigned short & retCode, const int & iconID) 
    { 
        this->retCode = retCode; 
        this->iconID = iconID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ChangeIconIDResp & data) 
{ 
    ws << data.retCode;  
    ws << data.iconID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ChangeIconIDResp & data) 
{ 
    rs >> data.retCode;  
    rs >> data.iconID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ChangeIconIDResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "iconID=" << info.iconID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ChangeModeIDReq 
{ 
    static const unsigned short getProtoID() { return 40067;} 
    static const std::string getProtoName() { return "ChangeModeIDReq";} 
    int modeID;  
    ChangeModeIDReq() 
    { 
        modeID = 0; 
    } 
    ChangeModeIDReq(const int & modeID) 
    { 
        this->modeID = modeID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ChangeModeIDReq & data) 
{ 
    ws << data.modeID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ChangeModeIDReq & data) 
{ 
    rs >> data.modeID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ChangeModeIDReq & info) 
{ 
    stm << "[\n"; 
    stm << "modeID=" << info.modeID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
struct ChangeModeIDResp 
{ 
    static const unsigned short getProtoID() { return 40068;} 
    static const std::string getProtoName() { return "ChangeModeIDResp";} 
    unsigned short retCode;  
    int modeID;  
    ChangeModeIDResp() 
    { 
        retCode = 0; 
        modeID = 0; 
    } 
    ChangeModeIDResp(const unsigned short & retCode, const int & modeID) 
    { 
        this->retCode = retCode; 
        this->modeID = modeID; 
    } 
}; 
inline zsummer::proto4z::WriteStream & operator << (zsummer::proto4z::WriteStream & ws, const ChangeModeIDResp & data) 
{ 
    ws << data.retCode;  
    ws << data.modeID;  
    return ws; 
} 
inline zsummer::proto4z::ReadStream & operator >> (zsummer::proto4z::ReadStream & rs, ChangeModeIDResp & data) 
{ 
    rs >> data.retCode;  
    rs >> data.modeID;  
    return rs; 
} 
inline zsummer::log4z::Log4zStream & operator << (zsummer::log4z::Log4zStream & stm, const ChangeModeIDResp & info) 
{ 
    stm << "[\n"; 
    stm << "retCode=" << info.retCode << "\n"; 
    stm << "modeID=" << info.modeID << "\n"; 
    stm << "]\n"; 
    return stm; 
} 
 
#endif 
