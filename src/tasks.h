#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <vec.h>
using namespace DirectX;
// -------------------------------------------------------
enum TaskCode {
	OB_POS,
	OB_V,
	CAM_V,
	CAM_ROT,
	OB_PICK,
	OB_CREATE,
	OB_ERASE,
	CAM_UPDATE_POS,
	CAM_UPDATE,
	OB_SORT,
	WORLD_UPDATE,
	OCCLU_ERASE,
	DRAW,
	PHYS_DEFRAG,
	GRAPH_DEFRAG,
};
// -------------------------------------------------------
struct ResultObCreate {
	TaskCode		kod_zad;
	uint32_t		uch_ob;
};
struct ResultObPick {
	TaskCode		kod_zad;
	uint32_t		uch_ob;
};
struct Task {
	TaskCode		code;
	uint32_t		el;
};
struct TaskObCreate {
	TaskCode		code;
	uint32_t		el;
	uint32_t		hnd_mesh;
	uint32_t		hnd_tex;
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




