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
    if(main_data == NULL){return false;}
    for(float v_coord : this->data_v)
        main_data->push_back(v_coord);
    // TODO: 写入面f与顶点法向量vn数据
    
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
