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
#include "my_ResourceLoader.h"

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
uint32_t VBO = 0, VAO = 0, EBO = 0;
uint32_t u32Texture = 0;

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
 * @function:   _texture
 * @brief:      纹理配置
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     static int32_t
 */
static int32_t _ConfTexture(EGL_Context *esContext)
{
    /* 纹理顶点属性绑定至 VAO */
    GL_RUN_CHECK_RET(glBindVertexArray(VAO));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_RUN_CHECK_RET(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(2));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, 0)); /* 解绑 */
    GL_RUN_CHECK_RET(glBindVertexArray(0));

  /****************************** 纹理的配置 *******************************/
    uint32_t u32TextureId = 0;
    /* 指定 GLSL 片段着色器中的 uniform 纹理值 */
    GLint location = glGetUniformLocation(esContext->u32GLSLProgram, "texture_1");
    BASE_CHECK_TRUE_RET(location < 0, -2);
    GL_RUN_CHECK_RET(glUniform1i(location, u32TextureId));
    
    /* 生成并配置纹理 */
    GL_RUN_CHECK_RET(glGenTextures(1, &u32Texture));
    GL_RUN_CHECK_RET(glActiveTexture(GL_TEXTURE0 + u32TextureId));
    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, u32Texture));
    // 为当前绑定的纹理对象设置环绕、过滤方式
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    int32_t s32Ret = OK;
    ImageInfo stImage = {0};
    snprintf((char *)stImage.ps8FileName, sizeof(stImage.ps8FileName), "./resources/textures/container.jpg");
    s32Ret = imageLoad(&stImage);
    BASE_CHECK_TRUE_RET(OK != s32Ret, -1);

    GL_RUN_CHECK_RET(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, stImage.s32Width, stImage.s32Height,
                                    0, GL_RGB, GL_UNSIGNED_BYTE, stImage.pu8Data));
//    GL_RUN_CHECK_RET(glGenerateMipmap(GL_TEXTURE_2D));    /* 可省略 */

    s32Ret = imageFree(&stImage);
    BASE_CHECK_TRUE_WARN(OK != s32Ret);
    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, 0));  /* unbind texture */

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

    float vVertices[] = {
   //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
       0, 1, 3, // 第一个三角形
       1, 2, 3, // 第二个三角形
    };

    GL_RUN_CHECK_RET(glGenVertexArrays(1, &VAO));
    GL_RUN_CHECK_RET(glGenBuffers(1, &VBO));
    GL_RUN_CHECK_RET(glGenBuffers(1, &EBO));

    GL_RUN_CHECK_RET(glBindVertexArray(VAO));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));

    GL_RUN_CHECK_RET(glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW));
    GL_RUN_CHECK_RET(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_RUN_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(0));
    GL_RUN_CHECK_RET(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(1));

    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, 0)); /* 解绑 */
    GL_RUN_CHECK_RET(glBindVertexArray(0));

    Cprintf_green("[%s %d]  \n", __func__, __LINE__);

    _ConfTexture(esContext);

	return OK;
}



/**
 * @function:   Draw
 * @brief:      绘制函数
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     int32_t
 */
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

    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, u32Texture));
    GL_RUN_CHECK_RET(glBindVertexArrayOES(VAO));
    GL_RUN_CHECK_RET(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

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
	stEglInfo.u32GLSLProgram =
	    GL_CreateProgramFromFile("./example/textureCombined/vertextShader.glsl",
	                             "./example/textureCombined/fragementShder.glsl");
 	BASE_CHECK_TRUE_RET(0 == stEglInfo.u32GLSLProgram, -1);

	stEglInfo.drawFunc = Draw;
	stEglInfo.shutdownFunc = NULL;

    beforeDraw(&stEglInfo);

	win_WinLoop(&stEglInfo);

	win_RestoreConsole();
    system("pause");

    return OK;
}



