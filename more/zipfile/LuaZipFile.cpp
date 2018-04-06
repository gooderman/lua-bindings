/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2017 Chukong Technologies Inc.

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
#include "CCLuaStack.h"
#include "CCLuaEngine.h"
// FIXME: hack, must be included before ziputils
#ifdef MINIZIP_FROM_SYSTEM
#include <minizip/unzip.h>
#else // from our embedded sources
#include "external/unzip/unzip.h"
#endif

#include "LuaZipFile.h"

#include <zlib.h>
#include <assert.h>
#include <stdlib.h>

#include "base/CCData.h"
#include "base/ccMacros.h"
#include "platform/CCFileUtils.h"
#include <map>

// FIXME: Other platforms should use upstream minizip like mingw-w64  
#ifdef MINIZIP_FROM_SYSTEM
#define unzGoToFirstFile64(A,B,C,D) unzGoToFirstFile2(A,B,C,D, NULL, 0, NULL, 0)
#define unzGoToNextFile64(A,B,C,D) unzGoToNextFile2(A,B,C,D, NULL, 0, NULL, 0)
#endif

NS_CC_BEGIN

// --------------------- ZipFile ---------------------
// from unzip.cpp
#define UNZ_MAXFILENAMEINZIP 256

static const std::string emptyFilename("");

struct ZipEntryInfo
{
    unz_file_pos pos;
    uLong uncompressed_size;
};

class ZipFilePrivate
{
public:
    unzFile zipFile;
    
    // std::unordered_map is faster if available on the platform
    typedef std::unordered_map<std::string, struct ZipEntryInfo> FileListContainer;
    FileListContainer fileList;
};

LuaZipFile *LuaZipFile::createWithBuffer(const void* buffer, uLong size)
{
    LuaZipFile *zip = new (std::nothrow) LuaZipFile();
    if (zip && zip->initWithBuffer(buffer, size)) {
        zip->autorelease();
        return zip;
    } else {
        if (zip) delete zip;
        return nullptr;
    }
}
LuaZipFile *LuaZipFile::create(const std::string &zipFile, const std::string &filter)
{
    LuaZipFile *zip = new (std::nothrow) LuaZipFile(zipFile,filter);
    if (zip) {
        zip->autorelease();
        return zip;
    } else {
        return nullptr;
    }
}

LuaZipFile::LuaZipFile()
: ZipFile()
{
    m_Thread = nullptr;
}

LuaZipFile::LuaZipFile(const std::string &zipFile, const std::string &filter)
: ZipFile(zipFile,filter)
{
    m_Thread = nullptr;
}

LuaZipFile::~LuaZipFile()
{
    if (_data && _data->zipFile)
    {
        unzClose(_data->zipFile);
    }

    CC_SAFE_DELETE(_data);
    
    if (m_Thread) m_Thread->join();
    CC_SAFE_DELETE(m_Thread);
}

bool LuaZipFile::extract(std::string&root)
{
    return extract_ex(root,0);
}
//解压文件
bool LuaZipFile::extract_ex(std::string&root,int handler)
{
    bool flag = false;
    auto fs = FileUtils::getInstance();
    do{
        if(root.empty()) {
            break;
        }
        auto len = root.length();
        if(root[0]!='/'){
            break;
        }
        if(root[len-1]!='/'){
            root+='/';
        }
        if(!fs->isDirectoryExist(root)){
            if(!fs->createDirectory(root)){
                break;
            }
        }
        int ct = 0;
        int max = _data->fileList.size();
        for(auto ff:_data->fileList)
        {
            auto & name = ff.first;
            auto ll = name.length();
            if(name[ll-1]=='/')
            {
                ct++;
                if(!fs->createDirectory(root+name)){
                    break;
                }
                continue;
            }
            ssize_t sz = 0;
            unsigned char* data = this->getFileData(name,&sz);
            if(data)
            {
                auto dd = Data();
                dd.fastSet(data, sz);
                auto f = fs->writeDataToFile(dd,root+name);
                if(!f) {
                    CCLOG("extract fail to writeDataToFile %s",name.c_str());
                    break;
                }
            }
            else{
                CCLOG("extract fail to getFileData %s",name.c_str());
                break;
            }
            ct++;
            if(handler && ct%30==0)
            {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,ct,max,handler](){
                    LuaEngine* luaEngine = LuaEngine::getInstance();
                    LuaStack *pStack = luaEngine->getLuaStack();
                    pStack->pushInt(1);
                    pStack->pushInt(max);
                    pStack->pushInt(ct);
                    pStack->executeFunctionByHandler(handler,3);
                    pStack->clean();
                });
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
        flag = true;
    }while(0);
    return flag;
}
//异步解压文件
int LuaZipFile::extract_async(std::string&root,int handler)
{
    if(m_Thread){
        return 1;
    }
    this->retain();
    m_Thread =  new std::thread([this,root,handler](){
        auto froot = root;
        auto f = this->extract_ex(froot,handler);
        auto h = handler;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,f,h](){
            LuaEngine* luaEngine = LuaEngine::getInstance();
            if (h && luaEngine)
            {
                LuaStack *pStack = luaEngine->getLuaStack();
                pStack->pushInt(f ? 0 : -1);
                pStack->executeFunctionByHandler(h,1);
                pStack->clean();
                luaEngine->removeScriptHandler(h);
            }
            this->release();
        });
    });
    return 0;

}

//返回所有文件名
void LuaZipFile::getFileList()
{
    LuaValueArray array;
    for(auto a : _data->fileList)
    {
        LuaValue value = LuaValue::stringValue(a.first);
        array.push_back(value);
    }
    LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
    stack->clean();
    stack->pushLuaValueArray(array);
    return;
}

NS_CC_END
