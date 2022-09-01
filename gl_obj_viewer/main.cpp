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
//#include "obj_loader.hpp"
#include "obj_data.hpp"

#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

using std::vector;
using std::string;

void Render(void);
//void Panel(void);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

//void removeData(int objid);
//bool manageData(int objcmd ,int objid, glm::mat4 transmat);

/*
bool enable_mouse = true;
GLuint display_mode = GL_FILL;
float yaw = 0;
float pitch = 0;
float roll = 0;
float scale = 0, SCALE = 1;

int obj_selected = 0;
string objfilepath;
bool show_file_dialog = false;
*/
/*
vector<string> objname;
vector<string> objpath;
//vector<vector<float>*> objdata;
vector<vector<float>*> objdata_v;
vector<vector<unsigned int>*> objdata_f;
vector<vector<float>*> objdata_vn;
*/
//Loader loader;

// 用于预览模型变换的临时变换矩阵temp_trans
//bool use_temp_trans = false;
//glm::mat4 temp_trans;

//渲染程序
void Render(void){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
}

// 主函数obj数据数组处理函数，于obj_panel中被调用
// 用于存储读取的obj文件数据、应用用户对模型的变换操作
/*bool manageData(int objcmd, int objid, glm::mat4 transmat){
    switch (objcmd) {
        case OBJ_INSERT:
            printf("insert %i\n", objid);
            if(loader.load(objpath[objid])){
                //objdata.push_back(new vector<float>);
                //if(loader.get(objdata[objid])){
                objdata_v.push_back(new vector<float>);
                objdata_f.push_back(new vector<unsigned int>);
                objdata_vn.push_back(new vector<float>);
                if(loader.get(objdata_v[objid], objdata_f[objid], objdata_vn[objid])){
                    for(int i=0; i<objdata_v[objid]->size(); ++i){
                        std::cout<<(*objdata_v[objid])[i]<<" ";
                        if(i%3==2)
                            std::cout<<std::endl;
                    }
                    std::cout<<objdata_v[objid]->size()/3<<std::endl;
                }else{
                    printf("failed to get data from %i\n", objid);
                    removeData(objid);
                }
            }
            break;
        case OBJ_REMOVE:
            printf("remove %i\n", objid);
            removeData(objid);
            break;
        case OBJ_TRANSFORM:
            use_temp_trans = true;
            temp_trans = transmat;
            break;
        case OBJ_CANCEL_TRANSFORM:
            use_temp_trans = false;
            temp_trans = glm::mat4(0);
            break;
        case OBJ_CONFIRM_TRANSFORM:
            //TODO: apply transform to objdata
            use_temp_trans = false;
            temp_trans = glm::mat4(0);
            break;
            
        default:
            printf("?\n");
            break;
    }
}*/

//imgui设置
/*
void Panel(void){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    //ImGui::ShowDemoWindow();
    ImGui::Begin("options");
    //ImGui::Text("Display mode");
    if(ImGui::TreeNode("Display Mode")){
        if (ImGui::RadioButton("fill", display_mode == GL_FILL))
            display_mode = GL_FILL;
        ImGui::SameLine();
        if (ImGui::RadioButton("linear", display_mode == GL_LINE))
            display_mode = GL_LINE;
        ImGui::TreePop();
    }
    //ImGui::Text("Rotate");
    if(ImGui::TreeNode("Rotate")){
        ImGui::Text("yaw  ");ImGui::SameLine();
        ImGui::SliderAngle("##yaw", &yaw);
        ImGui::Text("pitch");ImGui::SameLine();
        ImGui::SliderAngle("##pitch", &pitch);
        ImGui::Text("roll ");ImGui::SameLine();
        ImGui::SliderAngle("##roll", &roll);
        ImGui::Text("reset");ImGui::SameLine();
        if(ImGui::Button("rotation reset")){yaw = 0; pitch = 0; roll = 0;}
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Scale")){
        ImGui::Text("Scale: %f", SCALE);
        if(ImGui::SliderFloat("##scale index", &scale, -4, 4)){SCALE = pow(2.0f, scale);};
        ImGui::Text("reset");ImGui::SameLine();
        if(ImGui::Button("scale reset")){scale = 0; SCALE = 1;}
        ImGui::TreePop();
    }
    
    //ImGui::Text("Walk");
    if(ImGui::TreeNode("Objects")){
        // 向长径比中添加obj文件
        ImGui::Text("insert obj");ImGui::SameLine();
        if(ImGui::Button("insert")){
            show_file_dialog = true;
        }
        // 场景中显示的obj文件列表
        ImGui::Text("obj list");
        if (ImGui::BeginListBox("##obj list")) {
            for (int i = 0; i < objname.size(); ++i) {
                const bool isSelected = (obj_selected == i);
                if (ImGui::Selectable(objname[i].c_str(), isSelected)) {
                    obj_selected = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }
        ImGui::TreePop();
    }
    
    if(ImGui::TreeNode("Others")){
        ImGui::Checkbox("enable mouse control", &enable_mouse);
        ImGui::Text("reset transform");ImGui::SameLine();
        if(ImGui::Button("reset all")){
            yaw = 0; pitch = 0; roll = 0;
            scale = 0; SCALE = 1;
        }
        ImGui::Text("clear objects");ImGui::SameLine();
        ImGui::TreePop();
    }
    // 弹出添加obj文件窗口
    if(show_file_dialog){
        string objfilename;
        objfilepath = FileDialogImGui(&objfilename);
        //printf("%s\n", objpath.c_str());
        if (objfilepath != "") {
            show_file_dialog = false;
            //printf("%s\n", objfilepath.c_str());
            objpath.push_back(objfilepath);
            objname.push_back(objfilename);
        }
    }
    
    ImGui::End();

}
 */

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
    //panel.SaveIn(&objname, &objpath);
    //panel.SetHandler(manageData);
    panel.SaveIn(getObjName, getObjPath);
    panel.SetHandler(getHandler);
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //加载编译着色器
    //MARK: 若要使用相对路径，须在Product->Scheme->Edit Scheme->Run->Options->Use custom working directory中设置为项目路径
    Shader shader = shaderCreate("vertex_shader.vert", "fragment_shader.frag");
    
    //添加绘制对象
    vector<float> vertices{
        -0.5f, 0.288f, 0.0f,
        0.5f, 0.288f, 0.0f,
        0.0f, -0.577f, 0.0f
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //启动OpenGL窗口
    while (!glfwWindowShouldClose(window)) {
        //Panel();
        panel.Panel();
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        //glPolygonMode(GL_FRONT_AND_BACK, display_mode);
        glPolygonMode(GL_FRONT_AND_BACK, panel.display_mode);
        Render();
        processInput(window);

        shaderUse(shader);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawArrays(GL_LINES, 0, 2);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shaderDelete(shader);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}

/*
void removeData(int objid){
    objdata_v[objid]->clear();
    objdata_f[objid]->clear();
    objdata_vn[objid]->clear();
    free(objdata_v[objid]);
    free(objdata_f[objid]);
    free(objdata_vn[objid]);
    objdata_v.erase(objdata_v.begin() + objid);
    objdata_f.erase(objdata_f.begin() + objid);
    objdata_vn.erase(objdata_vn.begin() + objid);
}
*/

//窗口被拖拽时恢复原先大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

//输入反馈
void processInput(GLFWwindow *window){
    //按下Esc键时关闭窗口
    if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
