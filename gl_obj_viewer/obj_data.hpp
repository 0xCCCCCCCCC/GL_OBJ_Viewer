//
//  obj_data.hpp
//  gl_obj_viewer
//
//  Created by YNK on 2022/8/31.
//

#ifndef obj_data_hpp
#define obj_data_hpp

#include <iostream>
#include <string>
#include <vector>

#include "glm.hpp"

#include "obj_loader.hpp"
#include "obj_panel.hpp"

extern std::vector<std::string> _data_objname;
extern std::vector<std::string> _data_objpath;

extern std::vector<std::vector<float>*> _data_objdata_v;
extern std::vector<std::vector<unsigned int>*> _data_objdata_f;
extern std::vector<std::vector<float>*> _data_objdata_vn;

extern Loader _data_loader;

// 用于预览模型变换的临时变换矩阵temp_trans
extern bool _data_use_temp_trans;
extern glm::mat4 _data_temp_trans;

void _data_removeData(int objid);
void _data_manageData(int objcmd ,int objid, glm::mat4 transmat);

#define getObjPath (&(_data_objpath))
#define getObjName (&(_data_objname))

#define getVtxSize (((_data_objdata_v).size()) * sizeof(float))
#define getVtxPtr (&(_data_objdata_v[0]))
#define getIndexSize (((_data_objdata_f).size()) * sizeof(unsigned int))
#define getIndexPtr (&(_data_objdata_f[0]))
#define getVNSize (((_data_objdata_vn).size()) * sizeof(float))
#define getVNPtr (&(_data_objdata_vn[0]))

#define getHandler (_data_manageData)

#endif /* obj_data_hpp */
