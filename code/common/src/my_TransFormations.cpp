/**
 * @file:   my_TransFormations.cpp
 * @note:   2010-2020, <git.oschia.net/think3r>
 * @brief:  glm 变换的封装
 * @author:
 * @date:   2020/4/7
 * @note:
 * @note \n History:
   1.日    期: 2020/4/7
     作    者: think3r
     修改历史: 创建文件
 */

/*----------------------------------------------*/
/*                  头文件包含                  */
/*----------------------------------------------*/
#include "common.h"
#include "build_time.h"
#include "my_TransFormations.h"
//#include "glm/glm.hpp"
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "glm/gtc/constants.hpp" // glm::pi
#include "glm/gtc/type_ptr.hpp"

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

float * transFormations_get1(uint32_t u32NowTime)
{
    static glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    float fTime = GET_FLOAT_TIME_S(u32NowTime);
    transform = glm::rotate(transform, fTime, glm::vec3(0.0f, 0.0f, 1.0f));

    float * pfRet = (float *)(glm::value_ptr(transform));

//    printf("[%s %d]  [%.2f]\n", __func__, __LINE__, fTime);
//    for(uint32_t i = 0; i < 16; i ++)
//    {
//        printf("[%s %d]  idx:[%d[ pfRet[%.2f]\n", __func__, __LINE__, i, pfRet[i]);
//    }

    return pfRet;
}


