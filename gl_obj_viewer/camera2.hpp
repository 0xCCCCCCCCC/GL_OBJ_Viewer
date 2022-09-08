//
//  camera2.hpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/9/5.
//

/// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

#ifndef camera2_hpp
#define camera2_hpp

#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include <vector>

class Camera
{
private:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float* Yaw;
    float* Pitch;
    float* Roll;
    // camera options
    float* Zoom;
    
    float* transFront;
    float* transUp;
    float* transRight;
    
    float* Distance;
    
    glm::vec3 OriginPos;

public:

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)){
        Position    = position;
        WorldUp     = up;
        Yaw     = NULL;
        Pitch   = NULL;
        Roll    = NULL;
        Zoom    = NULL;
        transFront  = NULL;
        transUp     = NULL;
        transRight  = NULL;
        Distance = NULL;
        OriginPos   = position;
        if(position == glm::vec3(.0f, .0f, .0f))
            Front = glm::vec3(0.0f, 0.0f, -1.0f);
        else
            Front = position;
    }
    
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ){
        Position    = glm::vec3(posX, posY, posZ);
        WorldUp     = glm::vec3(upX, upY, upZ);
        Yaw     = NULL;
        Pitch   = NULL;
        Roll    = NULL;
        Zoom    = NULL;
        transFront  = NULL;
        transUp     = NULL;
        transRight  = NULL;
        Distance = NULL;
        OriginPos   = Position;
        if(Position == glm::vec3(.0f, .0f, .0f))
            Front = glm::vec3(0.0f, 0.0f, -1.0f);
        else
            Front = Position;
    }
    
    // constructor with vectors
    Camera(float* yaw, float* pitch, float* roll, float* zoom,
           float* transfront, float* transup, float* transright, float* distance,
           glm::vec3 position = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        Position    = position;
        WorldUp     = up;
        Yaw     = yaw;
        Pitch   = pitch;
        Roll    = roll;
        Zoom    = zoom;
        transFront  = transfront;
        transUp     = transup;
        transRight  = transright;
        Distance    = distance;
        updateCameraVectors();
        OriginPos   = position;
    }
    
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
           float* yaw, float* pitch, float* roll, float* zoom,
           float* transfront, float* transup, float* transright, float* distance)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        Position    = glm::vec3(posX, posY, posZ);
        WorldUp     = glm::vec3(upX, upY, upZ);
        Yaw     = yaw;
        Pitch   = pitch;
        Roll    = roll;
        Zoom    = zoom;
        transFront  = transfront;
        transUp     = transup;
        transRight  = transright;
        Distance    = distance;
        updateCameraVectors();
        OriginPos   = Position;
    }

    glm::mat4 GetViewMatrix(void);
    
    glm::mat4 GetProjection(float SCR_WIDTH, float SCR_HEIGHT);

    void updateCameraVectors(void);
    
    void updateCameraTransform(void);
    
    void set(float* yaw, float* pitch, float* roll, float* zoom,
             float* transfront, float* transup, float* transright, float* distance);
    
    void reset(void);
    
};

#endif /* camera2_hpp */
