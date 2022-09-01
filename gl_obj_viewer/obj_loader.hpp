//
//  obj_loader.hpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/26.
//

#ifndef obj_loader_hpp
#define obj_loader_hpp

#include <iostream>
#include <fstream>
#include <vector>

#include "glm.hpp"

class Loader{
private:
    std::string obj_path;
    bool has_vn;
    
    std::vector<float> data_v;
    std::vector<unsigned int> data_f;
    std::vector<float> data_vn;
    
    bool read_obj_file(void);
    bool calc_vn(void);
    bool ovr_vn(void);
    
    bool hdl_obj_data(const std::vector<std::string>& obj_data);
    
    glm::vec3 get_coord(int vid);
    
public:
    Loader(void): obj_path(""), has_vn(false){}
    bool load(std::string obj_file_path, bool vn_ovr = true);
    //bool get(std::vector<float>* main_data);
    bool get(std::vector<float> *main_data_v, std::vector<unsigned int> *main_data_f,
             std::vector<float> *main_data_vn);
    void clear(void);
};

#endif /* obj_loader_hpp */
