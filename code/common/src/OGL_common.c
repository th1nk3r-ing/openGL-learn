/**
 * @file:   OGL_common.c
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  openGL 公用方法
 * @author:
 * @date:   2020/4/2
 * @note:
 * @note \n History:
   1.日    期: 2020/4/2
     作    者:
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include "common.h"
#include "build_time.h"
#include "OGL_common.h"

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/
#define OPENGL_FPS_CALC_INTERVAL    (30u)

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

/**
 * @function:   __windowResizeCallback
 * @brief:      调整绘制窗口大小
 * @param[in]:  EGL_Context *pstEGL
 * @param[in]:  uint32_t width
 * @param[in]:  uint32_t height
 * @param[out]: None
 * @return:     void
 */
static void __windowResizeCallback(EGL_Context *pstEGL, uint32_t width, uint32_t height)
{
    BASE_CHECK_TRUE_WARN(NULL == pstEGL);
    BASE_CHECK_TRUE_WARN(0 == width);
    BASE_CHECK_TRUE_WARN(0 == height);

    Cprintf_white("[%s %d]  original:[%d x %d] now:[%d x %d]\n", __func__, __LINE__,
        pstEGL->s32SurfaceW, pstEGL->s32SurfaceH, width, height);

    pstEGL->s32NewSurfaceW = width;
    pstEGL->s32NewSurfaceH = height;
    pstEGL->bBeReSizeSurface = TRUE;

	return;
}


/**
 * @function:   _GL_CreateShader
 * @brief:      创建着色器
 * @param[in]:  uint32_t u32Type
 * @param[in]:  const GLchar *ps8Src
 * @param[out]: None
 * @return:     static uint32_t
 */
static uint32_t _GL_CreateShader(uint32_t u32Type, const GLchar *ps8Src)
{
    uint32_t u32Shader = 0;
    int32_t s32Status = 0;

    u32Shader = glCreateShader(u32Type);
    glShaderSource(u32Shader, 1, &ps8Src, NULL);
    glCompileShader(u32Shader);
    glGetShaderiv(u32Shader, GL_COMPILE_STATUS, &s32Status);
    if (s32Status != GL_TRUE)
    {
        GLchar s8ErrInfo[1024] = {0};

        glGetShaderInfoLog(u32Shader, sizeof(s8ErrInfo), NULL, s8ErrInfo);
		Cprintf_red("[%s %d]  [0x%x]:\n%s\n", __func__, __LINE__, u32Type, s8ErrInfo);
        glDeleteShader(u32Shader);
        return 0;
    }

    Cprintf_white( "Shader compiled successfully\n");

    return u32Shader;
}

/**
 * @function:   GL_CreateProgram
 * @brief:      创建 GLSL 程序
 * @param[in]:  const GLchar *ps8VertSrc
 * @param[in]:  const GLchar *ps8FragSrc
 * @param[out]: None
 * @return:     static uint32_t
 */
uint32_t GL_CreateProgram(const GLchar *ps8VertSrc, const GLchar *ps8FragSrc)
{
    BASE_CHECK_TRUE_RET(NULL == ps8VertSrc, -1);
    BASE_CHECK_TRUE_RET(NULL == ps8FragSrc, -1);

    uint32_t u32VertShader = _GL_CreateShader(GL_VERTEX_SHADER, ps8VertSrc);
 	BASE_CHECK_TRUE_RET(0 == u32VertShader, -2);

    uint32_t u32FragShader = _GL_CreateShader(GL_FRAGMENT_SHADER, ps8FragSrc);
 	BASE_CHECK_TRUE_RET(0 == u32FragShader, -2);

    uint32_t u32Program = glCreateProgram();
 	BASE_CHECK_TRUE_RET(0 == u32FragShader, -2);

    GL_RUN_CHECK_RET(glAttachShader(u32Program, u32VertShader));
    GL_RUN_CHECK_RET(glAttachShader(u32Program, u32FragShader));
    glLinkProgram(u32Program);

    int32_t s32Status = 0;
    glGetProgramiv(u32Program, GL_LINK_STATUS, &s32Status);
    if(s32Status != GL_TRUE)
    {
        GLchar s8ErrInfo[1024];
        glGetProgramInfoLog(u32Program, sizeof(s8ErrInfo), NULL, s8ErrInfo);
		Cprintf_red("[%s %d] ERR @ link. \n\t %s\n", __func__, __LINE__, s8ErrInfo);
        return 0;
    }

    GL_RUN_CHECK_RET(glDeleteShader(u32VertShader));
    GL_RUN_CHECK_RET(glDeleteShader(u32FragShader));

    Cprintf_green( "GLSL link success\n");

    return u32Program;
}


/**
 * @function:   GL_CreateProgram
 * @brief:      创建 GLSL 程序
 * @param[in]:  const GLchar *ps8VertSrc
 * @param[in]:  const GLchar *ps8FragSrc
 * @param[out]: None
 * @return:     static uint32_t
 */
uint32_t GL_CreateProgramFromFile(const char *ps8VertPath,const char *ps8FragPath)
{
    BASE_CHECK_TRUE_RET(NULL == ps8VertPath, -1);
    BASE_CHECK_TRUE_RET(NULL == ps8FragPath, -1);

    GLchar *ps8VertSrc = (GLchar *)readFile(NULL, ps8VertPath, 0);
    uint32_t u32VertShader = _GL_CreateShader(GL_VERTEX_SHADER, ps8VertSrc);
    free(ps8VertSrc);
 	BASE_CHECK_TRUE_RET(0 == u32VertShader, -2);

    GLchar *ps8FragSrc = (GLchar *)readFile(NULL, ps8FragPath, 0);
    uint32_t u32FragShader = _GL_CreateShader(GL_FRAGMENT_SHADER, ps8FragSrc);
    free(ps8FragSrc);
 	BASE_CHECK_TRUE_RET(0 == u32FragShader, -2);

    uint32_t u32Program = glCreateProgram();
 	BASE_CHECK_TRUE_RET(0 == u32FragShader, -2);

    GL_RUN_CHECK_RET(glAttachShader(u32Program, u32VertShader));
    GL_RUN_CHECK_RET(glAttachShader(u32Program, u32FragShader));
    glLinkProgram(u32Program);

    int32_t s32Status = 0;
    glGetProgramiv(u32Program, GL_LINK_STATUS, &s32Status);
    if(s32Status != GL_TRUE)
    {
        GLchar s8ErrInfo[1024];
        glGetProgramInfoLog(u32Program, sizeof(s8ErrInfo), NULL, s8ErrInfo);
		Cprintf_red("[%s %d] ERR @ link. \n\t %s\n", __func__, __LINE__, s8ErrInfo);
        return 0;
    }

    GL_RUN_CHECK_RET(glDeleteShader(u32VertShader));
    GL_RUN_CHECK_RET(glDeleteShader(u32FragShader));

    Cprintf_green( "GLSL link success!! \n\tVertextShader:[%s] \n\tFragementShader:[%s]\n",
                    ps8VertPath, ps8FragPath);

    return u32Program;
}


/**
 * @function:   GL_SetupEGL
 * @brief:      设置 EGL
 * @param[in]:  EGL_Context *pstEGL
 * @param[out]: None
 * @return:     int32_t
 */
int32_t GL_SetupEGL(EGL_Context *pstEGL)
{
    BASE_CHECK_TRUE_RET(NULL == pstEGL, -1);

    EGL_RUN_CHECK_RET(eglBindAPI(EGL_OPENGL_ES_API));

    /* 获取默认 display 参数 */
    pstEGL->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    BASE_CHECK_TRUE_RET(pstEGL->eglDisplay == EGL_NO_DISPLAY, -1);

    int32_t s32MajorVersion = 0, s32MinorVersion = 0;
    EGL_RUN_CHECK_RET(eglInitialize(pstEGL->eglDisplay, &s32MajorVersion, &s32MinorVersion));
    Cprintf_white( "EGL-Version:[%d.%d]\n", s32MajorVersion, s32MinorVersion);

	/* 查询 EGL 版本信息 */
    const char *ps8ClientApis = eglQueryString(pstEGL->eglDisplay, EGL_CLIENT_APIS);
    const char *ps8Vendor = eglQueryString(pstEGL->eglDisplay, EGL_VENDOR);
    const char *ps8Version = eglQueryString(pstEGL->eglDisplay, EGL_VERSION);
    Cprintf_white( "EGL_CLIENT_APIS: %s\n", ps8ClientApis);
    Cprintf_white( "EGL_VENDOR: %s\n", ps8Vendor);
    Cprintf_white( "EGL_VERSION: %s\n\n", ps8Version);

    // Get a matching config
    int32_t as32ConfigAttrs[] =
    {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE
    };
    int32_t s32CfgCount = 0;
    EGLConfig pvConfig = NULL;
    EGL_RUN_CHECK_RET(eglChooseConfig(pstEGL->eglDisplay, as32ConfigAttrs,
    						  &pvConfig, 1, &s32CfgCount));

    /* 通过 NativeWindwo 创建 egl Surface */
    int32_t as32SurfaceAttrs[] = {EGL_NONE};
    pstEGL->eglSurface = eglCreateWindowSurface(pstEGL->eglDisplay, pvConfig,
                                               pstEGL->eglNativeWindow, as32SurfaceAttrs);
	BASE_CHECK_TRUE_RET(pstEGL->eglNativeWindow == EGL_NO_SURFACE, -1);

    /* 获取 surface 的宽高 */
    EGL_RUN_CHECK_RET(eglQuerySurface(pstEGL->eglDisplay, pstEGL->eglSurface,
                                   EGL_WIDTH, &pstEGL->s32SurfaceW));
    EGL_RUN_CHECK_RET(eglQuerySurface(pstEGL->eglDisplay, pstEGL->eglSurface,
                                   EGL_HEIGHT, &pstEGL->s32SurfaceH));
    Cprintf_yellow( "surface: WxH:[%d x %d]\n", pstEGL->s32SurfaceW, pstEGL->s32SurfaceH);

    /* 创建 Context */
    int32_t as32ContextAttrs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    pstEGL->eglContext = eglCreateContext(pstEGL->eglDisplay,
                                          pvConfig, EGL_NO_CONTEXT, as32ContextAttrs);
	BASE_CHECK_TRUE_RET( EGL_NO_CONTEXT == pstEGL->eglContext, -1);

    /* 绑定 Context 至当前线程 */
    EGL_RUN_CHECK_RET(eglMakeCurrent(pstEGL->eglDisplay, pstEGL->eglSurface,
    						 pstEGL->eglSurface, pstEGL->eglContext));

    /* 配置窗口大小回调函数 */
    pstEGL->resizeWinFunc = __windowResizeCallback;

    CalcFpsInfo(pstEGL);

    return OK;
}


/**
 * @function:   CalcFpsInfo
 * @brief:      计算帧率
 * @param[in]:  EGL_Context *pstEGL
 * @param[out]: None
 * @return:     void
 */
void CalcFpsInfo(EGL_Context *pstEGL)
{
    pstEGL->u32DrawCnt++;

    if(pstEGL->u32DrawCnt % OPENGL_FPS_CALC_INTERVAL)
    {
        return;
    }

    uint32_t u32NowTime = getTime_ms();
    pstEGL->u32DrawFps = (uint32_t)(OPENGL_FPS_CALC_INTERVAL * 1000.0 /
                                        (float)(u32NowTime - pstEGL->u32LastFpsCalcTime) + 0.5);
    Cprintf_green("[%s %d]  disp:[%d], fps:[%d], thisT:[%d]\n",
                    __func__, __LINE__, pstEGL->u32DrawCnt++,
                    pstEGL->u32DrawFps, u32NowTime);

    pstEGL->u32LastFpsCalcTime = u32NowTime;

    return;
}



