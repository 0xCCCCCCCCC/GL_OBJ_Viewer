//
//  obj_panel.hpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/16.
//

#ifndef obj_panel_hpp
#define obj_panel_hpp

#include <iostream>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "file_dialog.hpp"

#define OBJ_STATIC 0                                        // 无变化
#define OBJ_INSERT 1                                        // 插入新obj文件
#define OBJ_TRANSFORM 2                                     // 有obj发生了变化（缩放、位移、旋转等）
#define OBJ_REMOVE 3                                        // 有obj被删除

#define OBJ_CONFIRM_TRANSFORM 21
#define OBJ_CANCEL_TRANSFORM 20

class ObjPanel{
private:
    int obj_selected;                                       // 选中obj在数组中的下标
    std::string obj_file_path;                              // 从FileDlg返回的obj文件路径
    bool show_file_dialog;                                  // 是否显示FileDlg

    FileDialog file_dialog;                                 // 文件选择窗口
    
    void(*op) (int objcmd ,int objid, glm::mat4 transmat);  // 主程序中objdata数组的处理函数句柄
    
    bool save_elsewhere;                                    // 是否将数据存储于外部（为true则存储于外部，为false则存储于类的内部数组）
    std::vector<std::string>* objnameew;                    // 外部（主函数）obj文件名数组指针
    std::vector<std::string>* objpathew;                    // 外部（主函数）obj文件路径指针

    float obj_scale;
    float obj_x;    float obj_y;        float obj_z;
    float obj_yaw;  float obj_pitch;    float obj_roll;
    glm::mat4 obj_trans;
    
    bool _if_dup(std::string _objname);                     // 检测是否重名
    
    bool _insert_obj(void);                                 // 插入新的obj文件，若成功插入则返回true
    bool _remove_obj(void);                                 // 删除obj文件，若删除成功则返回true
    
    void _calc_trans_mat(void);
    bool _trans_can(void);
    bool _trans_apply(void);
    
public:
    std::vector<std::string> objname;                       // 类内部obj文件名数组
    std::vector<std::string> objpath;                       // 类内部obj文件路径数组
    //std::vector<std::vector<float>> objdata;
    
    ObjPanel(void);
    void Panel(void);                                       // 展示面板，设置面板的基本布局
    
                                                            // 设置外部存储数组的指针
    void SaveIn(std::vector<std::string>* objnameout, std::vector<std::string>* objpathout);
                                                            // 传入主函数中处理函数句柄
    void SetHandler(void(*op) (int objcmd ,int objid, glm::mat4 transmat));
};

#endif /* obj_panel_hpp */
