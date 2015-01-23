#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <vec.h>
#include <resources.h>
using namespace DirectX;
// -------------------------------------------------------
enum TaskCode {
	TASK_PICK_OB,
	TASK_SET_LOC_OB,
	TASK_SET_V_OB,
	TASK_SET_ROT_CAM,
	TASK_SET_LOC_CAM,
	TASK_SET_V_CAM,
	TASK_CREATE_OB,
	TASK_ERASE_OB,
	TASK_DETECT_COLL,
	TASK_UPDATE_POS_OB,
	TASK_CULL_OCCL,
	TASK_DRAW,
	TASK_DEFRAGMENT_PHYS,
	TASK_DEFRAGMENT_GRAPH,
};
// -------------------------------------------------------
struct Task {
	TaskCode		code;
	uint32_t		el;
};
struct TaskPickOb {
	TaskCode		code;
	uint32_t		el;
	uint32_t		x;
	uint32_t		y;
};
struct TaskSetLocOb {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		loc;
};
struct TaskSetVOb {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		v;
};
struct TaskSetRotCam {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		rot;
};
struct TaskSetVCam {
	TaskCode		code;
	uint32_t		el;
	XMFLOAT3		v;
};
struct TaskCreateOb {
	TaskCode		code;
	uint32_t		el;
	MeshNo			mesh_hnd;
	TexNo			tex_hnd;
};
struct ResultPickOb {
	TaskCode		code;
	uint32_t		ob_hnd;
};
struct ResultCreateOb {
	TaskCode		code;
	uint32_t		ob_hnd;
};
// -------------------------------------------------------
struct Tasks {
	template<class T>
	void						insert_task(T);
	template<class T>
	void						insert_result(T);
	static Vec2<uint8_t>		task;
	static VecSparse<bool>		flg_task;
	static Vec2<uint8_t>		result;
};
template<class T>
void Tasks::insert_task(T _task) {
	task.push_back((uint8_t*)&_task, sizeof(T));
	flg_task[_task.code] = 1;
}
template<class T>
void Tasks::insert_result(T _result) {
	result.push_back((uint8_t*)&_result, sizeof(_result));
}
// -------------------------------------------------------




