#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <vec.h>
using namespace DirectX;
// -------------------------------------------------------
enum TaskCode {
	TASK_OB_POS,
	TASK_OB_V,
	TASK_CAM_V,
	TASK_CAM_ROT,
	TASK_OB_PICK,
	TASK_OB_CREATE,
	TASK_OB_ERASE,
	TASK_CAM_UPDATE_POS,
	TASK_CAM_UPDATE,
	TASK_OB_SORT,
	TASK_WORLD_UPDATE,
	TASK_WVP_UPDATE,
	TASK_BBOX_UPDATE,
	TASK_OCCL_CULL,
	TASK_DRAW,
	TASK_PHYS_DEFRAG,
	TASK_GRAPH_DEFRAG,
};
enum MeshNo{
	MESH_TRI,
	MESH_RECT,
	MESH_DIAMENT,
};
enum TexNo{
	TEX_TRI,
	TEX_RECT,
	TEX_DIAMENT,
};
// -------------------------------------------------------
struct ResultObCreate {
	TaskCode		code;
	uint32_t		hnd_ob;
};
struct ResultObPick {
	TaskCode		code;
	uint32_t		hnd_ob;
};
struct Task {
	TaskCode		code;
	uint32_t		el;
};
struct TaskObCreate {
	TaskCode		code;
	uint32_t		el;
	MeshNo			hnd_mesh;
	TexNo			hnd_tex;
};
struct TaskObPos {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		pos;
};
struct TaksObV {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		v;
};
struct TaskCamV {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		v;
};
struct TaskCamRot{
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		angles;
};
struct TaskObPick {
	TaskCode		code;
	uint32_t		el;
	uint32_t		x;
	uint32_t		y;
};
// -------------------------------------------------------
struct Tasks {
	template<class T>
	void						insert_task(T);
	static Vec2<uint8_t>		task;
	static VecSparse<bool>		flg_task;
	static Vec2<uint8_t>		result;
};
template<class T>
void Tasks::insert_task(T _t) {
	task.push_back((uint8_t*)&_t, sizeof(T));
	flg_task[_t.code] = 1;
}
// -------------------------------------------------------




