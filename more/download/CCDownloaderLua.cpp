/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "cocos2d.h"
#include "CCDownloaderLua.h"

int CCDownloaderLua::s_numb = 1;

static std::map<int,LUA_FUNCTION> s_funmap;

int CCDownloaderLua::createDownloadDataTask(const std::string& srcUrl, LUA_FUNCTION handler, const std::string& identifier)
{
    
    auto id = s_numb++;
    auto callback = handler;
    
    s_funmap[id] = callback;

    auto downloader = new network::Downloader();
    downloader->onTaskProgress = [=](const network::DownloadTask& task,
                                     int64_t bytesReceived,
                                     int64_t totalBytesReceived,
                                     int64_t totalBytesExpected)
    {
        auto __callback = s_funmap[id];
        if(__callback)
        {
            cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
            if (luaEngine)
            {
                cocos2d::LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(id);
                pStack->pushInt(1);
                pStack->pushInt((int)totalBytesExpected);
                pStack->pushInt((int)totalBytesReceived);
                pStack->executeFunctionByHandler(__callback,4);
                pStack->clean();
            }
        }
        CCLOG("onTaskProgress");
    };
    downloader->onDataTaskSuccess = [=](const cocos2d::network::DownloadTask& task,
                                        std::vector<unsigned char>& data)
    {
        auto __callback = s_funmap[id];
        if(__callback)
        {
            cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
            if (luaEngine)
            {
                cocos2d::LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(id);
                pStack->pushInt(0);
                pStack->pushString("");
                std::string dd(data.begin(), data.end());
                pStack->pushString(dd.data(),(int)dd.size());
                pStack->executeFunctionByHandler(__callback,4);
                pStack->clean();
                luaEngine->removeScriptHandler(__callback);
            }
            s_funmap[id] = 0;
        }
        CCLOG("onDataTaskSuccess");
        delete downloader;
    };
    downloader->onFileTaskSuccess = [=](const cocos2d::network::DownloadTask& task)
    {
        CCLOG("onFileTaskSuccess");
        delete downloader;
    };
    downloader->onTaskError = [=](const cocos2d::network::DownloadTask& task,
                                  int errorCode,
                                  int errorCodeInternal,
                                  const std::string& errorStr)
    {
        CCLOG("onTaskError");
        auto __callback = s_funmap[id];
        if(__callback)
        {
            cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
            if (luaEngine)
            {
                cocos2d::LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(id);
                pStack->pushInt(-1);
                pStack->pushInt(errorCode);
                pStack->pushString(errorStr.c_str(),(int)errorStr.size());
                pStack->executeFunctionByHandler(__callback,4);
                pStack->clean();
                luaEngine->removeScriptHandler(__callback);
            }
            s_funmap[id] = 0;
        }
        delete downloader;
    };
    
    downloader->createDownloadDataTask(srcUrl,identifier);
    
    return id;
}

int CCDownloaderLua::createDownloadFileTask(const std::string& srcUrl, const std::string& storagePath,LUA_FUNCTION handler, const std::string& identifier)
{
    auto id = s_numb++;
    auto callback = handler;
    s_funmap[id] = callback;
    
    auto downloader = new network::Downloader();
    downloader->onTaskProgress = [=](const network::DownloadTask& task,
                                        int64_t bytesReceived,
                                        int64_t totalBytesReceived,
                                        int64_t totalBytesExpected)
    {
        auto __callback = s_funmap[id];
        if(__callback)
        {
            cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
            if (luaEngine)
            {
                cocos2d::LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(id);
                pStack->pushInt(1);
                pStack->pushInt((int)totalBytesExpected);
                pStack->pushInt((int)totalBytesReceived);
                pStack->executeFunctionByHandler(__callback,4);
                pStack->clean();
            }
        }
        CCLOG("onTaskProgress");
    };
    
    downloader->onDataTaskSuccess = [=](const cocos2d::network::DownloadTask& task,
                                                      std::vector<unsigned char>& data)
    {
        CCLOG("onDataTaskSuccess");
        delete downloader;
    };
    downloader->onFileTaskSuccess = [=](const cocos2d::network::DownloadTask& task)
    {
        auto __callback = s_funmap[id];
        if(__callback)
        {
            cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
            if (luaEngine)
            {
                cocos2d::LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(id);
                pStack->pushInt(0);
                pStack->pushString(task.storagePath.c_str(),(int)task.storagePath.size());
                pStack->pushString("");
                pStack->executeFunctionByHandler(__callback,4);
                pStack->clean();
                luaEngine->removeScriptHandler(__callback);
            }
            s_funmap[id] = 0;
        }
        CCLOG("onFileTaskSuccess");
        delete downloader;
    };
    downloader->onTaskError = [=](const cocos2d::network::DownloadTask& task,
                                                int errorCode,
                                                int errorCodeInternal,
                                                const std::string& errorStr)
    {
        auto __callback = s_funmap[id];
        if(__callback)
        {
            cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
            if (luaEngine)
            {
                cocos2d::LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(id);
                pStack->pushInt(-1);
                pStack->pushInt(errorCode);
                pStack->pushString(errorStr.c_str(),(int)errorStr.size());
                pStack->executeFunctionByHandler(__callback,4);
                pStack->clean();
                luaEngine->removeScriptHandler(__callback);
            }
            s_funmap[id] = 0;
        }
        CCLOG("onTaskError");
        delete downloader;
    };
    
    if(FileUtils::getInstance()->isFileExist(storagePath))
    {
        FileUtils::getInstance()->removeFile(storagePath);
    }
    downloader->createDownloadFileTask(srcUrl,storagePath,identifier);
    
    return id;
}

void CCDownloaderLua::resetListener(int id)
{
    if(s_funmap.find(id)==s_funmap.end()) 
    {
        return;
    }
    auto __callback = s_funmap[id];
    if(__callback)
    {
        cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
        if (luaEngine)
        {
            luaEngine->removeScriptHandler(__callback);
        }
        s_funmap[id] = 0;
    }
}

void CCDownloaderLua::resetAllListener()
{
    cocos2d::LuaEngine* luaEngine = cocos2d::LuaEngine::getInstance();
    for(auto& it : s_funmap)
    {
        auto __callback = it.second;
        if(__callback)
        {
            luaEngine->removeScriptHandler(__callback);
        }
        it.second=0;
    }
}
  
