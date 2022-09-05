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
        // 分析读取的数据类型，并存储于相应数组中，若数据类型错误，则退出
        if(!hdl_obj_data(_data)){
            delete []_dline;
            reader.close();
            this->clear();
            return false;
        }
        delete []_dline;
        _data.clear();
        file_cont.clear();
    }
    reader.close();
    if(!this->has_vn || data_vn.size() != data_v.size())
        calc_vn();
    return true;
}

// 对于read_obj_file读取的不同类型的数据，将其存储于相应的数组中
bool Loader::hdl_obj_data(const vector<string>& obj_data){
    if(obj_data.size() < 4)
        return false;
    string _data_type = obj_data[0];
    // 存储顶点坐标数据
    if(_data_type == "v"){
        this->data_v.push_back(std::atof(obj_data[1].c_str()));
        this->data_v.push_back(std::atof(obj_data[2].c_str()));
        this->data_v.push_back(std::atof(obj_data[3].c_str()));
    }else{
        // 存储三角面片数据
        if(_data_type == "f"){
            this->data_f.push_back(std::atoi(obj_data[1].c_str()) - 1);
            this->data_f.push_back(std::atoi(obj_data[2].c_str()) - 1);
            this->data_f.push_back(std::atoi(obj_data[3].c_str()) - 1);
        }else{
            // 存储顶点法向量数据
            if(_data_type == "vn"){
                this->has_vn = true;
                this->data_vn.push_back(std::atof(obj_data[1].c_str()));
                this->data_vn.push_back(std::atof(obj_data[2].c_str()));
                this->data_vn.push_back(std::atof(obj_data[3].c_str()));
            }else{
                // 忽略贴图坐标数据
                if(_data_type == "vt" || _data_type == "g")
                    return true;
                else
                    return false;
            }
        }
    }
    
    return true;
}

// 计算顶点法向量
bool Loader::calc_vn(void){
    //vector<float> tdata_fn;
    data_vn = vector<float>(data_v.size(), .0f);
    // 遍历data_f数组，取得每一个面的三个顶点的坐标数据
    for(int fid = 0; fid < data_f.size() / 3; fid++){
        glm::vec3 vtx_a = get_coord(data_f[fid * 3]);
        glm::vec3 vtx_b = get_coord(data_f[fid * 3 + 1]);
        glm::vec3 vtx_c = get_coord(data_f[fid * 3 + 2]);
        // 计算面的法向量
        glm::vec3 _fn = glm::normalize(glm::cross(vtx_c - vtx_a, vtx_b - vtx_a));
        //tdata_fn.push_back(_fn[0]);
        //tdata_fn.push_back(_fn[1]);
        //tdata_fn.push_back(_fn[2]);
        // 将面的法向量累加到该面的3个顶点的法向量
        for (int vnvi = 0; vnvi < 3; vnvi++)
            for (int vnci = 0; vnci < 3; vnci++)
                data_vn[3 * data_f[fid * 3 + vnvi] + vnci] += _fn[vnci];
            //data_vn[3 * data_f[fid * 3 + vnvi]] += _fn[0];
            //data_vn[3 * data_f[fid * 3 + vnvi] + 1] += _fn[1];
            //data_vn[3 * data_f[fid * 3 + vnvi] + 2] += _fn[2];
    }
    /*if(tdata_fn.size() != data_f.size())
    //    return false;
    data_vn.clear();*/
    // 将所有顶点法向量标准化
    for (int vid = 0; vid < data_vn.size() / 3; vid++) {
        glm::vec3 vtx_norm = glm::normalize(glm::vec3(data_vn[vid * 3], data_vn[vid * 3 + 1], data_vn[vid * 3 + 2]));
        data_vn[vid * 3] = vtx_norm[0];
        data_vn[vid * 3 + 1] = vtx_norm[1];
        data_vn[vid * 3 + 2] = vtx_norm[2];
    }
    //for(int vid = 0; vid < data_v.size() / 3; vid++){
    //    for(int coordid)
    //}
    return true;
}

// 将计算所得的法向量写入原obj文件中
bool Loader::ovr_vn(void){
    // 若原文件中已存在法向量数据，则不作修改
    if(has_vn)
        return true;
    //if(!calc_vn())
        //return false;
    // 将calc_vn函数计算所得的顶点法向量写入obj文件中
    std::fstream writer;
    writer.open(obj_path.c_str(), std::ios::app | std::ios::out);
    if(!writer.is_open()){return false;}
    for(int vnid = 0; vnid < data_vn.size() / 3; vnid++){
        writer << "vn " <<data_vn[vnid * 3] << " " << data_vn[vnid * 3 + 1]
            << " " << data_vn[vnid * 3 + 2] << std::endl;
    }
    writer.close();
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
    else{
        this->clear();
        return false;
    }
}

// 向外部数组main_data写入读取的obj文件数据
/*
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
*/

// 获取obj文件的顶点坐标、三角面片、顶点法向量数据
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

glm::vec3 Loader::get_coord(int vid){
    if(vid >= data_v.size() / 3)
        return glm::vec3(1);
    return glm::vec3(data_v[vid * 3], data_v[vid * 3 + 1], data_v[vid * 3 + 2]);
}
