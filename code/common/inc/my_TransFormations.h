/**
 * @file:   my_TransFormations.h
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  my_TransFormations.cpp 的头文件
 * @author: think3r
 * @date:   2020/4/7
 * @note:
 * @note \n History:
   1.日    期: 2020/4/7
     作    者:  think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/

#ifndef __MY_TRANSFORMATIONS_H__
#define __MY_TRANSFORMATIONS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
/*----------------------------------------------*/
/*                 宏类型定义                   */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                结构体定义                    */
/*----------------------------------------------*/
typedef struct _coordinate_Systems_Info_
{
    int32_t s32CurSurfaceW;
    int32_t s32CurSurfaceH;

    int32_t s32GLSLModelLoc;    
    float * pfModelMat;             /* 模型位置矩阵 */
    
    int32_t s32GLSLViewLoc;
    float * pfViewMat;              /* 观察矩阵 */

    int32_t s32GLSLProjectionLoc;
    float * pfProjectionMat;        /* 投影矩阵 */
}CoorSysInfo;

/*----------------------------------------------*/
/*                 函数声明                     */
/*----------------------------------------------*/
float * transFormations_get1(uint32_t u32NowTime);
float * transFormations_get2(uint32_t u32NowTime);
int32_t coordinateSystem_getMat(CoorSysInfo * pstInfo, uint32_t u32NowTime);
int32_t coordinateSystemMuliCube_getMat(CoorSysInfo * pstInfo, uint32_t u32Idx);

void * Camera_creatHandle();
void Camera_deleteHandle( void * pHandle);
void Camera_processKey(void *pHandle, CameraKey eKey);
void Camera_processMouse(void * pHandle, float fXOffset, float fYOffset);
void Camera_scrollMouse(void * pHandle, float yoffset, float fWidth, float fHeight);
float * Camera_getViewMatrix(void * pHandle);
float * Camera_getProjectionMatrix(void * pHandle);

/*----------------------------------------------*/
/*                 全局变量                     */
/*----------------------------------------------*/

/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __MY_TRANSFORMATIONS_H__ */
