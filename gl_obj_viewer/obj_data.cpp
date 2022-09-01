//
//  obj_data.cpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/31.
//

#include "obj_data.hpp"

using std::vector;
using std::string;

vector<string> _data_objname;
vector<string> _data_objpath;

vector<vector<float>*> _data_objdata_v;
vector<vector<unsigned int>*> _data_objdata_f;
vector<vector<float>*> _data_objdata_vn;

Loader _data_loader;

bool _data_use_temp_trans = false;
glm::mat4 _data_temp_trans;

void _data_removeData(int objid){
    _data_objdata_v[objid]->clear();
    _data_objdata_f[objid]->clear();
    _data_objdata_vn[objid]->clear();
    free(_data_objdata_v[objid]);
    free(_data_objdata_f[objid]);
    free(_data_objdata_vn[objid]);
    _data_objdata_v.erase(_data_objdata_v.begin() + objid);
    _data_objdata_f.erase(_data_objdata_f.begin() + objid);
    _data_objdata_vn.erase(_data_objdata_vn.begin() + objid);
}

// 主函数obj数据数组处理函数，于obj_panel中被调用
// 用于存储读取的obj文件数据、应用用户对模型的变换操作
bool _data_manageData(int objcmd, int objid, glm::mat4 transmat){
    switch (objcmd) {
        case OBJ_INSERT:
            // 插入obj文件
            // 按照obj文件路径读取obj文件的内容
            if(_data_loader.load(_data_objpath[objid])){
                // 读取成功后获取读取的obj文件数据并存储于3个数组中
                _data_objdata_v.push_back(new vector<float>);
                _data_objdata_f.push_back(new vector<unsigned int>);
                _data_objdata_vn.push_back(new vector<float>);
                if(_data_loader.get(_data_objdata_v[objid], _data_objdata_f[objid], _data_objdata_vn[objid])){
                    printf("insert %i\n", objid);
                    /*
                    for(int i=0; i<_data_objdata_v[objid]->size(); ++i){
                        std::cout<<(*_data_objdata_v[objid])[i]<<" ";
                        if(i%3==2)
                            std::cout<<std::endl;
                    }*/
                    std::cout<<_data_objdata_v[objid]->size()/3<<std::endl;
                }else{
                    // 获取obj文件数据失败则清除先前创建的用于存储相应obj文件数据的数组
                    fprintf(stderr, "failed to get data from %i\n", objid);
                    _data_removeData(objid);
                    return false;
                }
            }else{
                // 读取obj文件失败则在文件名以及文件路径数组中删除该文件的数据
                //_data_objpath.erase(_data_objpath.begin() + objid);
                //_data_objname.erase(_data_objname.begin() + objid);
                fprintf(stderr, "invalid format!\n");
                return false;
            }
            break;
        case OBJ_REMOVE:
            printf("remove %i\n", objid);
            _data_removeData(objid);
            break;
        case OBJ_TRANSFORM:
            _data_use_temp_trans = true;
            _data_temp_trans = transmat;
            break;
        case OBJ_CANCEL_TRANSFORM:
            _data_use_temp_trans = false;
            _data_temp_trans = glm::mat4(0);
            break;
        case OBJ_CONFIRM_TRANSFORM:
            //TODO: apply transform to objdata
            _data_use_temp_trans = false;
            _data_temp_trans = glm::mat4(0);
            break;
            
        default:
            printf("?\n");
            break;
    }
    return true;
}
