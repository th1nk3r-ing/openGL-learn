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


	bool bBeKeyUp;    
	bool bBeKeyDown;
	bool bBeKeyLeft;
	bool bBeKeyRight;

	float offset;
	void ( ESCALLBACK *keyFunc ) ( EGL_Context *, uint32_t u32Key);	

    bool bBeMouseMove;
	int32_t s32MouseSubX;
	int32_t s32MouseSubY;	
	void ( ESCALLBACK *mouseMoveFunc ) ( EGL_Context *, int32_t s32SubX, int32_t s32SubY);

    bool bBeMouseScroll;
	int32_t s32MouseYOffset;
	void ( ESCALLBACK *mouseScrollFunc ) ( EGL_Context *, int32_t s32MouseYOffset);
	
	void ( ESCALLBACK *updateFunc ) ( EGL_Context *, float deltaTime );
};


typedef struct _TEXTURE_INFO_
{
    uint32_t u32Texture;
    uint32_t u32TextureId;
    uint32_t u32GLSLProgram;
    int8_t   ps8TextureName[48];
    int32_t  s32FilterMode;
    int32_t  s32WarpMode;
    bool     bBeGenMipMap;
}TEXTURE_INFO;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
typedef enum _Camera_Key_ {
    FORWARD = 1,
    BACKWARD = 2,
    LEFT = 3,
    RIGHT = 4,
    UP = 5,    // 暂时不用(3D 游戏中, 上下移动时为蹦跳等重力作用);
    DOWN = 6,
}CameraKey;


/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/
uint32_t GL_CreateProgram(const GLchar *ps8VertSrc, const GLchar *ps8FragSrc);
uint32_t GL_CreateProgramFromFile(const char *ps8VertPath,const char *ps8FragPath);
int32_t GL_SetupEGL(EGL_Context *pstEGL);
int32_t GL_GetSurfaceWxH(EGL_Context *pstEGL, int32_t * ps32Width, int32_t * ps32Heigh);
void CalcFpsInfo(EGL_Context *pstEGL);
int32_t GL_InputProcess(EGL_Context *pstEGL, void * pCamHandle);

#endif

