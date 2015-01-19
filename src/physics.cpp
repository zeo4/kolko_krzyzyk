#pragma once
#include <physics.h>
// -------------------------------------------------------
void Physics::do_tasks() {
	if(task.get_col_size() > 1) {
		uint32_t* _mapa = 0;
		task.sort_comp(_mapa);
		task.sort_exe(_mapa);
		task.erase_dupl_comp(_mapa, FunHasz<uint8_t>(), FunHasz2<uint8_t>());
		task.sort_exe(_mapa);
		free(_mapa);
	}

	for(uint32_t _i = 0; _i < task.get_col_size(); ++_i) {
		if(task.get_row(_i) == task.empty) continue;

		switch(((Task*)task[_i])->code) {
		case TASK_PICK_OB: pick_ob(_i); break;
		case TASK_SET_LOC_OB: set_loc_ob(_i); break;
		case TASK_SET_V_OB: set_v_ob(_i); break;
		case TASK_SET_ROT_CAM: set_rot_cam(_i); break;
		case TASK_SET_V_CAM: set_v_cam(_i); break;
		case TASK_CREATE_OB: create_ob(_i); break;
		}
	}
}
void Physics::pick_ob(uint32_t const _i_task) {
	TaskPickOb _task = *(TaskPickOb*)task[_i_task];
	ResultPickOb _w;
	_w.code = TASK_PICK_OB;

	XMVECTOR _loc, _loc1, _dir, _dir1;
	compute_ray_click(_loc, _dir, _task.x, _task.y);
	float _t = 1000.0f, _t1;
	uint32_t _hnd_pick = 0x80000000;
	for(uint32_t _hnd_ob = 0; _hnd_ob < data_e.no.wez_poj(); ++_hnd_ob) {
		if(data_e.no.sprawdz_pusty(_hnd_ob)) continue;
		_loc1 = XMVector3TransformCoord(_loc, XMMatrixInverse(
			&XMVectorSet(0,0,0,0), XMLoadFloat4x4(&data_e.mtx_world[data_e.no[_hnd_ob]])
		));
		_dir1 = XMVector3TransformNormal(_dir, XMMatrixInverse(
			&XMVectorSet(0,0,0,0), XMLoadFloat4x4(&data_e.mtx_world[data_e.no[_hnd_ob]])
		));
		_t1 = compute_ray_ob(_dir1, _dir1, data_e.no[_hnd_ob]);
		if(_t1 < _t) {
			_t = _t1;
			_hnd_pick = _hnd_ob;
		}
	}
	_w.ob_hnd = _hnd_pick;

	result.push_back((uint8_t*)&_w, sizeof(_w));
	task.erase(_i_task);
}
void Physics::compute_ray_click(XMVECTOR& _pocz, XMVECTOR& _kier, uint32_t const& _x, uint32_t const& _y) const {
	// licz początek
	_pocz = XMLoadFloat3(&cam.loc);

	// licz kierunek
	RECT _kwadr;
	GetClientRect(uch_okno, &_kwadr);
	_kier = XMVectorSet(
		(float(2 * _x) / _kwadr.right - 1) / cam.mtx_proj._11,
		(- float(2 * _y) / _kwadr.bottom + 1) / cam.mtx_proj._22,
		1.0f,
		0
	);
}
float Physics::compute_ray_ob(XMVECTOR const& _pocz, XMVECTOR const& _kier, uint32_t const& _nr_ob) const {
	XMVECTOR _w0, _w1, _w2;
	uint32_t const _uch_mod = data_e.mesh_hnd[_nr_ob];
	float _t = 1000.0f, _t1;
	for(uint32_t _i = 0; _i < data_e.mesh.get_ind_row(_uch_mod).drug; _i += 3) {
		_w0 = XMLoadFloat3(data_e.mesh.get_vert(_uch_mod) + data_e.mesh.get_ind(_uch_mod)[_i]);
		_w1 = XMLoadFloat3(data_e.mesh.get_vert(_uch_mod) + data_e.mesh.get_ind(_uch_mod)[_i+1]);
		_w2 = XMLoadFloat3(data_e.mesh.get_vert(_uch_mod) + data_e.mesh.get_ind(_uch_mod)[_i+2]);
		_t1 = compute_ray_tri(_pocz, _kier, _w0, _w1, _w2);
		if(_t1 < _t) _t = _t1;
	}
	return _t;
}
float Physics::compute_ray_tri(XMVECTOR const& _pocz, XMVECTOR const& _kier, CXMVECTOR const& _w0, CXMVECTOR const& _w1, CXMVECTOR const& _w2) const {
	//------------------WZORY-------------------------
	// pkt promienia r(t) = pocz + t*kier
	// wekt1 = w1 - w0, wekt2 = w2 - w0
	// pkt trojkata T(u,v) = w0 + u*wekt1 + v*wekt2, dla u >= 0, v >= 0, u+v <= 1
	// m = pocz - w0
	// t = wekt2 * (m x wekt1) / wekt1 * (kier x wekt2)
	// u = m * (kier x wekt2) / wekt1 * (kier x wekt2)
	// v = kier * (m x wekt1) / wekt1 * (kier x wekt2)
	//------------------------------------------------
	XMVECTOR _wekt1 = _w1 - _w0;
	XMVECTOR _wekt2 = _w2 - _w0;
	XMVECTOR _m = _pocz - _w0;
	XMVECTOR _kxwekt2 = XMVector3Cross(_kier, _wekt2);
	XMVECTOR _mxwekt1 = XMVector3Cross(_m, _wekt1);
	XMVECTOR _U = XMVector3Dot(_m, _kxwekt2) / XMVector3Dot(_wekt1, _kxwekt2);
	XMVECTOR _V = XMVector3Dot(_kier, _mxwekt1) / XMVector3Dot(_wekt1, _kxwekt2);
	float _u = XMVectorGetX(_U);
	float _v = XMVectorGetX(_V);

	// jeśli kolizja
	if(_u >= 0 && _v >= 0 && _u + _v <= 1) {
		XMVECTOR _T = XMVector3Dot(_wekt2, _mxwekt1) / XMVector3Dot(_wekt1, _kxwekt2);
		return XMVectorGetX(_T);
	} else {
		return 1000.0f;
	}
}
void Physics::set_loc_ob(uint32_t const _i_task) {
	TaskSetLocOb _task = *(TaskSetLocOb*)task[_i_task];
	data_e.loc[data_e.no[_task.el]] = _task.loc;
	task.erase(_i_task);
}
void Physics::set_v_ob(uint32_t const _i_task) {
	TaskSetVOb _task = *(TaskSetVOb*)task[_i_task];
	data_e.v[data_e.no[_task.el]] = _task.v;
	task.erase(_i_task);
}
void Physics::set_rot_cam(uint32_t const _i_task) {
	XMFLOAT3 _rot = ((TaskSetRotCam*)task[_i_task])->rot;

	// obrót poziomy
	XMVECTOR _q_rot = XMQuaternionRotationRollPitchYaw(0.0f, _rot.y, 0.0f);
	XMVECTOR _q_cam = XMLoadFloat4(&cam.q);
	_q_cam = XMQuaternionMultiply(_q_cam, _q_rot);

	// obrót pionowy
	_q_rot = XMQuaternionRotationRollPitchYaw(_rot.x, 0.0f, 0.0f);
	XMStoreFloat4(&cam.q, XMQuaternionMultiply(_q_rot, _q_cam));

	task.erase(_i_task);
}
void Physics::set_loc_cam(uint32_t const _i_task) {
}
void Physics::set_v_cam(uint32_t const _i_task) {
	XMFLOAT3 _v = ((TaskSetVCam*)task[_i_task])->v;
	if(_v.x && _v.y) cam.v.z = _v.z;
	else if(_v.x && _v.z) cam.v.y = _v.y;
	else if(_v.y && _v.z) cam.v.x = _v.x;
	else if(_v.x) cam.v.x = _v.x;
	else if(_v.y) cam.v.y = _v.y;
	else if(_v.z) cam.v.z = _v.z;
	task.erase(_i_task);
}
void Physics::create_ob(uint32_t const _i_task) {
	TaskCreateOb _task = *(TaskCreateOb*)task[_i_task];
	data_e.loc.push_back(XMFLOAT3(0.0f, 0.0f, 0.5f));
	data_e.v.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	data_e.mtx_world.push_back(XMFLOAT4X4());
	XMStoreFloat4x4(
		&data_e.mtx_world[data_e.mtx_world.get_size()-1], XMMatrixIdentity()
	);
	data_e.mtx_wvp.push_back(XMFLOAT4X4());
	XMStoreFloat4x4(
		&data_e.mtx_wvp[data_e.mtx_wvp.get_size()-1], XMMatrixIdentity()
	);
	XMFLOAT3 _bbox[] = {
		XMFLOAT3(-1.0f, -1.0f, -1.0f),
		XMFLOAT3(-1.0f, 1.0f, -1.0f),
		XMFLOAT3(1.0f, 1.0f, -1.0f),
		XMFLOAT3(1.0f, -1.0f, -1.0f),
		XMFLOAT3(-1.0f, -1.0f, 1.0f),
		XMFLOAT3(-1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, -1.0f, 1.0f),
	};
	data_e.bbox_local.push_back(_bbox, 8);
	data_e.bbox_scr.push_back(_bbox, 8);
	data_e.occluder.push_back(false);
	data_e.mesh_hnd.push_back(_task.mesh_hnd);
	data_e.tex_hnd.push_back(_task.tex_hnd);
	data_e.mesh.create(_task.mesh_hnd);
	data_e.tex.create(_task.tex_hnd);
	insert_result(
		ResultCreateOb{TASK_CREATE_OB, data_e.no.wstaw(data_e.loc.get_size()-1)}
	);
	task.erase(_i_task);
}
void Physics::detect_coll(uint32_t const _i_task) {
	// collision detection here
	for(uint32_t _i = 0; _i < data_e.t_coll.get_size(); ++_i) {
		data_e.t_coll[_i] = 1.0f;
	}

	task.erase(_i_task);
}
// -------------------------------------------------------



