/**
 * @file:   my_ResourceLoader.h
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  my_ResourceLoader.c 的头文件
 * @author: thiink3r
 * @date:   2020/4/6
 * @note:
 * @note \n History:
   1.日    期: 2020/4/6
     作    者:  think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include "common.h"

#ifndef __MY_RESOURCELOADER_H__
#define __MY_RESOURCELOADER_H__
/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/
typedef enum _IMAGE_FORMAT_
{
    IMAGE_ERR_YPTE = 0,
    
    YUV_420_I420 = 0x01,    /* [YYYYYYYY] [UU] [VV] */
    YUV_420_YV12 = 0x02,    /* [YYYYYYYY] [VV] [UU] */
    YUV_420_NV12 = 0x03,    /* [YYYYYYYY] [UVUV]    */
    YUV_420_NV21 = 0x04,    /* [YYYYYYYY] [VUVU]    */

    YUV_422_P422 = 0x11,    /* [YYYY] [UU] [VV] */
    YUV_422_S422 = 0x12,    /* [YYYY] [UVUV]    */
    YUV_422_YUYV = 0x13,    /* [UYVY UYVY]      */
    YUV_422_UYVY = 0x14,    /* [YUYV YUYV]      */

    RGB_888      = 0x20,    
    RGBA_8888    = 0x30,

    IMAGE_FORMAT_BUTT
}IMAGE_FORMAT;

typedef struct _STB_IMAGE_INFO_
{
    /* 配置的参数 */
    int8_t  ps8FileName[128];
    bool    bBeFlipVertical;

    int32_t s32Width;
    int32_t s32Height;
    int32_t s32Channels;    /* s32Channels表示通道数，RGBA 一共 4 个通道，有些图片只有 3 个 */
    IMAGE_FORMAT eImageFormat;
    
    uint8_t *pu8Data;
    bool    bBeVaild;
}ImageInfo;

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern int32_t imageLoad(ImageInfo * pstInfo);
extern int32_t imageFree(ImageInfo * pstInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MY_RESOURCELOADER_H__ */
