//
//  imgui_panel.hpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/15.
//

#ifndef imgui_panel_hpp
#define imgui_panel_hpp

#include <iostream>
#include <math.h>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "file_dialog.hpp"
#include "obj_panel.hpp"

#include "camera.h"

class ImGuiPanel{
private:
    float scale;
    Camera camera;
    ObjPanel obj_panel;

public:
    bool enable_mouse;
    GLuint display_mode;
    // 欧拉角参数
    float yaw;
    float pitch;
    float roll;
    // 缩放系数
    float SCALE;
    // 位移参数
    float front;
    float up;
    float right;

    // obj文件名称、绝对路径、顶点参数数组
    
    ImGuiPanel(void);
    void Panel(void);
    
    int panelSetUp(void);
    void SaveIn(std::vector<std::string>* objnamemain, std::vector<std::string>* objpathmain);
    
    //bool SetHandler(void(*op) (int objcmd ,int objid, glm::mat4 transmat));
    bool SetHandler(bool(*op) (int objcmd ,int objid, glm::mat4 transmat));
};

#endif /* imgui_panel_hpp */
