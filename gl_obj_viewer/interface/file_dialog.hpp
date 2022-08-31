//
//  file_dialog.hpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/15.
//

#ifndef file_dialog_hpp
#define file_dialog_hpp

#include <iostream>
#include <sys/stat.h>
#if defined(__APPLE__) || defined(__linux__)
#include <unistd.h>
#include <dirent.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <direct.h>
#endif
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>
#include <string>

class FileDialog{
private:
    std::string _dlg_title;                                  // 文件选择窗口标题
    std::vector<std::string> _cwdls;                         // 当前路径数组（用于快捷前往操作）
    std::vector<std::string> _extls;                         // 目标后缀名数组，用于筛选文件类型
    char _cwd[200];                                          // 当前路径
    bool _search_again;                                      // 是否重新搜索路径下所有文件名flag
    std::vector<std::string> _filels;                        // 当前路径下所有符合条件的文件名数组
    int _file_selected;                                      // 当前选中的文件在_filels数组中的下标
    bool _should_close;                                      // 是否关闭文件选择窗口
    
    bool _getFileList(const char* dp);                       // 获取当前路径下符合条件的文件名数组
    bool _is_hidden(const std::string &filename);            // 判断文件是否为隐藏文件
    bool _is_dir(const std::string &filename);               // 判断文件是否为路径（文件夹）
    bool _is_compl_file(const std::string &filename);        // 判断文件（后缀名）是否符合条件
    void _switch_cwd(const std::string &dirname);            // 切换当前路径至目标路径（参数dirname为当前路径下的相对路径）
    void _switch_cwd_abs(const std::string &dirname);        // 与上面功能一致，参数为绝对路径
    void _updt_cwdls(void);                                  // 更新_cwdls数组
    
public:
    FileDialog(std::string title= "##File");                 // 构造函数，可选参数为窗口标题
    void setTargetExt(const char* extlst);                   // 设置目标文件后缀名，“*”为不限
    std::string get(std::string* obj_name);                  // 显示文件窗口，以选择文件，返回值为文件的绝对路径
    void display(void);                                      // 设置文件窗口可见
    bool ifDisplay(void);                                    // 判断文件窗口是否可见
    ~FileDialog();
};

// deprecated
/*
static std::vector<std::string> _extls;

static char _cwd[200];
static bool _search_again = true;
static std::vector<std::string> _filels;
static int _file_selected = 0;

bool _getFileList(const char* dp, std::vector<std::string>* fl);
std::string FileDialogImGui(std::string* obj_name);

void setTargetExt(const char* extlst);

bool _is_dir(std::string filename);
bool _is_compl_file(std::string filename);
*/

#endif /* file_dialog_hpp */
