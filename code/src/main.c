/**
 * @file:   main.c
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  主函数文件
 * @author: think3r
 * @date:   2020/3/29
 * @note:
 * @note \n History:
   1.日    期: 2020/3/29
     作    者: think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include "common.h"
#include "build_time.h"
#include "OGL_common.h"
#include "win32_platform.h"

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/
char vShaderStr[] =
   "#version 300 es 						 \n"
   "layout(location = 0) in vec4 vPosition;  \n"
   "void main() 							 \n"
   "{										 \n"
   "   gl_Position = vPosition; 			 \n"
   "}										 \n";

char fShaderStr[] =
   "#version 300 es 							 \n"
   "precision mediump float;					 \n"
   "out vec4 fragColor; 						 \n"
   "void main() 								 \n"
   "{											 \n"
   "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
   "}											 \n";


/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

static int32_t setupEGL(EGL_Context *pstEGL)
{
    int32_t as32SurfaceAttrs[] = {EGL_NONE};
    int32_t s32MajorVersion = 0;
    int32_t s32MinorVersion = 0;
    EGLConfig pvConfig = NULL;
    int32_t s32CfgCount = 0;
    const char *ps8ClientApis = NULL;
    const char *ps8Vendor = NULL;
    const char *ps8Version = NULL;

    int32_t as32ConfigAttrs[] =
    {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_NONE
    };
    int32_t as32ContextAttrs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGL_EXECUTE_CHECK_RET(eglBindAPI(EGL_OPENGL_ES_API));

    pstEGL->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    BASE_CHECK_TRUE_RET(pstEGL->eglDisplay == EGL_NO_DISPLAY, -1);

    EGL_EXECUTE_CHECK_RET(eglInitialize(pstEGL->eglDisplay, &s32MajorVersion, &s32MinorVersion));
    Cprintf_white( "EGL-Version:[%d.%d]\n", s32MajorVersion, s32MinorVersion);

	/* 查询 EGL 信息 */
    ps8ClientApis = eglQueryString(pstEGL->eglDisplay, EGL_CLIENT_APIS);
    ps8Vendor = eglQueryString(pstEGL->eglDisplay, EGL_VENDOR);
    ps8Version = eglQueryString(pstEGL->eglDisplay, EGL_VERSION);
    Cprintf_white( "EGL_CLIENT_APIS: %s\n", ps8ClientApis);
    Cprintf_white( "EGL_VENDOR: %s\n", ps8Vendor);
    Cprintf_white( "EGL_VERSION: %s\n\n", ps8Version);

    // Get a matching config
    EGL_EXECUTE_CHECK_RET(eglChooseConfig(pstEGL->eglDisplay, as32ConfigAttrs,
    						  &pvConfig, 1, &s32CfgCount));

    // Create EGL surface with native window
    pstEGL->eglSurface = eglCreateWindowSurface(pstEGL->eglDisplay, pvConfig,
                                               pstEGL->eglNativeWindow, as32SurfaceAttrs);
	BASE_CHECK_TRUE_RET(pstEGL->eglNativeWindow == EGL_NO_SURFACE, -1);

    // 获取 surface 的宽高
    EGL_EXECUTE_CHECK_RET(eglQuerySurface(pstEGL->eglDisplay,
                                   pstEGL->eglSurface,
                                   EGL_WIDTH, &pstEGL->s32SurfaceW));
    EGL_EXECUTE_CHECK_RET(eglQuerySurface(pstEGL->eglDisplay,
                                   pstEGL->eglSurface,
                                   EGL_HEIGHT, &pstEGL->s32SurfaceH));
    Cprintf_yellow( "surface: WxH:[%d x %d]\n", pstEGL->s32SurfaceW, pstEGL->s32SurfaceH);

    // Create EGL context
    pstEGL->eglContext = 
    	eglCreateContext(pstEGL->eglDisplay, pvConfig, EGL_NO_CONTEXT, as32ContextAttrs);
	BASE_CHECK_TRUE_RET( EGL_NO_CONTEXT == pstEGL->eglContext, -1);

    // 绑定 context
    EGL_EXECUTE_CHECK_RET(eglMakeCurrent(pstEGL->eglDisplay, pstEGL->eglSurface,
    						 pstEGL->eglSurface, pstEGL->eglContext));

    return OK;
}


/**
 * @function:   GL_CreateShader
 * @brief:      创建着色器
 * @param[in]:  uint32_t u32Type      
 * @param[in]:  const GLchar *ps8Src  
 * @param[out]: None
 * @return:     static uint32_t
 */
static uint32_t GL_CreateShader(uint32_t u32Type, const GLchar *ps8Src)
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
		Cprintf_red("[%s %d]  [0x%x]:\n%s\n", __FUNCTION__, __LINE__, u32Type, s8ErrInfo);
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
static uint32_t GL_CreateProgram(const GLchar *ps8VertSrc, const GLchar *ps8FragSrc)
{
    uint32_t u32VertShader = 0;
    uint32_t u32FragShader = 0;
    uint32_t u32Program = 0;
    int32_t s32Status = 0;

    u32VertShader = GL_CreateShader(GL_VERTEX_SHADER, ps8VertSrc);
 	BASE_CHECK_TRUE_RET(0 == u32VertShader, -1);

    u32FragShader = GL_CreateShader(GL_FRAGMENT_SHADER, ps8FragSrc);
 	BASE_CHECK_TRUE_RET(0 == u32FragShader, -1);

    u32Program = glCreateProgram();
 	BASE_CHECK_TRUE_RET(0 == u32FragShader, -1);

    glAttachShader(u32Program, u32VertShader);
    glAttachShader(u32Program, u32FragShader);
    glLinkProgram(u32Program);
    glGetProgramiv(u32Program, GL_LINK_STATUS, &s32Status);
    if(s32Status != GL_TRUE)
    {
        GLchar s8ErrInfo[1024];
        glGetProgramInfoLog(u32Program, sizeof(s8ErrInfo), NULL, s8ErrInfo);
		Cprintf_red("[%s %d] ERR @ link. \n\t %s\n", __FUNCTION__, __LINE__, s8ErrInfo);
        return -2;
    }

    GL_EXECUTE_CHECK_RET(glDeleteShader(u32VertShader));
    GL_EXECUTE_CHECK_RET(glDeleteShader(u32FragShader));

    Cprintf_green( "GLSL link success\n");

    return u32Program;
}


/**
 * @function:   Draw
 * @brief:      绘制
 * @param[in]:  EGL_Context *esContext  
 * @param[out]: None
 * @return:     void
 */
void Draw(EGL_Context *esContext)
{
	GLfloat vVertices[] = {  
						0.0f,  0.5f, 0.0f,
	                    -0.5f, -0.5f, 0.0f,
	                    0.5f, -0.5f, 0.0f  };

	// Use the program object
	glUseProgram ( esContext->u32GLSLProgram );

	// Set the viewport
	glViewport ( 0, 0, esContext->s32SurfaceW , esContext->s32SurfaceH );

	// Clear the color buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1);
	glClear ( GL_COLOR_BUFFER_BIT );

	// Load the vertex data
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
	glEnableVertexAttribArray ( 0 );

	glDrawArrays ( GL_TRIANGLES, 0, 3 );

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	Cprintf_green("[%s %d]	disp triangle\n", __FUNCTION__, __LINE__);

	return;
}


int main(int argc, char *argv[])
{
	win_SetupConsole();

    mk_Build_Date(NULL);

	EGL_Context stEglInfo = {0};
	stEglInfo.s32SurfaceW = 640;
	stEglInfo.s32SurfaceH = 480;

	win_CreateWindow(&stEglInfo, "testWin");
	
	setupEGL(&stEglInfo);
	stEglInfo.u32GLSLProgram = GL_CreateProgram(vShaderStr, fShaderStr);
//	Draw(&stEglInfo);

	stEglInfo.drawFunc = Draw;
	stEglInfo.shutdownFunc = NULL;

	win_WinLoop(&stEglInfo);

	win_RestoreConsole();
	system("pause");

    return OK;
}

