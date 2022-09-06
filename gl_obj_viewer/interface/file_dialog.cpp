//
//  file_dialog.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/15.
//

#include "file_dialog.hpp"

using std::vector;
using std::string;

FileDialog::FileDialog(string title){
    _search_again = true;
    _should_close = true;
    _file_selected = 0;
    _dlg_title = title;
    
#if defined(__APPLE__) || defined(__linux__)
    getcwd(_cwd, 200);
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    _getcwd(cwd, 200);
#endif

}

FileDialog::~FileDialog(){}

// 判断是否为隐藏文件，即文件名是否以.开头；若为隐藏文件则返回true
bool FileDialog::_is_hidden(const string &filename){
    string __filename = filename;
    __filename.erase(0, __filename.find_first_not_of(" "));
    if(__filename.length() < 2) return false;
    return (__filename[0] == '.') && (__filename[1] != '.');
}

// 判断是否为文件夹
bool FileDialog::_is_dir(const string &filename){
    string abspath = _cwd + string("/") + filename;
    struct stat buffer;
    return (stat(abspath.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

// 判断文件格式是否符合，即文件后缀名是否与先前设定的后缀名一致
bool FileDialog::_is_compl_file(const string &filepath){
    if(_extls[0] == "*")
        return true;
    
    char __filepath[100];
    strcpy(__filepath, filepath.c_str());
    const char s[2] = ".";
    char *token;

    string ext = "";
    token = strtok(__filepath, s);

    while( token != NULL ) {
        ext = token;
        token = strtok(NULL, s);
    }
    
    for(string exti : _extls)
        if(ext == exti.substr(1))
            return true;
    
    return false;
}

// 获取路径dp下的所有文件名（除去隐藏文件与当前路径(.)）
bool FileDialog::_getFileList(const char* dp){
    _filels.clear();
#if defined(__APPLE__) || defined(__linux__)
    DIR * dir;
    struct dirent *ent;
    if((dir = opendir(dp)) != NULL){
        while((ent = readdir(dir)) != NULL){
            if((!_is_hidden(ent->d_name)) && (_is_dir(ent->d_name) || _is_compl_file(ent->d_name)))
                _filels.push_back(ent->d_name);
        }
        closedir(dir);
        return true;
    }
    else{
        perror("");
        return false;
    }
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    //TODO: windows get file list
#endif
}

// 切换文件对话框的当前路径（工作路径），使用相对路径
void FileDialog::_switch_cwd(const string &dirname){
    if(!_is_dir(dirname))
        return;
    
    string _dirname = dirname;
    _dirname.erase(0, _dirname.find_first_not_of(" "));
    _dirname.erase(_dirname.find_last_not_of(" ") + 1);
    
    string __cwd = _cwd;
    if(_dirname == ".."){
        __cwd.erase(__cwd.find_last_of("/"));
        if(__cwd.length() < 1) return;//__cwd = "/";
        strncpy(_cwd, __cwd.c_str(), __cwd.length() + 1);
        _search_again = true;
    }else{
        __cwd += string("/") + _dirname;
        strncpy(_cwd, __cwd.c_str(), __cwd.length() + 1);
        _search_again = true;
    }
}

// 与上一个功能一致，但是使用绝对路径
void FileDialog::_switch_cwd_abs(const string &dirname){
    //if(_is_dir(dirname)){
        strncpy(_cwd, dirname.c_str(), dirname.length() + 1);
        _search_again = true;
    //}
}

// 更新当前工作路径
void FileDialog::_updt_cwdls(void){
    _cwdls.clear();
    char __cwd[100];
    strcpy(__cwd, _cwd);
#if defined(__APPLE__) || defined(__linux__)
    const char s[2] = "/";
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    const char s[2] = "\\";
#endif
    char *token;

    token = strtok(__cwd, s);

    while( token != NULL ) {
        _cwdls.push_back(token);
        token = strtok(NULL, s);
    }
}

// 显示文件对话框，并返回选择的文件绝对路径
string FileDialog::get(std::string* obj_name){
    string file_path = "";
    
    // 若flag为true则重新搜索当前路径并更新_cwd, _cwdls等值
    if(_search_again){
        _updt_cwdls();
        _search_again = false;
        
        if(!_getFileList(_cwd))
            return "";
        _file_selected = 0;
    }
        
    ImGui::Begin(_dlg_title.c_str());
    ImGui::Text("Choose file:");
    // 文件目录列表，仅显示文件夹以及符合条件的文件
    // 单击文件以选中，双击文件夹以前往该文件夹
    if (ImGui::BeginListBox("##obj list")) {
        for (int i = 1; i < _filels.size(); ++i) {
            const bool isSelected = (_file_selected == i);
            ImGui::Selectable(_filels[i].c_str(), isSelected);
            
            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
                _file_selected = i;
            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
                _switch_cwd(_filels[i]);
            
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
    ImGui::SameLine();
    // 当前路径列表，单击以快速前往相应路径
    if (ImGui::BeginListBox("##cwd path", ImVec2(70, 0))) {
        for (int i = 0; i < _cwdls.size(); ++i) {
            if(ImGui::Selectable(_cwdls[i].c_str())){
                string _tar_abs_path = "";
                for(int tap_i = 0; tap_i <= i; ++tap_i)
                    _tar_abs_path += "/" + _cwdls[tap_i];
                _switch_cwd_abs(_tar_abs_path);
            }
        }
        ImGui::EndListBox();
    }
    // 确认按键，若选中需要的文件并单击此按钮，函数返回文件的绝对路径，并使窗口隐藏
    if(ImGui::Button("confirm")){
        file_path=_filels[_file_selected];
        if(_is_dir(file_path))
            _switch_cwd(file_path);
        if(_is_compl_file(file_path)){
            ImGui::End();
            _search_again = true;
            if(obj_name != NULL)
                *obj_name = file_path.substr(0, file_path.find_last_of("."));
            _should_close = true;
            return _cwd + string("/") + file_path;
        }
        file_path = "";
    }
    ImGui::SameLine();
    // 取消按键，单击后窗口隐藏
    if(ImGui::Button("cancel")){
        _should_close = true;
        _search_again = true;
    }
    
    ImGui::End();
    return file_path;
}

// 设置目标文件类型（后缀名）
// 若不限后缀名，则设置为“*”；若有多个后缀名，则后缀名间以空格进行间隔，如".jpg .jpeg"
void FileDialog::setTargetExt(const char* extlst){
    _extls.clear();
    char __extlst[100];
    strcpy(__extlst, extlst);
    const char s[2] = " ";
    char *token;

    token = strtok(__extlst, s);

    while( token != NULL ) {
        _extls.push_back(string(token));
        token = strtok(NULL, s);
    }
}

void FileDialog::display(){
    _should_close = false;
}

bool FileDialog::ifDisplay(void){
    return !_should_close;
}


// deprecated
/*
bool _getFileList(const char* dp, vector<string> *fl){
    fl->clear();
#if defined(__APPLE__) || defined(__linux__)
    DIR * dir;
    struct dirent *ent;
    if((dir = opendir(dp)) != NULL){
        while((ent = readdir(dir)) != NULL){
            if(_is_dir(ent->d_name) || _is_compl_file(ent->d_name))
                fl->push_back(ent->d_name);
        }
        closedir(dir);
        return true;
    }
    else{
        perror("");
        return false;
    }
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    //TODO: windows get file list
#endif
}

string FileDialogImGui(string *obj_name){
    string file_path = "";
        
    if(_search_again){
#if defined(__APPLE__) || defined(__linux__)
        getcwd(_cwd, 200);
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        _getcwd(cwd, 200);
#endif
        _search_again = false;
        
        if(!_getFileList(_cwd, &_filels))
            return "";
        _file_selected = 0;
    }
    
    ImGui::Begin("obj");
    ImGui::Text("Choose obj file:");
    
    if (ImGui::BeginListBox("##obj list")) {
        for (int i = 0; i < _filels.size(); ++i) {
            const bool isSelected = (_file_selected == i);
            //printf("%i\n", _is_dir(_filels[i]));
            //printf("%i\n", _is_compl_file(_filels[i]));
            if (ImGui::Selectable(_filels[i].c_str(), isSelected)) {
                _file_selected = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
    
    if(ImGui::Button("add")){
        file_path=_filels[_file_selected];
        if(_is_compl_file(file_path)){
            ImGui::End();
            _search_again = true;
            if(obj_name != NULL)
                *obj_name = file_path;
            return _cwd + string("/") + file_path;
        }
        else
            file_path = "";
    }
    
    ImGui::End();
    return file_path;
}

void setTargetExt(const char* extlst){
    _extls.clear();
    char __extlst[100];
    strcpy(__extlst, extlst);
    const char s[2] = " ";
    char *token;

    token = strtok(__extlst, s);

    while( token != NULL ) {
        //printf( "%s\n", token );
        _extls.push_back(string(token));
        token = strtok(NULL, s);
    }
}

bool _is_dir(std::string filename){
    //string filepath = _cwd + filename;
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
}

bool _is_compl_file(std::string filepath){
    if(_extls[0] == "*")
        return true;
    
    char __filepath[100];
    strcpy(__filepath, filepath.c_str());
    const char s[2] = ".";
    char *token;

    string ext = "";
    token = strtok(__filepath, s);

    while( token != NULL ) {
        ext = token;
        token = strtok(NULL, s);
    }
    
    //printf("%s\n", ext.c_str());
    for(string exti : _extls)
        //printf("%s%s\n",ext.c_str(),exti.c_str());
        if(ext == exti.substr(1))
            return true;
    
    return false;
}
*/
