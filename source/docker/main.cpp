﻿
/*
* breeze License
* Copyright (C) 2014 YaweiZhang <yawei.zhang@foxmail.com>.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "logic/docker.h"
using namespace zsummer::log4z;




void sigInt(int sig);


int main(int argc, char* argv[])
{

#ifndef _WIN32
//! linux下需要屏蔽的一些信号
    signal( SIGHUP, SIG_IGN );
    signal( SIGALRM, SIG_IGN );
    signal( SIGPIPE, SIG_IGN );
    signal( SIGXCPU, SIG_IGN );
    signal( SIGXFSZ, SIG_IGN );
    signal( SIGPROF, SIG_IGN ); 
    signal( SIGVTALRM, SIG_IGN );
    signal( SIGQUIT, SIG_IGN );
    signal( SIGCHLD, SIG_IGN);
    //setenv("TZ", "GMT-8", 1);
#else
    //system("chcp 65001"); //需要配合设置显示用的字体为TrueType字体 
#endif
    signal(SIGINT, &sigInt);
    signal(SIGTERM, &sigInt);


    std::string filename = "../config.lua";
    DockerID index = 0;
    if (argc > 1)
    {
        index = atoi(argv[1]);
    }
    if (argc > 2)
    {
        filename = argv[2];
    }
    bool ret = false;
    ILog4zManager::getPtr()->config("../log.cfg");
    ILog4zManager::getPtr()->setLoggerName(LOG4Z_MAIN_LOGGER_ID, (std::string("Main") + toString(index)).c_str());
    ILog4zManager::getPtr()->setLoggerName(ILog4zManager::getRef().findLogger("NetWork"), (std::string("NetWork") + toString(index)).c_str());
    ret = ILog4zManager::getPtr()->start();
    if (!ret)
    {
        std::cout << "ILog4zManager start false." << std::endl;
        return ret;
    }
    LOGA("version released by " << __DATE__ << " " << __TIME__);
    LOGA("dockerID=" << index << ", config file=" << filename);
    LOGD("filecontent=" << readFileContent(filename));
    LOGA("begin start docker ...");
    try
    {
        if (!SessionManager::getRef().start())
        {
            LOGE("SessionManager start false.");
            return 1;
        }
        
        if (!Docker::getRef().init(filename, index))
        {
            LOGE("Docker init false.");
            return 2;
        }
        if (!Docker::getRef().start())
        {
            LOGE("Docker start false.");
            return 3;
        }
        Docker::getRef().run();
    }
    catch (const std::exception & e)
    {
        LOGE("Docker::init have runtime error. what=" << e.what());
        return -1;
    }

    

    LOGI("Docker exit.");

    return 0;
}

