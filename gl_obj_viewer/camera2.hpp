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

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
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
    //float MovementSpeed;
    //float MouseSensitivity;
    float* Zoom;
    
    float* transFront;
    float* transUp;
    float* transRight;

public:

    // constructor with vectors
    Camera(float* yaw, float* pitch, float* roll, float* zoom,
           glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        Zoom = zoom;
        updateCameraVectors();
    }
    
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
           float* yaw, float* pitch, float* roll, float* zoom)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f))
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        Roll = roll;
        Zoom = zoom;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix(void);
    
    glm::mat4 GetProjection(float SCR_WIDTH, float SCR_HEIGHT);

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors(void);
    
    void updateCameraTransform(void);
    
    void Reset(void);
    
};

#endif /* camera2_hpp */
