//
//  camera2.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/9/5.
//

#include "camera2.hpp"

// 观察矩阵
glm::mat4 Camera::GetViewMatrix(void){
    return glm::lookAt(Position, Position + Front, Up);
}

// 投影矩阵
glm::mat4 Camera::GetProjection(float SCR_WIDTH, float SCR_HEIGHT){
    //return glm::perspective(glm::radians(*Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    return glm::perspective(*Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

glm::vec3 Camera::GetCameraPos(void){
    return this->Position;
}

// 更新相机向量
void Camera::updateCameraVectors(void){
    glm::vec3 front;
    front.x = cos(*Yaw - .5f * M_PI) * cos(-*Pitch);
    front.y = sin(-*Pitch);
    front.z = sin(*Yaw - .5f * M_PI) * cos(-*Pitch);
    Front   = glm::normalize(front);
    
    if(abs(*Pitch) < .5f * M_PI || abs(*Pitch) > 1.5f * M_PI)
        Right = glm::normalize(glm::cross(Front, WorldUp));
    else
        Right = glm::normalize(glm::cross(WorldUp, Front));
    Up  = glm::normalize(glm::cross(Right, Front));
    
    Position = -*Distance * (Front + glm::mat3(Right, Up, Front) * glm::vec3(*(this->transRight), *(this->transUp), -*(this->transFront)));
}

void Camera::set(float* yaw, float* pitch, float* roll, float* zoom,
                 float* transfront, float* transup, float* transright, float* distance){
    Yaw     = yaw;
    Pitch   = pitch;
    Roll    = roll;
    Zoom    = zoom;
    transFront  = transfront;
    transUp     = transup;
    transRight  = transright;
    Distance = distance;
    updateCameraVectors();
}

// 重置相机参数
void Camera::reset(void){
    this->Position  = this->OriginPos;
    this->WorldUp   = glm::vec3(.0f, 1.0f, .0f);
    
    if(this->Position == glm::vec3(.0f, .0f, .0f))
        this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    else
        this->Front = this->Position;
    *(this->Distance) = glm::length(this->OriginPos);
    updateCameraVectors();
}
