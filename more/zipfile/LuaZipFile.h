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

#ifndef __LUA_ZIPFILE_H__
#define __LUA_ZIPFILE_H__
/// @cond DO_NOT_SHOW

#include "cocos2d.h"

/**
 * @addtogroup base
 * @{
 */

namespace cocos2d
{
    class CC_DLL LuaZipFile : public ZipFile,public Ref
    {
    public:
        /**
        * Constructor, open zip file and store file list.
        *
        * @param zipFile Zip file name
        * @param filter The first part of file names, which should be accessible.
        *               For example, "assets/". Other files will be missed.
        *
        * @since v2.0.5
        */
        LuaZipFile(const std::string &zipFile, const std::string &filter = std::string());
        virtual ~LuaZipFile();
        
        static LuaZipFile *createWithBuffer(const void* buffer, unsigned long size);
        static LuaZipFile *create(const std::string &zipFile, const std::string &filter = std::string());
        
        //解压文件
        bool extract(std::string&root);
        //解压文件
        bool extract_ex(std::string&root,int handler=0);
        //异步解压文件
        int extract_async(std::string&root,int handler=0);
        //返回所有文件名
        void getFileList();
        //异步解压线程
        std::thread* m_Thread;
    public:
        /* Only used internal for createWithBuffer() */
        LuaZipFile();
        
    };
} // end of namespace cocos2d

// end group
/// @}

/// @endcond
#endif
