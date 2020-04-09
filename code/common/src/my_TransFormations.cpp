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
// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

/*----------------------------------------------*/
/*                 类定义                     */
/*----------------------------------------------*/

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::mat4 mat4LooaAt;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera():Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw = YAW;
        Pitch = PITCH;
        updateCameraVectors();
    }

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    float * GetViewMatrix()
    {   
        mat4LooaAt = glm::lookAt(Position, Position + Front, Up);
        return (float *)(glm::value_ptr(mat4LooaAt));
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraKey direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};


/*----------------------------------------------*/
/*                 函数定义                     */
/*----------------------------------------------*/

float * transFormations_get1(uint32_t u32NowTime)
{
    static glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    float fTime = GET_FLOAT_TIME_S(u32NowTime);

    float fYpitch = (float)fmod(fTime * 0.1, 2.0f);
    transform = glm::translate(transform, glm::vec3(-1.0f + fYpitch, -0.5f, 0.0f));

    transform = glm::rotate(transform, fTime, glm::vec3(0.0f, 0.0f, 1.0f));

    float * pfRet = (float *)(glm::value_ptr(transform));

//    printf("[%s %d]  [%.2f]\n", __func__, __LINE__, fTime);
//    for(uint32_t i = 0; i < 16; i ++)
//    {
//        printf("[%s %d]  idx:[%d[ pfRet[%.2f]\n", __func__, __LINE__, i, pfRet[i]);
//    }

    return pfRet;
}


float * transFormations_get2(uint32_t u32NowTime)
{
    static glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    float fTime = GET_FLOAT_TIME_S(u32NowTime);
    
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scaleAmount = sin(fTime);
    transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));


    float * pfRet = (float *)(glm::value_ptr(transform));

//    printf("[%s %d]  [%.2f]\n", __func__, __LINE__, fTime);
//    for(uint32_t i = 0; i < 16; i ++)
//    {
//        printf("[%s %d]  idx:[%d[ pfRet[%.2f]\n", __func__, __LINE__, i, pfRet[i]);
//    }

    return pfRet;
}


int32_t coordinateSystem_getMat(CoorSysInfo * pstInfo, uint32_t u32NowTime)
{
    // create transformations
    static glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    static glm::mat4 view          = glm::mat4(1.0f);
    static glm::mat4 projection    = glm::mat4(1.0f);
    model         = glm::mat4(1.0f); 
    view          = glm::mat4(1.0f);
    projection    = glm::mat4(1.0f);

    float fTime = GET_FLOAT_TIME_S(u32NowTime);
    
    model = glm::rotate(model, fTime, glm::vec3(0.5f, 1.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), 
                                  (float)pstInfo->s32CurSurfaceW / (float)pstInfo->s32CurSurfaceH, 
                                  0.1f, 
                                  100.0f);

    pstInfo->pfModelMat =  (float *)(glm::value_ptr(model));
    pstInfo->pfViewMat =  (float *)(glm::value_ptr(view));
    pstInfo->pfProjectionMat =  (float *)(glm::value_ptr(projection));

    return OK;
}


int32_t coordinateSystemMuliCube_getMat(CoorSysInfo * pstInfo, uint32_t u32Idx)
{
    // create transformations
    static glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };    
    static glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    static glm::mat4 view          = glm::mat4(1.0f);
    static glm::mat4 projection    = glm::mat4(1.0f);
    model         = glm::mat4(1.0f); 
    view          = glm::mat4(1.0f);
    projection    = glm::mat4(1.0f);

    model = glm::translate(model, cubePositions[u32Idx]);
    float angle = 20.0f * (float)u32Idx;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), 
                                  (float)pstInfo->s32CurSurfaceW / (float)pstInfo->s32CurSurfaceH, 
                                  0.1f, 
                                  100.0f);

    pstInfo->pfModelMat =  (float *)(glm::value_ptr(model));
    pstInfo->pfViewMat =  (float *)(glm::value_ptr(view));
    pstInfo->pfProjectionMat =  (float *)(glm::value_ptr(projection));

    return OK;
}


void * Camera_creatHandle()
{
    Camera * pMyCamera =  new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    return (void *)pMyCamera;
}

void Camera_deleteHandle( void * pHandle)
{
    BASE_CHECK_TRUE_RET_VOID(NULL == pHandle);

    Camera * pMyCamera = (Camera *)pHandle;
    delete pMyCamera;
    
    return;
}

void Camera_processKey(void *pHandle, CameraKey eKey)
{
    // TODO ECS 调用关闭窗口
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);

    BASE_CHECK_TRUE_RET_VOID(NULL == pHandle);

    Camera * pMyCamera = (Camera *)pHandle;

    //  TODO deltaTime 变量
    pMyCamera->ProcessKeyboard(eKey, 0.040f);

    return;
}

float * Camera_getViewMatrix(void * pHandle)
{
    BASE_CHECK_TRUE_RET(NULL == pHandle, NULL);
    
    Camera * pMyCamera = (Camera *)pHandle;

    return pMyCamera->GetViewMatrix();
}


void Camera_scrollMouse(void * pHandle, double yoffset)
{
    BASE_CHECK_TRUE_RET_VOID(NULL == pHandle);

    Camera * pMyCamera = (Camera *)pHandle;

    pMyCamera->ProcessMouseScroll(yoffset);
}

