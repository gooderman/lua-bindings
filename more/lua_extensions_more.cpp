
#include "lua_extensions_more.h"

#include "zipfile/lua_zipfile_bindings.h"
#include "xxsocket/lua_xxsocket_bindings.h"
#include "crypto/lua_crypto_bindings.h"

#if __cplusplus
extern "C" {
#endif

// cjson
#include "cjson/lua_cjson.h"

// zlib
#include "zlib/lua_zlib.h"

// lpack
#include "lpack/lpack.h"

// filesystem
#include "filesystem/lfs.h"

// lsqlite3
#include "lsqlite3/lsqlite3.h"

// lunqlite3
#include "unqlite/lunqlite.h"

// protoc-gen-lua
#include "protobuf/pb.h"

#define CC_USE_SQLITE 1
#define CC_USE_UNQLITE 1
#define CC_USE_PROTOBUF 1
#define CC_USE_SPROTO 1
    
#if CC_USE_SPROTO
LUALIB_API int luaopen_lpeg (lua_State *L);
LUALIB_API int luaopen_sproto_core(lua_State *L);
LUALIB_API int luaopen_crypt(lua_State *L);
#endif

static luaL_Reg luax_exts[] = {
    {"cjson", luaopen_cjson_safe},
    {"zlib", luaopen_zlib},
    {"pack", luaopen_pack},
    {"lfs", luaopen_lfs},
#if CC_USE_SQLITE
    {"lsqlite3", luaopen_lsqlite3},
#endif
#if CC_USE_UNQLITE
	{"unqlite", luaopen_lunqlite},
#endif
#if CC_USE_PROTOBUF
    {"pb", luaopen_pb},
#endif
#if CC_USE_SPROTO
    {"lpeg", luaopen_lpeg},
    {"sproto.core", luaopen_sproto_core},
    {"crypt",luaopen_crypt},
#endif
    {NULL, NULL}
};

void luaopen_lua_extensions_more(lua_State *L)
{
    // load extensions
    luaL_Reg* lib = luax_exts;
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    for (; lib->func; lib++)
    {
        lua_pushcfunction(L, lib->func);
        lua_setfield(L, -2, lib->name);
    }

    lua_pop(L, 2);
    
    register_zipfile_binding(L);
    register_xxsocket_binding(L);
    register_crypto_binding(L);
}

#if __cplusplus
} // extern "C"
#endif


