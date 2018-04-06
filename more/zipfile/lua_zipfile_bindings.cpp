#include "lua_zipfile_bindings.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "LuaZipFile.h"

int lua_cocos2dx_ZipFile_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.LuaZipFile",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc >= 1 )
    {
        std::string arg0;
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.LuaZipFile:create");
        if (!ok)  goto tolua_lerror;
        if(argc == 2)
        {
            ok &= luaval_to_std_string(tolua_S, 3,&arg1, "cc.LuaZipFile:create");
            if (!ok)  goto tolua_lerror;
        }
        cocos2d::LuaZipFile* ret = cocos2d::LuaZipFile::create(arg0,arg1);
        if(!ret)  goto tolua_lerror;
        object_to_luaval<cocos2d::LuaZipFile>(tolua_S, "cc.LuaZipFile",(cocos2d::LuaZipFile*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.LuaZipFile:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_ZipFile_create'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_ZipFile_createWithBuffer(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.LuaZipFile",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 1 )
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "cc.LuaZipFile:createWithBuffer");
        if (!ok)  goto tolua_lerror;
        cocos2d::LuaZipFile* ret = cocos2d::LuaZipFile::createWithBuffer(arg0.data(),arg0.size());
        if(!ret)  goto tolua_lerror;
        object_to_luaval<cocos2d::LuaZipFile>(tolua_S, "cc.LuaZipFile",(cocos2d::LuaZipFile*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.LuaZipFile:createWithBuffer",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_ZipFile_createWithBuffer'.",&tolua_err);
#endif
    return 0;
}


int lua_cocos2dx_ZipFile_getFileData(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    LuaZipFile* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.LuaZipFile",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<LuaZipFile *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_ZipFile_getFileData'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.LuaZipFile:getFileData");
        if (ok)
        {
            ssize_t len=0;
            unsigned char * buff = self->getFileData(arg0,&len);
            if (buff)
            {
                lua_pushlstring(tolua_S, (const char*)buff, len);
                free(buff);
            } 
            return 1;
        }
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.LuaZipFile:lua_cocos2dx_ZipFile_getFileData",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_ZipFile_getFileData'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_ZipFile_extract(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    LuaZipFile* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.LuaZipFile",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<LuaZipFile *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_ZipFile_extract'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {
        std::string arg0; 
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.LuaZipFile:extract");
        if (ok)
        {
            bool ret = self->extract(arg0);
            tolua_pushboolean(tolua_S, ret);
            return 1;
        }
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.LuaZipFile:lua_cocos2dx_ZipFile_extract",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_ZipFile_extract'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_ZipFile_getFileList(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    LuaZipFile* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.LuaZipFile",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<LuaZipFile *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_ZipFile_getFileList'\n", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->getFileList();
        return 1;
    }
    
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.LuaZipFile:lua_cocos2dx_ZipFile_getFileList",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_ZipFile_getFileList'.",&tolua_err);
#endif
    return 0;
}


int lua_cocos2dx_ZipFile_extract_async(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    LuaZipFile* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.LuaZipFile",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<LuaZipFile *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_ZipFile_extract_async'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (2 == argc)
    {
        std::string arg0; 
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.LuaZipFile:extract_async");

#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        int handler = toluafix_ref_function(tolua_S,3,0);
        self->extract_async(arg0,handler);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.LuaZipFile:lua_cocos2dx_ZipFile_extract_async",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_ZipFile_extract_async'.",&tolua_err);
#endif
    return 0;
}

int lua_register_cocos2dx_zipfile(lua_State* L)
{
    tolua_usertype(L,"cc.LuaZipFile");
    tolua_cclass(L,"LuaZipFile","cc.LuaZipFile","cc.Ref",nullptr);
    
    tolua_beginmodule(L,"LuaZipFile");
    tolua_function(L,"extract_async",lua_cocos2dx_ZipFile_extract_async);
    tolua_function(L,"extract",lua_cocos2dx_ZipFile_extract);
    tolua_function(L,"getFileData",lua_cocos2dx_ZipFile_getFileData);
    tolua_function(L,"getFileList",lua_cocos2dx_ZipFile_getFileList);
    tolua_function(L,"createWithBuffer",lua_cocos2dx_ZipFile_createWithBuffer);
    tolua_function(L,"create", lua_cocos2dx_ZipFile_create);
    tolua_endmodule(L);
    std::string typeName = typeid(cocos2d::LuaZipFile).name();
    g_luaType[typeName] = "cc.LuaZipFile";
    g_typeCast["LuaZipFile"] = "cc.LuaZipFile";
    return 1;
}

int register_zipfile_binding(lua_State* L)
{
    tolua_open(L);
    tolua_module(L, "cc", 0);
    tolua_beginmodule(L, "cc");
        lua_register_cocos2dx_zipfile(L);
    tolua_endmodule(L);
    return 0;
}
