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
#define USE_EBO                     /* 使用 BO,VBO,VAO,EBO */     
//#define USE_TRANGLES              /* 不使用 BO 的 6 顶点, 每次绘制都需要传递顶点 */
//#define USE_TRANGLES_STRIP        /* 不使用 BO 的 4 顶点, 每次绘制都需要传递顶点 */

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/
uint32_t VAO = 0, VBO = 0, EBO = 0;

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
   "out vec4 fragColor; 						 \n"
   "void main() 								 \n"
   "{											 \n"
   "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
   "}											 \n";


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
        GL_EXECUTE_CHECK_RET(glViewport( 0, 0, esContext->s32NewSurfaceW,
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
    EGL_EXECUTE_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
    						 esContext->eglSurface, esContext->eglContext));

    /* Use the program object */
	GL_EXECUTE_CHECK_RET(glUseProgram (esContext->u32GLSLProgram));

#ifdef USE_EBO
    Cprintf_reverse("[%s %d]  USE_EBO\n", __func__, __LINE__);

    float vVertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f,   // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3, // 第二个三角形
    };

    GL_EXECUTE_CHECK_RET(glGenVertexArraysOES(1, &VAO));
    GL_EXECUTE_CHECK_RET(glGenBuffers(1, &VBO));   
    GL_EXECUTE_CHECK_RET(glGenBuffers(1, &EBO));
    
    /* bind the Vertex Array Object first, 
        then bind and set vertex buffer(s), 
            and then configure vertex attributes(s). */
    GL_EXECUTE_CHECK_RET(glBindVertexArrayOES(VAO));
    GL_EXECUTE_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, VBO));   
    GL_EXECUTE_CHECK_RET(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

    GL_EXECUTE_CHECK_RET(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
    GL_EXECUTE_CHECK_RET(glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW));
	GL_EXECUTE_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GL_EXECUTE_CHECK_RET(glEnableVertexAttribArray(0));

    /* note that this is allowed, the call to glVertexAttribPointer 
        registered VBO as the vertex attribute's bound vertex buffer object,
            so afterwards we can safely unbind */
    GL_EXECUTE_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_EXECUTE_CHECK_RET(glBindVertexArrayOES(0));

#elif (defined USE_TRANGLES)

    Cprintf_reverse("[%s %d]  USE_TRANGLES\n", __func__, __LINE__);

#elif (defined USE_TRANGLES_STRIP)

    Cprintf_reverse("[%s %d]  USE_TRANGLES_STRIP\n", __func__, __LINE__);

#endif

    Cprintf_green("[%s %d]  \n", __func__, __LINE__);

	return OK;
}

int32_t Draw(EGL_Context *esContext)
{
    /* 绑定 Context 至当前线程 */
    EGL_EXECUTE_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
    						 esContext->eglSurface, esContext->eglContext));

    /* 重配窗口大小 */
    resizeSurface(esContext);

	// Clear the color buffer
	GL_EXECUTE_CHECK_RET(glClearColor(0.0f, 1.0f, 0.0f, 1));
	GL_EXECUTE_CHECK_RET(glClear(GL_COLOR_BUFFER_BIT));

#ifdef USE_EBO	

    GL_EXECUTE_CHECK_RET(glBindVertexArrayOES(VAO));
    GL_EXECUTE_CHECK_RET(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    
#elif (defined USE_TRANGLES)
    /* Load the vertex data */
    float vVertices[] = {
        // 第一个三角形
        0.5f,  0.5f, 0.0f,  // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, 0.5f, 0.0f,  // 左上角
        // 第二个三角形
         0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f,  // 左下角
        -0.5f,  0.5f, 0.0f,  // 左上角
    };

    GL_EXECUTE_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices));
	GL_EXECUTE_CHECK_RET(glEnableVertexAttribArray(0));	
	GL_EXECUTE_CHECK_RET(glDrawArrays( GL_TRIANGLES, 0, 6));
	
#elif (defined USE_TRANGLES_STRIP)

    float vVertices[] = {    
        -0.5f, 0.5f, 0.0f,   // 左上角        
        -0.5f, -0.5f, 0.0f, // 左下角
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
    };
    
    GL_EXECUTE_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices));
    GL_EXECUTE_CHECK_RET(glEnableVertexAttribArray(0)); 
    GL_EXECUTE_CHECK_RET(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

#endif	

    /* swap 2 disp */
	EGL_EXECUTE_CHECK_RET(eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface));

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

    GL_EXECUTE_CHECK_RET(glDeleteProgram(esContext->u32GLSLProgram));

    if(VAO)
    {
        GL_EXECUTE_CHECK_RET(glDeleteVertexArraysOES(1, &VAO));
    }

    if(EBO)
    {
        GL_EXECUTE_CHECK_RET(glDeleteBuffers(1, &EBO));
    }


    GL_EXECUTE_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, EGL_NO_SURFACE,
                                        EGL_NO_SURFACE, EGL_NO_CONTEXT));

    GL_EXECUTE_CHECK_RET(eglDestroyContext(esContext->eglDisplay, esContext->eglContext));

    GL_EXECUTE_CHECK_RET(eglDestroySurface(esContext->eglDisplay, esContext->eglSurface));

    GL_EXECUTE_CHECK_RET(eglTerminate(esContext->eglDisplay));

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


