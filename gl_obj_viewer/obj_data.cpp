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
std::vector<glm::vec3> _data_objdata_center;

vector<float> _data_local_v;
vector<unsigned int> _data_local_f;
//vector<float> _data_local_vn;

Loader _data_loader;

bool _data_use_temp_trans = false;
glm::mat4 _data_temp_trans;
bool _data_is_transforming = false;

unsigned int _local_pt_cnt = 0;

float _data_coord[636] = {0};

// 从场景中删除指定模型
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
    _data_objdata_center.erase(_data_objdata_center.begin() + objid);
    _data_updateLocal();
}

// 主函数obj数据数组处理函数，于obj_panel中被调用
// 用于存储读取的obj文件数据、应用用户对模型的变换操作
bool _data_manageData(int objcmd, int objid, glm::mat4 transmat){
    switch (objcmd) {
        case OBJ_STATIC:
            break;
        case OBJ_INSERT:
            // 插入obj文件
            // 按照obj文件路径读取obj文件的内容
            if(_data_loader.load(_data_objpath[objid], false)){
                // 读取成功后获取读取的obj文件数据并存储于3个数组中
                _data_objdata_v.push_back(new vector<float>);
                _data_objdata_f.push_back(new vector<unsigned int>);
                _data_objdata_vn.push_back(new vector<float>);
                _data_objdata_center.push_back(glm::vec3(.0f, .0f, .0f));
                if(_data_loader.get(_data_objdata_v[objid], _data_objdata_f[objid], _data_objdata_vn[objid])){
                    printf("insert %i\n", objid);
                    _data_is_transforming = true;
                }else{
                    // 获取obj文件数据失败则清除先前创建的用于存储相应obj文件数据的数组
                    fprintf(stderr, "failed to get data from %i\n", objid);
                    _data_removeData(objid);
                    return false;
                }
            }else{
                // 读取obj文件失败则在文件名以及文件路径数组中删除该文件的数据
                fprintf(stderr, "invalid format!\n");
                return false;
            }
            break;
        case OBJ_REMOVE:
            printf("remove %i\n", objid);
            _data_removeData(objid);
            _data_is_transforming = true;
            break;
        case OBJ_TRANSFORM:
            _data_use_temp_trans = true;
            _data_temp_trans = transmat;
            _data_is_transforming = true;
            //TODO: 位移、缩放操作预览
            printf("transforming %i\n", objid);
            break;
        case OBJ_CANCEL_TRANSFORM:
            _data_use_temp_trans = false;
            _data_temp_trans = glm::mat4(0);
            _data_is_transforming = true;
            break;
        case OBJ_CONFIRM_TRANSFORM:
            //TODO: apply transform to objdata
            _data_use_temp_trans = false;
            _data_useTransform(objid);
            break;
            
        default:
            printf("?\n");
            break;
    }
    _data_updateLocal(objid);
    return true;
}

void _data_updateLocal(int transid){
    _local_pt_cnt = 0;
    /*
    _data_local_v.clear();
    for(int objid = 0; objid<_data_objdata_v.size(); objid++)
        for(int vid = 0; vid<_data_objdata_v[objid]->size(); vid++)
            _data_local_v.push_back((*(_data_objdata_v[objid]))[vid]);
    _data_local_f.clear();
     */
    _data_local_v.clear();
    if(_data_use_temp_trans){
        for(int objid = 0; objid<_data_objdata_v.size(); objid++){
            if(objid == transid){
                for(int vid = 0; vid<_data_objdata_v[objid]->size() / 3; vid++ ){
                    glm::vec4 former_v((*(_data_objdata_v[objid]))[3 * vid],
                                       (*(_data_objdata_v[objid]))[3 * vid + 1],
                                       (*(_data_objdata_v[objid]))[3 * vid + 2], 1.0f);
                    glm::vec4 trans_v = former_v * _data_temp_trans;
                    glm::vec3 former_vn((*(_data_objdata_vn[objid]))[3 * vid],
                                       (*(_data_objdata_vn[objid]))[3 * vid + 1],
                                       (*(_data_objdata_vn[objid]))[3 * vid + 2]);
                    glm::vec3 trans_vn = glm::normalize(glm::vec3(former_vn * glm::mat3(_data_temp_trans)));
                    _data_local_v.push_back(trans_v[0]);
                    _data_local_v.push_back(trans_v[1]);
                    _data_local_v.push_back(trans_v[2]);
                    _data_local_v.push_back(trans_vn[0]);
                    _data_local_v.push_back(trans_vn[1]);
                    _data_local_v.push_back(trans_vn[2]);
                }
            }else{
                for(int vid = 0; vid<_data_objdata_v[objid]->size() / 3; vid++ ){
                    _data_local_v.push_back((*(_data_objdata_v[objid]))[3 * vid]);
                    _data_local_v.push_back((*(_data_objdata_v[objid]))[3 * vid + 1]);
                    _data_local_v.push_back((*(_data_objdata_v[objid]))[3 * vid + 2]);
                    _data_local_v.push_back((*(_data_objdata_vn[objid]))[3 * vid]);
                    _data_local_v.push_back((*(_data_objdata_vn[objid]))[3 * vid + 1]);
                    _data_local_v.push_back((*(_data_objdata_vn[objid]))[3 * vid + 2]);
                }
            }
        }
    }
    else{
        for(int objid = 0; objid<_data_objdata_v.size(); objid++){
            for(int vid = 0; vid<_data_objdata_v[objid]->size() / 3; vid++ ){
                _data_local_v.push_back((*(_data_objdata_v[objid]))[3 * vid]);
                _data_local_v.push_back((*(_data_objdata_v[objid]))[3 * vid + 1]);
                _data_local_v.push_back((*(_data_objdata_v[objid]))[3 * vid + 2]);
                _data_local_v.push_back((*(_data_objdata_vn[objid]))[3 * vid]);
                _data_local_v.push_back((*(_data_objdata_vn[objid]))[3 * vid + 1]);
                _data_local_v.push_back((*(_data_objdata_vn[objid]))[3 * vid + 2]);
            }
        }
    }
    _data_local_f.clear();
    
    unsigned int fid_start = 0;
    for(int objid = 0; objid<_data_objdata_f.size(); objid++){
        if(objid > 0)
            fid_start += _data_objdata_v[objid - 1]->size() / 3;
        for(int fid = 0; fid<_data_objdata_f[objid]->size(); fid++)
            _data_local_f.push_back((*(_data_objdata_f[objid]))[fid] + fid_start);
    }
    /*
    _data_local_vn.clear();
    for(int objid = 0; objid<_data_objdata_vn.size(); objid++)
        for(int vnid = 0; vnid<_data_objdata_vn[objid]->size(); vnid++)
            _data_local_vn.push_back((*(_data_objdata_vn[objid]))[vnid]);
     */
    _local_pt_cnt = (unsigned int)_data_local_f.size();
}

// 应用变换，将临时变换矩阵与相应模型坐标相乘，并清空变换矩阵
void _data_useTransform(int objid){
    for(int vid = 0; vid<_data_objdata_v[objid]->size() / 3; vid++ ){
        glm::vec4 former_v((*(_data_objdata_v[objid]))[3 * vid],
                           (*(_data_objdata_v[objid]))[3 * vid + 1],
                           (*(_data_objdata_v[objid]))[3 * vid + 2], 1.0f);
        glm::vec4 trans_v = former_v * _data_temp_trans;
        glm::vec3 former_vn((*(_data_objdata_vn[objid]))[3 * vid],
                           (*(_data_objdata_vn[objid]))[3 * vid + 1],
                           (*(_data_objdata_vn[objid]))[3 * vid + 2]);
        glm::vec3 trans_vn = glm::normalize(glm::vec3(former_vn * glm::mat3(_data_temp_trans)));
        (*_data_objdata_v[objid])[3 * vid] = trans_v[0];
        (*_data_objdata_v[objid])[3 * vid + 1] = trans_v[1];
        (*_data_objdata_v[objid])[3 * vid + 2] = trans_v[2];
        (*_data_objdata_vn[objid])[3 * vid] = trans_vn[0];
        (*_data_objdata_vn[objid])[3 * vid + 1] = trans_vn[1];
        (*_data_objdata_vn[objid])[3 * vid + 2] = trans_vn[2];
    }
    _data_temp_trans = glm::mat4(0);
    _data_is_transforming = true;
}

void _data_TransComplete(void){
    _data_is_transforming = false;
}

void _data_genCoord(int fov){
    int _cid = 0;
    for (float _coordx = -1.0f; _coordx < 1.1f; _coordx += .1f) {
        _data_coord[3 * _cid++] = _coordx * fov;
        _data_coord[3 * _cid] = _coordx * fov;
        _data_coord[3 * _cid++ + 1] = 1.0f * fov;
        _data_coord[3 * _cid] = _coordx * fov;
        _data_coord[3 * _cid++ + 2] = -1.0f * fov;
        _data_coord[3 * _cid] = _coordx * fov;
        _data_coord[3 * _cid++ + 2] = 1.0f * fov;
    }

    for (float _coordy = .0f; _coordy < 1.1f; _coordy += .1f) {
        _data_coord[3 * _cid] = -1.0f * fov;
        _data_coord[3 * _cid++ + 1] = _coordy * fov;
        _data_coord[3 * _cid] = 1.0f * fov;
        _data_coord[3 * _cid++ + 1] = _coordy * fov;
        _data_coord[3 * _cid + 1] = _coordy * fov;
        _data_coord[3 * _cid++ + 2] = -1.0f * fov;
        _data_coord[3 * _cid + 1] = _coordy * fov;
        _data_coord[3 * _cid++ + 2] = 1.0f * fov;
    }
    
    for (float _coordz = -1.0f; _coordz < 1.1f; _coordz += .1f){
        _data_coord[3 * _cid] = -1.0f * fov;
        _data_coord[3 * _cid++ + 2] = _coordz * fov;
        _data_coord[3 * _cid] = 1.0f * fov;
        _data_coord[3 * _cid++ + 2] = _coordz * fov;
        _data_coord[3 * _cid++ + 2] = _coordz * fov;
        _data_coord[3 * _cid + 1] = 1.0f * fov;
        _data_coord[3 * _cid++ + 2] = _coordz * fov;
    }
}

void _data_clearAll(void){
    int objid = (int)_data_objname.size() - 1;
    while(objid >= 0){
        _data_removeData(objid--);
    }
}
