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

#pragma once

#include <functional>
#include <string>
#include <memory>
#include <vector>

#include "CCLuaEngine.h"
#include "network/CCDownloader.h"

using namespace  cocos2d;
using namespace  cocos2d::network;


//handler(id,st,param...)

//ing
//handler(id,1,all,curr)

//succ
//handler(id,0,std::string file,std::string data)
//fail
//handler(id,-1,errorcode,std::string error)

class CCDownloaderLua
{
public:
    static int s_numb;
    static int createDownloadDataTask(const std::string& srcUrl, LUA_FUNCTION handler,const std::string& identifier = "");
    static int createDownloadFileTask(const std::string& srcUrl, const std::string& storagePath, LUA_FUNCTION handler,const std::string& identifier = "");
    static void resetListener(int id);
    static void resetAllListener();
};

