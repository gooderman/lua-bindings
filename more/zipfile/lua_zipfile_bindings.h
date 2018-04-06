#ifndef COCOS2DX_LUA_ZIPFILE_BINDINGS_H
#define COCOS2DX_LUA_ZIPFILE_BINDINGS_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_zipfile_binding(lua_State* tolua_S);

#endif // #ifndef COCOS2DX_LUA_ZIPFILE_BINDINGS_H
