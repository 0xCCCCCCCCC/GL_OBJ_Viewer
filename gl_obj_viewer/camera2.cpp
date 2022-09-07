//
//  camera2.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/9/5.
//

#include "camera2.hpp"

glm::mat4 Camera::GetViewMatrix(void){
    glm::vec3 CurPos = Position + glm::vec3(*(this->transRight), *(this->transUp), *(this->transFront));
    return glm::lookAt(CurPos, CurPos + Front, Up);
}

glm::mat4 Camera::GetProjection(float SCR_WIDTH, float SCR_HEIGHT){
    //return glm::perspective(glm::radians(*Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    return glm::perspective(*Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void Camera::updateCameraVectors(void){
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(*Yaw) * cos(*Pitch);
    front.y = sin(*Pitch);
    front.z = sin(*Yaw) * cos(*Pitch);
    Front = glm::normalize(front);
    
    //TODO: update worldup vector
    //glm::vec3 up;
    
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::updateCameraTransform(void){
    this->Position      += glm::vec3(*(this->transRight), *(this->transUp), *(this->transFront));
    *(this->transRight) = 0;
    *(this->transUp)    = 0;
    *(this->transFront) = 0;
}

void Camera::set(float* yaw, float* pitch, float* roll, float* zoom, float* transfront, float* transup, float* transright){
    Yaw     = yaw;
    Pitch   = pitch;
    Roll    = roll;
    Zoom    = zoom;
    transFront  = transfront;
    transUp     = transup;
    transRight  = transright;
    updateCameraVectors();
}

void Camera::reset(void){
    this->Position  = this->OriginPos;
    this->WorldUp   = glm::vec3(.0f, 1.0f, .0f);
    
    if(this->Position == glm::vec3(.0f, .0f, .0f))
        this->Front = glm::vec3(0.0f, 0.0f, -1.0f);
    else
        this->Front = this->Position;
}
