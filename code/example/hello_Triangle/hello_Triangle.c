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


/**
 * @function:   Draw
 * @brief:      绘制
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     void
 */
int32_t Draw(EGL_Context *esContext)
{
    /* 绑定 Context 至当前线程 */
    EGL_RUN_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
    						 esContext->eglSurface, esContext->eglContext));

    /* 重配窗口大小 */
    if(esContext->bBeReSizeSurface)
    {
        GL_RUN_CHECK_RET(glViewport( 0, 0, esContext->s32NewSurfaceW,
                                            esContext->s32NewSurfaceH));
        Cprintf_yellow("[%s %d]  original:[%d x %d] now:[%d x %d]\n",__func__, __LINE__,
            esContext->s32SurfaceW, esContext->s32SurfaceH,
            esContext->s32NewSurfaceW, esContext->s32NewSurfaceH);

        esContext->s32SurfaceW = esContext->s32NewSurfaceW;
        esContext->s32SurfaceH = esContext->s32NewSurfaceH;
        esContext->bBeReSizeSurface = FALSE;
    }

    /* Use the program object */
	GL_RUN_CHECK_RET(glUseProgram (esContext->u32GLSLProgram));

	// Clear the color buffer
	GL_RUN_CHECK_RET(glClearColor(0.0f, 1.0f, 0.0f, 1));
	GL_RUN_CHECK_RET(glClear(GL_COLOR_BUFFER_BIT));

    /* Load the vertex data */
	GLfloat vVertices[] = { 0.0f,   0.5f, 0.0f,
    	                    -0.5f, -0.5f, 0.0f,
	                        0.5f,  -0.5f, 0.0f  };
	GL_RUN_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices));
	GL_RUN_CHECK_RET(glEnableVertexAttribArray(0));

    /* draw */
	GL_RUN_CHECK_RET(glDrawArrays( GL_TRIANGLES, 0, 3));

    /* swap 2 disp */
	EGL_RUN_CHECK_RET(eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface));

    CalcFpsInfo(esContext);

	return OK;
}

/**
 * @function:   deInit
 * @brief:      去初始化: 资源销毁
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     int32_t
 */
int32_t deInit(EGL_Context *esContext)
{
    Cprintf_yellow("[%s %d]  \n", __func__, __LINE__);

    GL_RUN_CHECK_RET(glDeleteProgram(esContext->u32GLSLProgram));

    GL_RUN_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, EGL_NO_SURFACE,
                                        EGL_NO_SURFACE, EGL_NO_CONTEXT));

    GL_RUN_CHECK_RET(eglDestroyContext(esContext->eglDisplay, esContext->eglContext));

    GL_RUN_CHECK_RET(eglDestroySurface(esContext->eglDisplay, esContext->eglSurface));

    GL_RUN_CHECK_RET(eglTerminate(esContext->eglDisplay));

    return OK;
}


int main(int argc, char *argv[])
{
	win_SetupConsole();

    mk_Build_Date(NULL);

	EGL_Context stEglInfo = {0};
	stEglInfo.s32SurfaceW = 640;
	stEglInfo.s32SurfaceH = 480;
	stEglInfo.s32NewSurfaceW = stEglInfo.s32SurfaceW;
	stEglInfo.s32NewSurfaceH = stEglInfo.s32SurfaceH;
	stEglInfo.bBeReSizeSurface = TRUE;

	win_CreateWindow(&stEglInfo, "testWin");

	GL_SetupEGL(&stEglInfo);
	stEglInfo.u32GLSLProgram = GL_CreateProgram(vShaderStr, fShaderStr);
 	BASE_CHECK_TRUE_RET(0 == stEglInfo.u32GLSLProgram, -1);

	stEglInfo.drawFunc = Draw;
	stEglInfo.shutdownFunc = NULL;

	win_WinLoop(&stEglInfo);

	win_RestoreConsole();
    system("pause");

    return OK;
}

