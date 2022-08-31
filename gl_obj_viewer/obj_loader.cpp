//
//  obj_loader.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/26.
//

#include "obj_loader.hpp"

using std::string;
using std::vector;

// 读取obj文件，并存储至类内部的数组data_v, data_f与data_vn中
bool Loader::read_obj_file(void){
    std::fstream reader;
    reader.open(obj_path.c_str(), std::ios::in);
    if(!reader.is_open()){return false;}
    // TODO: 读取obj文件的顶点v, 面f, 顶点法向量vn数据存储至相应数组
    // 逐行读取obj文件数据
    string file_cont;
    vector<string> _data;
    while(std::getline(reader, file_cont)){
        if(file_cont == "")
            continue;
        file_cont = file_cont.substr(file_cont.find_first_not_of(" "));
        // 跳过以#开头的注释行
        if(file_cont[0] == '#')
            continue;
        char* _dline = new char[file_cont.size() + 1];//(char*)malloc((file_cont.size() + 1) * sizeof(char));
        strcpy(_dline, file_cont.c_str());
        // 将读取的数据以空格进行分割，并存储于临时数组中
        char* _line_data = strtok(_dline, " ");
        while(_line_data != NULL){
            _data.push_back(_line_data);
            _line_data = strtok(NULL, " ");
        }
        if(!hdl_obj_data(_data))
            return false;
        delete []_dline;
        _data.clear();
        file_cont.clear();
    }
    return true;
}

bool Loader::hdl_obj_data(const vector<string>& obj_data){
    if(obj_data.size() < 4)
        return false;
    string _data_type = obj_data[0];
    if(_data_type == "v"){
        this->data_v.push_back(std::atof(obj_data[1].c_str()));
        this->data_v.push_back(std::atof(obj_data[2].c_str()));
        this->data_v.push_back(std::atof(obj_data[3].c_str()));
    }else{
        if(_data_type == "f"){
            this->data_f.push_back(std::atoi(obj_data[1].c_str()));
            this->data_f.push_back(std::atoi(obj_data[2].c_str()));
            this->data_f.push_back(std::atoi(obj_data[3].c_str()));
        }else{
            if(_data_type == "vn"){
                this->has_vn = true;
                this->data_vn.push_back(std::atof(obj_data[1].c_str()));
                this->data_vn.push_back(std::atof(obj_data[2].c_str()));
                this->data_vn.push_back(std::atof(obj_data[3].c_str()));
            }else{
                return false;
            }
        }
    }
    
    return true;
}

bool Loader::ovr_vn(void){
    return true;
}

// 读取obj文件，外部调用
bool Loader::load(string obj_file_path, bool vn_ovr){
    if(obj_file_path == "") {return false;}
    this->obj_path = obj_file_path;
    // 调用私有函数read_obj_file读取obj文件
    if(read_obj_file()){
        if(vn_ovr)
            // 若需要在原文件覆写计算得到的顶点法向量，则调用ovr_vn函数
            return ovr_vn();
        else
            return true;
    }
    else
        return false;
}

// 向外部数组main_data写入读取的obj文件数据
bool Loader::get(std::vector<float>* main_data){
    if(main_data == NULL){this->clear(); return false;}
    for(float v_coord : this->data_v)
        main_data->push_back(v_coord);
    // TODO: 写入面f与顶点法向量vn数据
    for (unsigned int f_id : this->data_f)
        main_data->push_back((float)f_id);
    if (this->data_vn.size() > 0)
        for (float v_norm : this->data_vn)
            main_data->push_back(v_norm);
    this->clear();
    return true;
}

bool Loader::get(vector<float> *main_data_v, vector<unsigned int> *main_data_f,
                 vector<float> *main_data_vn) {
    if (main_data_v == NULL || main_data_f == NULL) {
        this->clear();
        return false;
    }
    for (float v_coord : this->data_v)
        main_data_v->push_back(v_coord);
    for (unsigned int f_id : this->data_f)
        main_data_f->push_back(f_id);
    if (main_data_vn != NULL && this->data_vn.size() > 0)
        for (float v_norm : this->data_vn)
            main_data_vn->push_back(v_norm);
    this->clear();
    return true;
}

void Loader::clear(void){
    this->obj_path = "";
    this->has_vn = false;
    this->data_v.clear();
    this->data_f.clear();
    this->data_vn.clear();
}
