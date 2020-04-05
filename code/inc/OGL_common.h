/**
 * @file:   OGL_common.h
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  OGL
 * @author:
 * @date:   2020/3/29
 * @note:
 * @note \n History:
   1.日    期: 2020/3/29
     作    者:
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
//#include <GLES3/gl3.h>

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/
#ifdef WIN32
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl
#else
#define ESUTIL_API
#define ESCALLBACK
#endif

/* EL 执行表达式, 检查执行结果, 出错时返回	*/
#define EGL_RUN_CHECK_RET(expr) \
    do { \
        EGLBoolean bRet = (expr); \
        if(EGL_TRUE != bRet)\
        {\
			Cprintf_red("[%s %d] err:[0x%x],\n", __func__, __LINE__, eglGetError());\
			return -1;\
        }\
    } while (0)

/* GL 执行表达式, 检查执行结果, 出错时返回	*/
#define GL_RUN_CHECK_RET(expr) \
    do { \
    	(expr);\
        int32_t err = glGetError(); \
        if(err != GL_NO_ERROR)\
        {\
			Cprintf_red("[%s %d] err:[0x%x],\n", __func__, __LINE__, err);\
        	return -2;\
        }\
    } while (0)

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/
#ifndef _MY_EGL_CONTEXT_
#define _MY_EGL_CONTEXT_

typedef struct EGL_Context EGL_Context;

struct EGL_Context
{
	EGLNativeDisplayType eglNativeDisplay;	/* Display handle */
	EGLNativeWindowType  eglNativeWindow;	/* Window handle */
	EGLDisplay  eglDisplay;					/* EGL display */
	EGLContext  eglContext;					/* EGL context */
	EGLSurface  eglSurface;					/* EGL surface */

    BOOL   bBeReSizeSurface; /* 是否需要调用       glViewport 重配窗口大小 */
    int32_t s32SurfaceW;
    int32_t s32SurfaceH;
    int32_t s32NewSurfaceW;
    int32_t s32NewSurfaceH;
	void ( ESCALLBACK *resizeWinFunc ) ( EGL_Context *, uint32_t width, uint32_t height);

	uint32_t u32GLSLProgram;

    uint32_t u32DrawCnt;
    uint32_t u32DrawFps;
    uint32_t u32LastFpsCalcTime;

	/// Callbacks
	int32_t( ESCALLBACK *drawFunc ) ( EGL_Context * );
	int32_t( ESCALLBACK *shutdownFunc ) ( EGL_Context * );
	void ( ESCALLBACK *keyFunc ) ( EGL_Context *, unsigned char, int, int );
	void ( ESCALLBACK *updateFunc ) ( EGL_Context *, float deltaTime );
};
#endif

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/
uint32_t GL_CreateShader(uint32_t u32Type, const GLchar *ps8Src);
uint32_t GL_CreateProgram(const GLchar *ps8VertSrc, const GLchar *ps8FragSrc);
int32_t GL_SetupEGL(EGL_Context *pstEGL);
void CalcFpsInfo(EGL_Context *pstEGL);



