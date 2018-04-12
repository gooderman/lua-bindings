#include "lua_download_bindings.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "CCDownloaderLua.h"

int lua_downloader_createDownloadDataTask(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.DownloaderLua",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (argc>=2)
    {
        std::string url;
        ok &= luaval_to_std_string(tolua_S, 2, &url, "cc.DownloaderLua:createDownloadDataTask");

 #if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        int handler = toluafix_ref_function(tolua_S,3,0);
        std::string identifer="";
        if(argc>2) 
        {
            ok &= luaval_to_std_string(tolua_S, 5, &identifer, "cc.DownloaderLua:createDownloadDataTask");
        }
        if(!ok) goto tolua_lerror;
        CCDownloaderLua::createDownloadDataTask(url,handler,identifer);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.DownloaderLua.createDownloadDataTask",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_downloader_createDownloadDataTask'.",&tolua_err);
#endif
    return 0;
}

int lua_downloader_createDownloadFileTask(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.DownloaderLua",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (argc>=3)
    {
        std::string url; 
        std::string storePath;

        ok &= luaval_to_std_string(tolua_S, 2, &url, "cc.DownloaderLua:createDownloadFileTask");
        ok &= luaval_to_std_string(tolua_S, 3, &storePath, "cc.DownloaderLua:createDownloadFileTask");

 #if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        int handler = toluafix_ref_function(tolua_S,4,0);
        std::string identifer="";
        if(argc>3) 
        {
            ok &= luaval_to_std_string(tolua_S, 5, &identifer, "cc.DownloaderLua:createDownloadFileTask");
        }
        if(!ok) goto tolua_lerror;
        CCDownloaderLua::createDownloadFileTask(url,storePath,handler,identifer);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.DownloaderLua.createDownloadFileTask",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_downloader_createDownloadFileTask'.",&tolua_err);
#endif
    return 0;
}


int lua_downloader_resetAllListener(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.DownloaderLua",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        CCDownloaderLua::resetAllListener();
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.DownloaderLua.resetAllListener",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_downloader_resetAllListener'.",&tolua_err);
#endif
    return 0;
}

int lua_downloader_resetListener(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.DownloaderLua",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {
        int value=0; 
        ok &= luaval_to_int32(tolua_S,2,&value, "cc.DownloaderLua.resetListener");
        if(!ok) goto tolua_lerror;
        CCDownloaderLua::resetListener(value);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.DownloaderLua.resetListener",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_downloader_resetListener'.",&tolua_err);
#endif
    return 0;
}

int lua_register_cocos2dx_download(lua_State* L)
{
    tolua_usertype(L,"cc.DownloaderLua");
    tolua_cclass(L,"DownloaderLua","cc.DownloaderLua","",nullptr);
    
    tolua_beginmodule(L,"DownloaderLua");
    tolua_function(L,"createDownloadDataTask",lua_downloader_createDownloadDataTask);
    tolua_function(L,"createDownloadFileTask",lua_downloader_createDownloadFileTask);
    tolua_function(L,"resetListener",lua_downloader_resetListener);
    tolua_function(L,"resetAllListener",lua_downloader_resetAllListener);
    tolua_endmodule(L);
    return 1;
}

int register_download_binding(lua_State* L)
{
    tolua_open(L);
    tolua_module(L, "cc", 0);
    tolua_beginmodule(L, "cc");
        lua_register_cocos2dx_download(L);
    tolua_endmodule(L);
    return 0;
}
