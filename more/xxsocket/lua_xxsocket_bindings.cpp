#include "lua_xxsocket_bindings.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "NetXX.h"
USING_NS_CC;
using namespace network;
int lua_cocos2dx_NetXX_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc ==0 )
    {
        NetXX* ret = NetXX::getInstance();
        if(!ret)  goto tolua_lerror;
        object_to_luaval<NetXX>(tolua_S, "cc.NetXX",(NetXX*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_getInstance'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc ==0 )
    {
        NetXX* ret = NetXX::create();
        if(!ret)  goto tolua_lerror;
        object_to_luaval<NetXX>(tolua_S, "cc.NetXX",(NetXX*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_create'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_init(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_init'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->init();
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:init",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_init'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_shutdown(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_shutdown'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->shutdown();
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:shutdown",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_shutdown'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_setLuaListener(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_setLuaListener'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {

#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err)) {
            goto tolua_lerror;
        }
#endif
        int handler = toluafix_ref_function(tolua_S,2,0);
        self->setLuaListener(handler);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:setLuaListener",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_setLuaListener'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_delLuaListener(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_delLuaListener'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->delLuaListener();
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:delLuaListener",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_delLuaListener'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_setPauseReceive(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_setPauseReceive'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {
        bool flag;
        ok &= luaval_to_boolean(tolua_S, 2, &flag, "cc.NetXX:setConnectWaitTimeout");
        if(!ok) goto tolua_lerror;
        self->setPauseReceive(flag);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:setPauseReceive",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_setPauseReceive'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_setEndpoint(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_setEndpoint'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (3 == argc)
    {
        std::string arg0; 
        std::string arg1;
        ushort port; 
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.NetXX:setEndpoint");
        if(!ok) goto tolua_lerror;
        ok &= luaval_to_std_string(tolua_S, 3, &arg1, "cc.NetXX:setEndpoint");
        if(!ok) goto tolua_lerror;
        ok &= luaval_to_ushort(tolua_S, 4, &port, "cc.NetXX:setEndpoint");
        if(!ok) goto tolua_lerror;
        self->setEndpoint(arg0.c_str(),arg1.c_str(),port);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:setEndpoint",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_setEndpoint'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_startConnect(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_startConnect'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->startConnect();
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:startConnect",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_startConnect'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_reConnect(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_reConnect'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->reConnect();
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:reConnect",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_reConnect'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_setConnectWaitTimeout(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_setConnectWaitTimeout'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {
        long time=0;
        ok &= luaval_to_long(tolua_S, 2, &time, "cc.NetXX:setConnectWaitTimeout");
        if(!ok) goto tolua_lerror;
        self->setConnectWaitTimeout(time);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:setConnectWaitTimeout",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_setConnectWaitTimeout'.",&tolua_err);
#endif
    return 0;
}

int lua_cocos2dx_NetXX_setSendTimeout(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_setSendTimeout'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if (1 == argc)
    {
        long time=0;
        ok &= luaval_to_long(tolua_S, 2, &time, "cc.NetXX:setSendTimeout");
        if(!ok) goto tolua_lerror;
        self->setSendTimeout(time);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:setSendTimeout",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_setSendTimeout'.",&tolua_err);
#endif
    return 0;
}


int lua_cocos2dx_NetXX_sendMsg(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
    NetXX* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.NetXX",0,&tolua_err)) goto tolua_lerror;
#endif

    self = static_cast<NetXX *>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'lua_cocos2dx_NetXX_sendMsg'\n", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S) - 1;
    
    if ( argc >=1 )
    {
        // size_t len;
        // const char* cmsg = lua_tolstring(tolua_S, 2, &len);
        std::string arg0; 
        int sid = 0;
        ok &= luaval_to_std_string(tolua_S, 2, &arg0, "cc.NetXX:sendMsg");
        if(!ok) goto tolua_lerror;
        if(argc==2)
        {
            ok &= luaval_to_int32(tolua_S, 3, &sid, "cc.NetXX:sendMsg");
            if(!ok) goto tolua_lerror;
        }
        self->sendMsg(arg0);
        return 1;
    }

    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "cc.NetXX:sendMsg",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_NetXX_sendMsg'.",&tolua_err);
#endif
    return 0;
}


int lua_register_cocos2dx_xxsocket(lua_State* L)
{
    tolua_usertype(L,"cc.NetXX");
    tolua_cclass(L,"NetXX","cc.NetXX","cc.Ref",nullptr);
    
    tolua_beginmodule(L,"NetXX");
    tolua_function(L,"sendMsg",lua_cocos2dx_NetXX_sendMsg);
    tolua_function(L,"setSendTimeout",lua_cocos2dx_NetXX_setSendTimeout);
    tolua_function(L,"setConnectWaitTimeout",lua_cocos2dx_NetXX_setConnectWaitTimeout);
    tolua_function(L,"reConnect",lua_cocos2dx_NetXX_reConnect);
    tolua_function(L,"startConnect",lua_cocos2dx_NetXX_startConnect);
    tolua_function(L,"setEndpoint",lua_cocos2dx_NetXX_setEndpoint);
    tolua_function(L,"setPauseReceive",lua_cocos2dx_NetXX_setPauseReceive);
    tolua_function(L,"delLuaListener",lua_cocos2dx_NetXX_delLuaListener);
    tolua_function(L,"setLuaListener",lua_cocos2dx_NetXX_setLuaListener);
    tolua_function(L,"shutdown",lua_cocos2dx_NetXX_shutdown);
    tolua_function(L,"init",lua_cocos2dx_NetXX_init);
    tolua_function(L,"getInstance", lua_cocos2dx_NetXX_getInstance);
    tolua_function(L,"create", lua_cocos2dx_NetXX_create);
    tolua_endmodule(L);
    std::string typeName = typeid(NetXX).name();
    g_luaType[typeName] = "cc.NetXX";
    g_typeCast["NetXX"] = "cc.NetXX";
    return 1;
}

int register_xxsocket_binding(lua_State* L)
{
    tolua_open(L);
    tolua_module(L, "cc", 0);
    tolua_beginmodule(L, "cc");
        lua_register_cocos2dx_xxsocket(L);
    tolua_endmodule(L);
    return 0;
}
