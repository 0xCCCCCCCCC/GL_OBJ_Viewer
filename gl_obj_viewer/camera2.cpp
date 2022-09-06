//
//  camera2.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/9/5.
//

#include "camera2.hpp"

glm::mat4 Camera::GetViewMatrix(void){
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjection(float SCR_WIDTH, float SCR_HEIGHT){
    return glm::perspective(glm::radians(*Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //return glm::perspective(*Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
}

void Camera::updateCameraVectors(void){
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(*Yaw) * cos(*Pitch);
    front.y = sin(*Pitch);
    front.z = sin(*Yaw) * cos(*Pitch);
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

void updateCameraTransform(void){
    
}
