//
//  obj_panel.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/16.
//

#include "obj_panel.hpp"

using std::string;
using std::vector;

ObjPanel::ObjPanel(){
    obj_selected = 0;
    show_file_dialog = false;
    
    save_elsewhere = false;
    objpathew = nullptr;
    objnameew = nullptr;
    
    file_dialog = FileDialog("obj");
    file_dialog.setTargetExt(".obj");
    
    obj_scale = 0;
    obj_x = 0;    obj_y = 0;        obj_z = 0;
    obj_yaw = 0;  obj_pitch = 0;    obj_roll = 0;
    obj_trans = glm::mat4(1);
}

void ObjPanel::Panel(void){
    if(op == NULL){
        fprintf(stderr, "must set up a handler function\n");
        return;
    }
    
    ImGui::Begin("objects");
    
    if(obj_trans != glm::mat4(1))
        ImGui::BeginDisabled();
    
    // 场景中显示的obj文件列表
    ImGui::Text("obj list");
    if (ImGui::BeginListBox("##obj list", ImVec2(320, 0))) {
        int _list_size = save_elsewhere ? (int)objnameew->size() : (int)objname.size();
        //for (int i = 0; i < objname.size(); ++i) {
        for (int i = 0; i < _list_size; ++i) {
            const bool isSelected = (obj_selected == i);
            const char * _objname = (save_elsewhere ? (*objnameew)[i].c_str() : objname[i].c_str());
            //if (ImGui::Selectable(objname[i].c_str(), isSelected)) {
            if (ImGui::Selectable(_objname, isSelected)) {
                obj_selected = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndListBox();
    }
    // 添加物体至场景
    ImGui::Spacing();ImGui::SameLine(0, 265);
    if(ImGui::Button("+", ImVec2(25, 25))){
        file_dialog.display();
    }
    ImGui::SameLine(0, 5);
    // 从场景中删除选择的物体
    if(ImGui::Button("-", ImVec2(25, 25))){
        _remove_obj();
    }
    
    if(obj_trans != glm::mat4(1))
        ImGui::EndDisabled();
    
    if((save_elsewhere ? this->objnameew->size() : this->objname.size()) < 1)
        ImGui::BeginDisabled();
    
    //ImGui::Separator();
    ImGui::Text("Translate");
    ImGui::Separator();
    ImGui::Text("x:");ImGui::SameLine(0, 60);
    if(ImGui::DragFloat("##obj_x", &this->obj_x, .01f)){
        _calc_trans_mat();
    }
    ImGui::Text("y:");ImGui::SameLine(0, 60);
    if(ImGui::DragFloat("##obj_y", &this->obj_y, .01f)){
        _calc_trans_mat();
    }
    ImGui::Text("z:");ImGui::SameLine(0, 60);
    if(ImGui::DragFloat("##obj_z", &this->obj_z, .01f)){
        _calc_trans_mat();
    }
    
    ImGui::Spacing();ImGui::Text("Scale");
    ImGui::Separator();
    ImGui::Text("scale:%.2f", pow(10, this->obj_scale));
    ImGui::Text("scale rate:");ImGui::SameLine();
    if(ImGui::SliderFloat("##obj_scale_rate", &this->obj_scale, -1.0f, 1.0f)){
    //if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(1))
        //this->obj_scale = .0f;
    //if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
        _calc_trans_mat();
    }
    
    ImGui::Spacing();ImGui::Text("Rotate");
    ImGui::Separator();
    ImGui::Text("yaw");ImGui::SameLine(0, 45);
    if(ImGui::SliderAngle("##yaw", &this->obj_yaw)){
        _calc_trans_mat();
    }
    ImGui::Text("pitch");ImGui::SameLine(0, 39);
    if(ImGui::SliderAngle("##pitch", &this->obj_pitch)){
        _calc_trans_mat();
    }
    ImGui::Text("roll");ImGui::SameLine(0, 49);
    if(ImGui::SliderAngle("##roll", &this->obj_roll)){
        _calc_trans_mat();
    }
    
    ImGui::Separator();
    ImGui::Spacing();ImGui::SameLine(0, 130);
    if(ImGui::Button("Apply", ImVec2(0, 30))){
        _trans_apply();
    }
    ImGui::SameLine();
    if(ImGui::Button("Cancel", ImVec2(0, 30))){
        _trans_can();
    }
    
    if((save_elsewhere ? this->objnameew->size() : this->objname.size()) < 1)
        ImGui::EndDisabled();
    
    ImGui::End();
    
    // 显示FileDlg，调用函数添加obj文件数据
    if(file_dialog.ifDisplay()){
        _insert_obj();
        /*
        string objfilename;
        obj_file_path = file_dialog.get(&objfilename);
        if (obj_file_path != "") {
            if(_if_dup(objfilename)){
                string _renamed_ofn = objfilename + string("(1)");
                int _sn_index = 2;
                while(_if_dup(_renamed_ofn)){
                    _renamed_ofn = objfilename + string("(") + std::to_string(_sn_index) + string(")");
                    _sn_index++;
                }
                objfilename = _renamed_ofn;
            }
            if(save_elsewhere){
                objpathew->push_back(obj_file_path);
                objnameew->push_back(objfilename);
            }
            else{
                objpath.push_back(obj_file_path);
                objname.push_back(objfilename);
            }
            op(OBJ_INSERT, (int)(save_elsewhere ? objnameew->size() : objname.size()), glm::mat4(0));
        }
        */
    }
}

void ObjPanel::SaveIn(vector<string> *objnameout, vector<string> *objpathout){
    this->objnameew = objnameout;
    this->objpathew = objpathout;
    save_elsewhere = true;
}

void ObjPanel::SetHandler(void(*op) (int objcmd ,int objid, glm::mat4 transmat)){
    this->op = op;
}

bool ObjPanel::_if_dup(std::string _objname){
    const vector<string> * _objnamels = save_elsewhere ? this->objnameew : &this->objname;
    for(string name : *_objnamels)
        if(name == _objname)
            return true;
    return false;
}

bool ObjPanel::_insert_obj(void){
    // 通过FileDlg获取obj文件名以及文件路径
    string _obj_file_name;
    obj_file_path = file_dialog.get(&_obj_file_name);
    // 若获取的文件路径非空
    if (obj_file_path != "") {
        // 处理同名情况
        if(_if_dup(_obj_file_name)){
            string _renamed_ofn = _obj_file_name + string("(1)");
            int _sn_index = 2;
            while(_if_dup(_renamed_ofn)){
                _renamed_ofn = _obj_file_name + string("(") + std::to_string(_sn_index) + string(")");
                _sn_index++;
            }
            _obj_file_name = _renamed_ofn;
        }
        // 存储获取的obj文件名以及文件路径
        if(save_elsewhere){
            objpathew->push_back(obj_file_path);
            objnameew->push_back(_obj_file_name);
        }
        else{
            objpath.push_back(obj_file_path);
            objname.push_back(_obj_file_name);
        }
        // 调用主程序中的处理函数，对obj文件数据进行读取、模型展示
        op(OBJ_INSERT, (int)(save_elsewhere ? objnameew->size() : objname.size()) - 1, glm::mat4(0));
    }
    return true;
}

bool ObjPanel::_remove_obj(void){
    if((save_elsewhere ? this->objnameew->size() : this->objname.size()) < 1)
        return false;
    // 在文件名、文件路径数组中删除选中的obj文件对应的数组元素
    if(save_elsewhere){
        this->objnameew->erase(this->objnameew->begin() + obj_selected);
        this->objpathew->erase(this->objpathew->begin() + obj_selected);
    }
    else{
        this->objname.erase(this->objname.begin() + obj_selected);
        this->objpath.erase(this->objpath.begin() + obj_selected);
    }
    // 调用主程序中的处理函数，以删除该obj文件读取的数据以及模型
    op(OBJ_REMOVE, obj_selected, glm::mat4(0));
    return true;
}

bool ObjPanel::_trans_can(void){
    obj_scale = 0;
    obj_x = 0;    obj_y = 0;        obj_z = 0;
    obj_yaw = 0;  obj_pitch = 0;    obj_roll = 0;
    obj_trans = glm::mat4(1);
    
    op(OBJ_CANCEL_TRANSFORM, obj_selected, glm::mat4(0));
    
    return false;
}

bool ObjPanel::_trans_apply(void){
    op(OBJ_CONFIRM_TRANSFORM, obj_selected, obj_trans);
    
    obj_scale = 0;
    obj_x = 0;    obj_y = 0;        obj_z = 0;
    obj_yaw = 0;  obj_pitch = 0;    obj_roll = 0;
    obj_trans = glm::mat4(1);
    
    return true;
}

void ObjPanel::_calc_trans_mat(void){
    // TODO: calculate transform matrix
    // obj_trans = glm::mat4(1);
    glm::mat4 _trans_mat(1.0f);
    _trans_mat = glm::translate(_trans_mat, glm::vec3(this->obj_x, this->obj_y, this->obj_z));
    _trans_mat = glm::rotate(_trans_mat, glm::radians(this->obj_yaw), glm::vec3(0, 0, 1));
    _trans_mat = glm::rotate(_trans_mat, glm::radians(this->obj_pitch), glm::vec3(0, 1, 0));
    _trans_mat = glm::rotate(_trans_mat, glm::radians(this->obj_roll), glm::vec3(1, 0, 0));
    float _obj_scale = pow(10, this->obj_scale);
    _trans_mat = glm::scale(_trans_mat, glm::vec3(_obj_scale, _obj_scale, _obj_scale));
    
    this->obj_trans = _trans_mat;
    // std::cout<<glm::to_string(this->obj_trans)<<std::endl;
}
