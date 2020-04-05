/**
 * @file:   win32.h
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  win32.c 的头文件
 * @author:  
 * @date:   2020/3/29
 * @note:
 * @note \n History:
   1.日    期: 2020/3/29
     作    者:  
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include "OGL_common.h"

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
#ifndef __WIN32_OGLES_H__
#define __WIN32_OGLES_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern void win_SetupConsole(void);
extern void win_RestoreConsole(void);

extern void win_WinLoop(EGL_Context *esContext);
extern GLboolean win_CreateWindow(EGL_Context *esContext, const char *title);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __WIN32_H__ */
