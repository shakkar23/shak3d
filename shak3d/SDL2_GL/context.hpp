#pragma once
#ifdef _WIN32

//---------------------------------------------------------------------------
//  On Win32, use our own GL headers and prepare a function table
//---------------------------------------------------------------------------
#include "SDL2/SDL_opengles2_khrplatform.h"
#include "SDL2/SDL_opengles2_gl2platform.h"
#include "SDL2/SDL_opengles2_gl2.h"
#include "SDL2/SDL_opengles2_gl2ext.h"
#include <assert.h>

struct GLFuncTable {
#define X(retVal, name, args) retVal (GL_APIENTRY *name)args;
#include "funcs.hpp"
#undef X
	void initialize();
};
extern GLFuncTable glFuncTable;
#define GL_CHECK(call) do { glFuncTable.call; assert(glFuncTable.GetError() == GL_NO_ERROR); } while (0)
#define GL_NO_CHECK(call) (glFuncTable.call)

#else // TURF_TARGET_WIN32

//---------------------------------------------------------------------------
//  Otherwise, use the system's GL headers
//---------------------------------------------------------------------------
#ifdef __APPLE__
#import <OpenGL/gl.h>
#else
#include <GLES2/gl2.h>
#endif
#define GL_CHECK(call) do { gl ## call; assert(glGetError() == GL_NO_ERROR); } while (0)
#define GL_NO_CHECK(call) (gl ## call)

#endif // TURF_TARGET_WIN32
