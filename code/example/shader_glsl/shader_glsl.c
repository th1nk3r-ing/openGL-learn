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
#include <math.h>

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/
//#define TWO_VERTEX_ATTRI_POINTER             /* 单个 VBO   -> 两个顶点属性 */
#define UINFORM_TEST                         /* 使用 uniform 传递顶点颜色 */
/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/
uint32_t VBO = 0, VAO = 0;


#ifdef TWO_VERTEX_ATTRI_POINTER
char vShaderStr[] =
   "#version 300 es 						 \n"
   "layout(location = 0) in vec3 vPosition;  \n"
   "layout(location = 1) in vec3 vColor;     \n"
   "out vec3  outColor;                      \n"
   "void main() 							 \n"
   "{										 \n"
   "   outColor = vColor;                    \n"
   "   gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0); 	 \n"
   "}										 \n";

char fShaderStr[] =
   "#version 300 es 							 \n"
   "precision mediump float;					 \n"
   "in vec3  outColor;                           \n"
   "out vec4 fragColor; 						 \n"
   "void main() 								 \n"
   "{											 \n"
   "   fragColor = vec4 (outColor, 1.0 );  \n"
   "}											 \n";
#elif (defined UINFORM_TEST)

char vShaderStr[] =
   "#version 300 es 						 \n"
   "layout(location = 0) in vec3 vPosition;  \n"
   "void main() 							 \n"
   "{										 \n"
   "   gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0); 	 \n"
   "}										 \n";

char fShaderStr[] =
   "#version 300 es 							 \n"
   "precision mediump float;					 \n"
   "uniform vec3 ourColor;                      \n"
   "out vec4 fragColor; 						 \n"
   "void main() 								 \n"
   "{											 \n"
   "   fragColor = vec4(ourColor, 1.0 );  \n"
   "}											 \n";

#endif
/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

/**
 * @function:   resizeSurface
 * @brief:      重新配置显示窗口
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     static void
 */
static int32_t resizeSurface(EGL_Context *esContext)
{
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

    return OK;
}


/**
 * @function:   Draw
 * @brief:      绘制
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     void
 */
int32_t beforeDraw(EGL_Context *esContext)
{
    /* 绑定 Context 至当前线程 */
    EGL_RUN_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
    						 esContext->eglSurface, esContext->eglContext));

    /* Use the program object */
	GL_RUN_CHECK_RET(glUseProgram (esContext->u32GLSLProgram));
    GL_RUN_CHECK_RET(glLineWidth(3));

    Cprintf_reverse("[%s %d]  ONE_VBO\n", __func__, __LINE__);

    float vVertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    GL_RUN_CHECK_RET(glGenVertexArrays(1, &VAO));
    GL_RUN_CHECK_RET(glGenBuffers(1, &VBO));

    GL_RUN_CHECK_RET(glBindVertexArray(VAO));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, VBO));       
    /* 一个 VBO ,固仅需要 buferData 一次 */
    GL_RUN_CHECK_RET(glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW));
    GL_RUN_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(0));

#ifdef TWO_VERTEX_ATTRI_POINTER    
    GL_RUN_CHECK_RET(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                                                    (void*)(3* sizeof(float))));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(1));    
#endif

    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, 0)); /* 解绑 */
    GL_RUN_CHECK_RET(glBindVertexArrayOES(0));

    Cprintf_green("[%s %d]  \n", __func__, __LINE__);

	return OK;
}

int32_t Draw(EGL_Context *esContext)
{
    /* 绑定 Context 至当前线程 */
    EGL_RUN_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
    						 esContext->eglSurface, esContext->eglContext));

    /* 重配窗口大小 */
    resizeSurface(esContext);

	// Clear the color buffer
	GL_RUN_CHECK_RET(glClearColor(0.0f, 0.0f, 0.0f, 1));
	GL_RUN_CHECK_RET(glClear(GL_COLOR_BUFFER_BIT));

    GL_RUN_CHECK_RET(glUseProgram(esContext->u32GLSLProgram));
    GL_RUN_CHECK_RET(glBindVertexArrayOES(VAO));

#ifdef UINFORM_TEST
    double timeValue = (double)getTime_ms() / 1000.0;
    float greenValue = (float)sin(timeValue) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(esContext->u32GLSLProgram, "ourColor");
    glUniform3f(vertexColorLocation, 1 - greenValue, greenValue, 0.0f);
#endif

    GL_RUN_CHECK_RET(glDrawArrays(GL_TRIANGLES, 0, 3));

    /* swap to disp */
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

    if(VAO)
    {
        GL_RUN_CHECK_RET(glDeleteVertexArrays(1, &VAO));
    }

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

    beforeDraw(&stEglInfo);

	win_WinLoop(&stEglInfo);

	win_RestoreConsole();
    system("pause");

    return OK;
}



