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
#include "my_TransFormations.h"

/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/
#define MULTIPLE_CUBE
/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/
uint32_t u32g_StartTime = 0;    /* 启动时间 */
uint32_t VBO = 0, VAO = 0;
TEXTURE_INFO stTexture1 = {0}, stTexture2 = {0};
CoorSysInfo stCoorSysInfo = {0};

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

static int32_t _ConfTexture(TEXTURE_INFO *pstTexture, ImageInfo *pstImage)
{
    BASE_CHECK_TRUE_RET(NULL == pstTexture, -1);
    BASE_CHECK_TRUE_RET(NULL == pstImage, -1);

    /* 指定 GLSL 片段着色器中的 uniform 纹理值 */
    GLint location = glGetUniformLocation(pstTexture->u32GLSLProgram,
                                          (const GLchar *)pstTexture->ps8TextureName);
    BASE_CHECK_TRUE_RET(location < 0, -2);
    GL_RUN_CHECK_RET(glUniform1i(location, pstTexture->u32TextureId));

    /* 生成并配置纹理 */
    GL_RUN_CHECK_RET(glGenTextures(1, &pstTexture->u32Texture));
    GL_RUN_CHECK_RET(glActiveTexture(GL_TEXTURE0 + pstTexture->u32TextureId));
    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, pstTexture->u32Texture));
    // 为当前绑定的纹理对象设置环绕、过滤方式
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, pstTexture->s32WarpMode));
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, pstTexture->s32WarpMode));
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pstTexture->s32FilterMode));
    GL_RUN_CHECK_RET(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pstTexture->s32FilterMode));

    int32_t s32Ret = OK;
    s32Ret = imageLoad(pstImage);
    BASE_CHECK_TRUE_RET(OK != s32Ret, -1);

    if(pstImage->s32Channels == 3)
    {
        GL_RUN_CHECK_RET(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pstImage->s32Width, pstImage->s32Height,
                                        0, GL_RGB, GL_UNSIGNED_BYTE, pstImage->pu8Data));
    }
    else if(pstImage->s32Channels == 4)
    {
        GL_RUN_CHECK_RET(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pstImage->s32Width, pstImage->s32Height,
                                0, GL_RGBA, GL_UNSIGNED_BYTE, pstImage->pu8Data));
    }

    if(pstTexture->bBeGenMipMap)
    {
        GL_RUN_CHECK_RET(glGenerateMipmap(GL_TEXTURE_2D));    /* 可省略 */
    }

    s32Ret = imageFree(pstImage);
    BASE_CHECK_TRUE_WARN(OK != s32Ret);

    /* unbind texture */
    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, 0));

    return OK;
}


/**
 * @function:   _ConfTextureParam
 * @brief:      纹理配置
 * @param[in]:  EGL_Context *esContext
 * @param[out]: None
 * @return:     static int32_t
 */
static int32_t _ConfTextureParam(EGL_Context *esContext)
{
    /* 纹理顶点属性绑定至 VAO */
    GL_RUN_CHECK_RET(glBindVertexArray(VAO));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_RUN_CHECK_RET(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(1));
    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, 0)); /* 解绑 */
    GL_RUN_CHECK_RET(glBindVertexArray(0));

  /****************************** 纹理的配置 *******************************/
    ImageInfo stImage = {0};

    stTexture1.u32TextureId = 0;
    snprintf((char *)stTexture1.ps8TextureName, sizeof(stTexture1.ps8TextureName), "texture_0");
    stTexture1.u32GLSLProgram = esContext->u32GLSLProgram;
    stTexture1.s32WarpMode = GL_REPEAT;
    stTexture1.s32FilterMode = GL_LINEAR;
    stTexture1.bBeGenMipMap = true;
    memset(&stImage, 0x00, sizeof(stImage));
    snprintf((char *)stImage.ps8FileName, sizeof(stImage.ps8FileName), "./resources/textures/container.jpg");
    _ConfTexture(&stTexture1, &stImage);

    stTexture2.u32TextureId = 1;
    snprintf((char *)stTexture2.ps8TextureName, sizeof(stTexture2.ps8TextureName), "texture_1");
    stTexture2.u32GLSLProgram = esContext->u32GLSLProgram;
    stTexture2.s32WarpMode = GL_REPEAT;
    stTexture2.s32FilterMode = GL_LINEAR;
    stTexture2.bBeGenMipMap = true;
    memset(&stImage, 0x00, sizeof(stImage));
    stImage.bBeFlipVertical = true;
    snprintf((char *)stImage.ps8FileName, sizeof(stImage.ps8FileName), "./resources/textures/awesomeface.png");
    _ConfTexture(&stTexture2, &stImage);

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
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };



    /* global stat */
    GL_RUN_CHECK_RET(glEnable(GL_DEPTH_TEST));

    GL_RUN_CHECK_RET(glGenVertexArrays(1, &VAO));
    GL_RUN_CHECK_RET(glGenBuffers(1, &VBO));

    GL_RUN_CHECK_RET(glBindVertexArray(VAO));

    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_RUN_CHECK_RET(glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW));

    GL_RUN_CHECK_RET(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_RUN_CHECK_RET(glEnableVertexAttribArray(0));


    GL_RUN_CHECK_RET(glBindBuffer(GL_ARRAY_BUFFER, 0)); /* 解绑 */
    GL_RUN_CHECK_RET(glBindVertexArray(0));

    Cprintf_green("[%s %d]  \n", __func__, __LINE__);

    _ConfTextureParam(esContext);

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
    int32_t s32Ret = OK;

    /* 绑定 Context 至当前线程 */
    EGL_RUN_CHECK_RET(eglMakeCurrent(esContext->eglDisplay, esContext->eglSurface,
    						 esContext->eglSurface, esContext->eglContext));

    /* 重配窗口大小 */
    s32Ret = resizeSurface(esContext);
    BASE_CHECK_TRUE_RET(OK != s32Ret, -2);

	// Clear the color buffer
	GL_RUN_CHECK_RET(glClearColor(0.0f, 0.0f, 0.0f, 1));
	GL_RUN_CHECK_RET(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GL_RUN_CHECK_RET(glUseProgram(esContext->u32GLSLProgram));

    GL_RUN_CHECK_RET(glActiveTexture(GL_TEXTURE0 + stTexture1.u32TextureId));
    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, stTexture1.u32Texture));

    GL_RUN_CHECK_RET(glActiveTexture(GL_TEXTURE0 + stTexture2.u32TextureId));
    GL_RUN_CHECK_RET(glBindTexture(GL_TEXTURE_2D, stTexture2.u32Texture));

    GL_RUN_CHECK_RET(glBindVertexArrayOES(VAO));

    stCoorSysInfo.s32CurSurfaceW = esContext->s32SurfaceW;
    stCoorSysInfo.s32CurSurfaceH = esContext->s32SurfaceH;

    s32Ret = coordinateSystem_getMat(&stCoorSysInfo, (getTime_ms() - u32g_StartTime));
    BASE_CHECK_TRUE_RET(OK != s32Ret, -2);

#ifndef MULTIPLE_CUBE

    GL_RUN_CHECK_RET(glUniformMatrix4fv(stCoorSysInfo.s32GLSLModelLoc, 1,
                                        GL_FALSE, stCoorSysInfo.pfModelMat));
    GL_RUN_CHECK_RET(glUniformMatrix4fv(stCoorSysInfo.s32GLSLViewLoc, 1,
                                        GL_FALSE, stCoorSysInfo.pfViewMat));
    GL_RUN_CHECK_RET(glUniformMatrix4fv(stCoorSysInfo.s32GLSLProjectionLoc, 1,
                                        GL_FALSE, stCoorSysInfo.pfProjectionMat));

    GL_RUN_CHECK_RET(glDrawArrays(GL_TRIANGLES, 0, 36));
#else
    GL_RUN_CHECK_RET(glUniformMatrix4fv(stCoorSysInfo.s32GLSLViewLoc, 1,
                                        GL_FALSE, stCoorSysInfo.pfViewMat));
    GL_RUN_CHECK_RET(glUniformMatrix4fv(stCoorSysInfo.s32GLSLProjectionLoc, 1,
                                        GL_FALSE, stCoorSysInfo.pfProjectionMat));

    for(uint32_t u32Idx = 0; u32Idx < 10; u32Idx++)
    {
        s32Ret = coordinateSystemMuliCube_getMat(&stCoorSysInfo, u32Idx);
        BASE_CHECK_TRUE_RET(OK != s32Ret, -2);
        GL_RUN_CHECK_RET(glUniformMatrix4fv(stCoorSysInfo.s32GLSLModelLoc, 1,
                                            GL_FALSE, stCoorSysInfo.pfModelMat));
        GL_RUN_CHECK_RET(glDrawArrays(GL_TRIANGLES, 0, 36));                                            
    }
#endif


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

    u32g_StartTime = getTime_ms();

	EGL_Context stEglInfo = {0};
	stEglInfo.s32SurfaceW = 640;
	stEglInfo.s32SurfaceH = 480;
	stEglInfo.s32NewSurfaceW = stEglInfo.s32SurfaceW;
	stEglInfo.s32NewSurfaceH = stEglInfo.s32SurfaceH;
	stEglInfo.bBeReSizeSurface = TRUE;

	win_CreateWindow(&stEglInfo, __FILE__);

	GL_SetupEGL(&stEglInfo);
	stEglInfo.u32GLSLProgram =
	    GL_CreateProgramFromFile("./example/coordinateSystem1/vertextShader.glsl",
	                             "./example/coordinateSystem1/fragementShder.glsl");
 	BASE_CHECK_TRUE_RET(0 == stEglInfo.u32GLSLProgram, -2);

    stCoorSysInfo.s32CurSurfaceW = stEglInfo.s32SurfaceW;
    stCoorSysInfo.s32CurSurfaceH = stEglInfo.s32SurfaceH;

    stCoorSysInfo.s32GLSLModelLoc = glGetUniformLocation(stEglInfo.u32GLSLProgram, "model");
    BASE_CHECK_TRUE_RET(stCoorSysInfo.s32GLSLModelLoc < 0, -2);
    stCoorSysInfo.s32GLSLViewLoc = glGetUniformLocation(stEglInfo.u32GLSLProgram, "view");
    BASE_CHECK_TRUE_RET(stCoorSysInfo.s32GLSLViewLoc < 0, -2);
    stCoorSysInfo.s32GLSLProjectionLoc = glGetUniformLocation(stEglInfo.u32GLSLProgram, "projection");
    BASE_CHECK_TRUE_RET(stCoorSysInfo.s32GLSLProjectionLoc < 0, -2);

	stEglInfo.drawFunc = Draw;
	stEglInfo.shutdownFunc = NULL;

    beforeDraw(&stEglInfo);

	win_WinLoop(&stEglInfo);

	win_RestoreConsole();
    system("pause");

    return OK;
}



