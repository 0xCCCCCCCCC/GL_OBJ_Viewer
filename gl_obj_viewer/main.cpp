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

#include "shader.h"
#include "imgui_panel.hpp"
#include "obj_data.hpp"

#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define TEST_OBJ

using std::vector;
using std::string;

//void Render(void);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void shaderSetMat4(Shader shader, string name, glm::mat4 mat);

//渲染程序
/*void Render(void){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}*/

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
    Shader shader = shaderCreate("vertex_shader.vert", "fragment_shader.frag");
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, getLocalVtxSize, getLocalVtxPtr, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLocalIndexSize, getLocalIndexPtr, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 场景中当前Vertex的数量，用于判断是否新增obj模型
    unsigned int local_pt_cnt = 0;
    
    //glEnable(GL_CULL_FACE);
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    
    //启动OpenGL窗口
    while (!glfwWindowShouldClose(window)) {
        panel.Panel();
        glPolygonMode(GL_FRONT_AND_BACK, panel.display_mode);
        //Render();
        glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        
        // 当场景中新增obj模型时，重新绑定VBO与EBO
        if(getLocalPtCount != local_pt_cnt){
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, getLocalVtxSize, getLocalVtxPtr, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, getLocalIndexSize, getLocalIndexPtr, GL_STATIC_DRAW);
            //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
            //glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            local_pt_cnt = getLocalPtCount;
            //std::cout<<local_pt_cnt<<std::endl;
        }
        shaderUse(shader);
        shaderSetMat4(shader, "projection", panel.getProjection(WINDOW_WIDTH, WINDOW_HEIGHT));
        shaderSetMat4(shader, "view", panel.getViewMatrix());
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, getLocalPtCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shaderDelete(shader);
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

void shaderSetMat4(Shader shader, string name, glm::mat4 mat){
    glUniformMatrix4fv(glGetUniformLocation(*shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
