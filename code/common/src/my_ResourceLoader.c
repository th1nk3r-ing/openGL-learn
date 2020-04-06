/**
 * @file:   my_ResourceLoader.c
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  OGL 资源文件加载
 * @author: think3r
 * @date:   2020/4/6
 * @note:
 * @note \n History:
   1.日    期: 2020/4/6
     作    者: think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include "my_ResourceLoader.h"

/* 使用   stb 单头文件, 加载图片 */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/
/**
 * @function:   imageLoad
 * @brief:      加载图像(自带内存申请)
 * @param[in]:  ImageInfo * pstInfo
 * @param[out]: None
 * @return:     int32_t
 */
int32_t imageLoad(ImageInfo * pstInfo)
{
    BASE_CHECK_TRUE_RET(NULL == pstInfo, -1);
    BASE_CHECK_TRUE_RET(0 == strlen((const char *)pstInfo->ps8FileName), -1);

    if(pstInfo->bBeFlipVertical)
    {
        stbi_set_flip_vertically_on_load(TRUE);
    }

    pstInfo->pu8Data = stbi_load((char const *)pstInfo->ps8FileName, 
                                &pstInfo->s32Width, 
                                &pstInfo->s32Height,
                                &pstInfo->s32Channels, 
                                0);
    if (pstInfo->pu8Data)
    {
        Cprintf_yellow("[%s %d]  load image:[%s] OK! [%d x %d] @ %d Chans\n",
            __func__, __LINE__, pstInfo->ps8FileName, pstInfo->s32Width, pstInfo->s32Height, pstInfo->s32Channels);
        pstInfo->bBeVaild = TRUE;
        return OK;
    }
    else
    {
        Cprintf_red("[%s %d] ERR! Failed to load texture :[%s]\n", __func__, __LINE__, pstInfo->ps8FileName);
        return -2;
    }
}

/**
 * @function:   imageFree
 * @brief:      释放图像内存
 * @param[in]:  ImageInfo * pstInfo
 * @param[out]: None
 * @return:     int32_t
 */
int32_t imageFree(ImageInfo * pstInfo)
{
    BASE_CHECK_TRUE_RET(NULL == pstInfo, -1);

    if((pstInfo->bBeVaild) && (NULL != pstInfo->pu8Data))
    {
        stbi_image_free(pstInfo->pu8Data);
        Cprintf_yellow("[%s %d]  release OK\n", __func__, __LINE__);
        return OK;
    }
    else
    {
        Cprintf_red("[%s %d] ERR! beVaild:[%d], pData:[%p] \n",
                __func__, __LINE__, pstInfo->bBeVaild, pstInfo->pu8Data);
        return -2;
    }
}


