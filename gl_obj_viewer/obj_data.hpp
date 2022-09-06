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

extern std::vector<float> _data_local_v;
extern std::vector<unsigned int> _data_local_f;
//extern std::vector<float> _data_local_vn;

extern Loader _data_loader;

// 用于预览模型变换的临时变换矩阵temp_trans
extern bool _data_use_temp_trans;
extern glm::mat4 _data_temp_trans;

extern unsigned int _local_pt_cnt;

void _data_removeData(int objid);
bool _data_manageData(int objcmd ,int objid, glm::mat4 transmat);
void _data_updateLocal(void);
void _data_clearAll(void);

#define getObjPath (&(_data_objpath))
#define getObjName (&(_data_objname))

#define getVtxSize(x) (((_data_objdata_v[x])->size()) * sizeof(float))
#define getVtxPtr(x) (&((*(_data_objdata_v[x]))[0]))
#define getIndexSize(x) (((_data_objdata_f[x])->size()) * sizeof(unsigned int))
#define getIndexPtr(x) (&((*(_data_objdata_f[x]))[0]))
#define getVNSize(x) (((_data_objdata_vn[x])->size()) * sizeof(float))
#define getVNPtr(x) (&((*(_data_objdata_vn[x]))[0]))
#define getPtCount(x) ((unsigned int)((_data_objdata_f[x])->size()))

#define getLocalVtxSize ((_data_local_v.size()) * sizeof(float))
#define getLocalVtxPtr (&(_data_local_v[0]))
#define getLocalIndexSize ((_data_local_f.size()) * sizeof(unsigned int))
#define getLocalIndexPtr (&(_data_local_f[0]))
//#define getLocalVNSize ((_data_local_vn.size()) * sizeof(float))
//#define getLocalVNPtr (&(_data_local_vn[0]))
#define getLocalPtCount ((unsigned int)(_local_pt_cnt))

#define getHandler (_data_manageData)

#define clearObjData _data_clearAll

#endif /* obj_data_hpp */
