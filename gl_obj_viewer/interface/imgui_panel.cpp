//
//  imgui_panel.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/15.
//

#include "imgui_panel.hpp"

using std::vector;
using std::string;

// 构造函数，控制面板中一些参数的初始化
ImGuiPanel::ImGuiPanel(void){
    scale = 0;
    
    enable_mouse = false;
    display_mode = GL_FILL;
    show_coord = true;
    yaw = -M_PI / 6;
    pitch = M_PI / 9;
    roll = 0;
    SCALE = 1;
    front = 0;
    up = 0;
    right = 0;
    distance = 1.0f;
    
    camera = Camera(.0f, .0f, 1.0f, .0f, 1.0f, .0f,
                    &(this->yaw), &(this->pitch), &(this->roll), &(this->SCALE),
                    &(this->front), &(this->up), &(this->right), &(this->distance));
        
    panelSetUp();
}

// 控制面板布局设置
void ImGuiPanel::Panel(void){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("options");
    ImGui::Text("Camera arguments");
    // 显示模式，选择以多边形或是线框的形式进行展示
    if(ImGui::CollapsingHeader("Display Mode")){
        if (ImGui::RadioButton("fill", display_mode == GL_FILL))
            display_mode = GL_FILL;
        ImGui::SameLine();
        if (ImGui::RadioButton("linear", display_mode == GL_LINE))
            display_mode = GL_LINE;
        ImGui::Checkbox("show coordinate", &show_coord);
    }
    // 场景旋转，旋转的三个参数为欧拉角参数，单击按钮后旋转重置
    if(ImGui::CollapsingHeader("Camera Condition")){
        ImGui::Text("yaw");ImGui::SameLine(0, 50);
        if(ImGui::SliderAngle("##yaw", &yaw)){
            camera.updateCameraVectors();
        }
        ImGui::Text("pitch");ImGui::SameLine(0, 44);
        if(ImGui::SliderAngle("##pitch", &pitch)){
            camera.updateCameraVectors();
        }
        //ImGui::Text("roll");ImGui::SameLine(0, 54);
        //if(ImGui::SliderAngle("##roll", &roll)){
        //    camera.updateCameraVectors();
        //}
        ImGui::Text("distance:");ImGui::SameLine();
        if(ImGui::SliderFloat("##distance", &distance, 0, 5)){
            camera.updateCameraVectors();
        }
        ImGui::Text("reset");ImGui::SameLine(0, 44);
        if(ImGui::Button("rotation reset")){
            yaw = 0; pitch = 0; roll = 0;
            camera.updateCameraVectors();
        }
    }
    // 场景缩放，缩放的倍率为参数SCALE，参数scale为缩放倍数的指数
    if(ImGui::CollapsingHeader("Field of View")){
        ImGui::Text("Scale: %.2f", SCALE);ImGui::SameLine(0, SCALE >= 10 ? 3 : 10);
        if(ImGui::SliderFloat("##scale index", &scale, -.75f, .25f)){SCALE = (1 - (scale + .75f)) * M_PI;};
        ImGui::Text("reset");ImGui::SameLine(0, 44);
        if(ImGui::Button("scale reset")){scale = 0; SCALE = 1;}
    }
    
    //ImGui::Text("Wander");
    if(ImGui::CollapsingHeader("Camera Position")){
        ImGui::Text("right");ImGui::SameLine(0, 40);
        if(ImGui::SliderFloat("##right", &this->right, -1, 1))
            camera.updateCameraVectors();
        ImGui::Text("up");ImGui::SameLine(0, 50);
        if(ImGui::SliderFloat("##up", &this->up, -1, 1))
            camera.updateCameraVectors();
        ImGui::Text("front");ImGui::SameLine(0, 38);
        if(ImGui::SliderFloat("##front", &this->front, -1, 1))
            camera.updateCameraVectors();
        ImGui::Text("reset");ImGui::SameLine(0, 44);
        if(ImGui::Button("back to origin")){
            yaw = 0; pitch = 0; roll = 0;
            front = 0; up = 0; right = 0;
            camera.reset();
        }
    }
    // obj文件列表，添加、移除obj文件，对obj文件进行变换操作

    // 其他，包括设置是否可以通过鼠标进行变换、重置场景变换等
    if(ImGui::CollapsingHeader("Others")){
        ImGui::BeginDisabled();
        ImGui::Checkbox("enable mouse control", &enable_mouse);
        ImGui::EndDisabled();
        ImGui::Text("reset transform");ImGui::SameLine();
        if(ImGui::Button("reset all")){
            yaw = 0; pitch = 0; roll = 0;
            scale = 0; SCALE = 1;
            front = 0; up = 0; right = 0;
            camera.reset();
        }
        //ImGui::Text("clear objects");ImGui::SameLine();
    }
    
    if(ImGui::CollapsingHeader("About")){
        ImGui::TextDisabled("build with opengl, dear imgui;\n using font Adobe Source Han Sans.");
    }
    ImGui::End();
    
    obj_panel.Panel();
    
}

// 控制面板外观配置
int ImGuiPanel::panelSetUp(void){
    int opc = 0;
    
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    char _font_path[] = "../assets/SourceHanSansSC-Regular.otf";
    if(access(_font_path, F_OK) != -1){
        io.Fonts->AddFontFromFileTTF(_font_path, 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
        opc++;
    }
        
    style.FrameRounding = 6.0f; opc++;
    style.GrabRounding = 6.0f; opc++;
    
    style.Colors[2] = ImVec4(.0f, .0f, .0f, .5f); opc++;
    
    return opc;
}

// 设置ObjPanel中读取的文件名、文件路径等数据在主程序中存放的数组
void ImGuiPanel::SaveIn(vector<string>* objnamemain, vector<string>* objpathmain){
    if(objnamemain != NULL && objpathmain != NULL)
        this->obj_panel.SaveIn(objnamemain, objpathmain);
}

// 将主程序中的objdata数组处理函数句柄传入ObjPanel中以便调用
/*
bool ImGuiPanel::SetHandler(void(*op) (int objcmd ,int objid, glm::mat4 transmat)){
    if(op == NULL)
        return false;
    obj_panel.SetHandler(op);
    return true;
}
*/

bool ImGuiPanel::SetHandler(bool(*op) (int objcmd ,int objid, glm::mat4 transmat)){
    if(op == NULL)
        return false;
    obj_panel.SetHandler(op);
    return true;
}

glm::mat4 ImGuiPanel::getViewMatrix(void){
    return this->camera.GetViewMatrix();
}

glm::mat4 ImGuiPanel::getProjection(float scrwid, float scrht){
    return this->camera.GetProjection(scrwid, scrht);
}

glm::vec3 ImGuiPanel::getCameraPos(void){
    return this->camera.GetCameraPos();
}
