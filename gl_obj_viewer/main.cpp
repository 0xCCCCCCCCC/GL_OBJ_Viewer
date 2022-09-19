//
//  main.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/11.
//

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm.hpp"

#include "shader.hpp"
#include "imgui_panel.hpp"
#include "obj_data.hpp"

#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

using std::vector;
using std::string;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char * argv[]){
    //OpenGL初始化，声明版本号为3.3，使用核心配置文件
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Mac系统需要
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    //创建窗口
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test OpenGL", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetKeyCallback(window, key_call_back);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    ImGuiPanel panel;
    panel.SaveIn(getObjName, getObjPath);
    panel.SetHandler(getHandler);
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //加载编译着色器
    //MARK: 若要使用相对路径，须在Product->Scheme->Edit Scheme->Run->Options->Use custom working directory中设置为项目路径
    Shader shader = shaderCreate("shader/vertex_shader.vert", "shader/fragment_shader.frag");
    Shader coord_shader = shaderCreate("shader/coord_shader_v.vert", "shader/coord_shader_f.frag");
    
    //初始化坐标系
    genCoord(1.0f);
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ARRAY_BUFFER, getLocalVtxSize, getLocalVtxPtr, GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLocalIndexSize, getLocalIndexPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    unsigned int C_VBO, C_VAO;
    glGenVertexArrays(1, &C_VAO);
    glGenBuffers(1, &C_VBO);
    glBindVertexArray(C_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, C_VBO);
    glBufferData(GL_ARRAY_BUFFER, getCoordSize, getCoordPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 场景中当前Vertex的数量，用于判断是否新增obj模型
    //unsigned int local_pt_cnt = 0;
    
    //glEnable(GL_CULL_FACE);
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    //启动OpenGL窗口
    while (!glfwWindowShouldClose(window)) {
        panel.Panel();
        glPolygonMode(GL_FRONT_AND_BACK, panel.display_mode);
        glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        
        // 当场景中新增obj模型时，重新绑定VBO与EBO
        if(getIfTransform /*|| getLocalPtCount != local_pt_cnt*/){
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, getLocalVtxSize, getLocalVtxPtr, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLocalIndexSize, getLocalIndexPtr, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            //local_pt_cnt = getLocalPtCount;
            localTransformComplete();
        }
        // 将观察矩阵等参数传入着色器程序
        shaderUse(shader);
        shaderSetMat4(shader, "projection", panel.getProjection(WINDOW_WIDTH, WINDOW_HEIGHT));
        shaderSetMat4(shader, "view", panel.getViewMatrix());
        shaderSetVec3(shader, "lightPos", glm::vec3(1, 1, 1));
        shaderSetVec3(shader, "lightColor", glm::vec3(.6, .8, .8));
        shaderSetVec3(shader, "viewPos", panel.getCameraPos());
        shaderSetFloat(shader, "ambientStrength", .3f);
        shaderSetFloat(shader, "shininess", 2);
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, getLocalPtCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        if(panel.show_coord){
            shaderUse(coord_shader);
            shaderSetMat4(coord_shader, "projection", panel.getProjection(WINDOW_WIDTH, WINDOW_HEIGHT));
            shaderSetMat4(coord_shader, "view", panel.getViewMatrix());
            glBindVertexArray(C_VAO);
            glDrawArrays(GL_LINES, 0, getCoordCnt);
            glBindVertexArray(0);
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    clearObjData();
    
    glfwTerminate();
    return 0;
}

//窗口被拖拽时恢复原先大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

//输入反馈
void processInput(GLFWwindow *window){
    //按下Esc键时关闭窗口
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
