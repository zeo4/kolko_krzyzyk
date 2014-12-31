#pragma once

#include <debugs.h>
#include <graphics.h>
#include <handles.h>
// -------------------------------------------------------
void Graphics::create_occlu_map() {
	// bounding boxes to graph device
	res.ob.update_vert(g_par.ob.bbox[0], g_par.ob.bbox.get_size());

	devctx->IASetInputLayout(res.in_lay[IN_F4F44]);
	res.ob.bind_vert(VS_TFORM);
	devctx->VSSetShader(res.vs[VS_TFORM], 0, 0);
	devctx->IASetIndexBuffer(res.ob.ind_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->PSSetShader(res.ps[PS_WRITE_DEPTH], 0, 0);
	devctx->OMSetRenderTargets(1, &res.back_buf_rtv, res.ds_dsv);
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	for(uint32_t _i = 0; _i < g_par.ob.bbox.get_col_size(); ++_i) {

	}
	//draw_ob(g_par.ob.mesh_hnd, g_par.ob.tex_hnd, &g_par.ob.mesh, &g_par.ob.tex);
	chain->Present(0, 0);
}
void Graphics::draw_ob(Vec<uint32_t>const& _hnd_mesh, Vec<uint32_t>const& _hnd_tex, Meshes*const _mesh, Textures*const _tex) const {
	uint32_t _i, _il_kopie = 1, _il_wyrys = 0;
	for(_i = 1; _i < _hnd_mesh.get_size(); ++_i) {
		if(_hnd_mesh[_i-1] == _hnd_mesh[_i] && _hnd_tex[_i-1] == _hnd_tex[_i]) {
			++_il_kopie;
		} else {
			if(_tex != 0) devctx->PSSetShaderResources(0, 1, &_tex->get(_hnd_tex[_i-1]));
			devctx->DrawIndexedInstanced(
				_mesh->get_ind_row(_hnd_mesh[_i-1]).drug,
				_il_kopie,
				_mesh->get_ind_row(_hnd_mesh[_i-1]).pierw,
				_mesh->get_vert_row(_hnd_mesh[_i-1]).pierw,
				_il_wyrys
			);
			_il_kopie = 1;
			_il_wyrys = _i;
		}
	}
	if(_tex != 0) devctx->PSSetShaderResources(0, 1, &_tex->get(_hnd_tex[_i-1]));
	devctx->DrawIndexedInstanced(
		_mesh->get_ind_row(_hnd_mesh[_i-1]).drug,
		_il_kopie,
		_mesh->get_ind_row(_hnd_mesh[_i-1]).pierw,
		_mesh->get_vert_row(_hnd_mesh[_i-1]).pierw,
		_il_wyrys
	);
}
void Graphics::erase_occlu() {
	//XMFLOAT3 _wierz[] = {
	//	XMFLOAT3(-0.5f, -0.5f, 0.0f),
	//	XMFLOAT3(-0.5f, 0.5f, 0.0f),
	//	XMFLOAT3(0.5f, 0.5f, 0.0f),
	//	XMFLOAT3(0.5f, -0.5f, 0.0f),
	//};
	//res.update_vert(_wierz, 4, res.ob.vert_buf);
	//XMFLOAT2 _wsp_teks[] = {
	//	XMFLOAT2(0.0f, 1.0f),
	//	XMFLOAT2(0.0f, 0.0f),
	//	XMFLOAT2(1.0f, 0.0f),
	//	XMFLOAT2(1.0f, 1.0f),
	//};
	//res.update_coord_tex(_wsp_teks, 4, res.ob.world_buf);
	//DWORD _ind[] = {
	//	0, 1, 2,
	//	0, 2, 3
	//};
	//res.update_ind(_ind, 6, res.ob.ind_buf);

	//res.devctx->IASetInputLayout(res.in_lay[IN_F4F2]);
	//res.devctx->VSSetShader(res.vs[PASS_ON], 0, 0);
	//res.bind_vert(PASS_ON, res.ob);
	//res.devctx->IASetIndexBuffer(res.ob.ind_buf, DXGI_FORMAT_R32_UINT, 0);
	//res.devctx->PSSetShaderResources(0, 1, &res.ds_srv);
	//res.devctx->PSSetShader(res.ps[SAMPLE_TEX], 0, 0);
	//res.devctx->OMSetRenderTargets(1, &res.back_buf_rtv, 0);
	//float const _wart[] = {0.0f, 0.0f, 0.0f, 0.0f};
	//res.devctx->ClearRenderTargetView(res.back_buf_rtv, _wart);
	//res.devctx->DrawIndexed(6, 0, 0);
	//res.chain->Present(0, 0);
}
void Graphics::exe_cam_update(uint32_t const _i_task) {
	XMStoreFloat4x4(&cam.mtx_view, XMMatrixLookAtLH(
		XMLoadFloat3(&cam.pos),
		XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4(&cam.quat)) + XMLoadFloat3(&cam.pos),
		XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMLoadFloat4(&cam.quat))
	));
	XMStoreFloat4x4(&cam.mtx_proj, XMMatrixPerspectiveFovLH(
		cam.fov * 3.14f/180, float(scr_size.width)/scr_size.height, cam.near_z, cam.far_z
	));
	task.erase(_i_task);
}
void Graphics::exe_cam_update_pos(uint32_t const _i_task) {
	XMVECTOR _v = XMVectorSet(cam.v.x, 0.0f, cam.v.z, 0.0f);
	XMVECTOR _dl_v = XMVector3LengthEst(_v);
	if(XMVectorGetX(_dl_v) != 0.0f) {
		_v = XMVector3Rotate(_v, XMLoadFloat4(&cam.quat));
		_v = XMVectorSetY(_v, 0.0f);
		XMVECTOR _v_modul = XMVectorAbs(_v);
		_v = _v / (XMVectorGetX(_v_modul) + XMVectorGetZ(_v_modul)) * _dl_v;
	}
	_v = XMVectorSetY(_v, cam.v.y);
	XMStoreFloat3(&cam.pos, XMLoadFloat3(&cam.pos) + _v);
	task.erase(_i_task);
}
void Graphics::exe_cam_rot(uint32_t const _i_task) {
	XMFLOAT3 _katy = ((TaskCamRot*)task[_i_task])->angles;

	// obrót poziomy
	XMVECTOR _kwat_obr = XMQuaternionRotationRollPitchYaw(0.0f, _katy.y, 0.0f);
	XMVECTOR _kwat = XMLoadFloat4(&cam.quat);
	_kwat = XMQuaternionMultiply(_kwat, _kwat_obr);

	// obrót pionowy
	_kwat_obr = XMQuaternionRotationRollPitchYaw(_katy.x, 0.0f, 0.0f);
	XMStoreFloat4(&cam.quat, XMQuaternionMultiply(_kwat_obr, _kwat));

	task.erase(_i_task);
}
void Graphics::exe_cam_v(uint32_t const _i_task) {
	XMFLOAT3 _v = ((TaskCamV*)task[_i_task])->v;
	if(_v.x && _v.y) cam.v.z = _v.z;
	else if(_v.x && _v.z) cam.v.y = _v.y;
	else if(_v.y && _v.z) cam.v.x = _v.x;
	else if(_v.x) cam.v.x = _v.x;
	else if(_v.y) cam.v.y = _v.y;
	else if(_v.z) cam.v.z = _v.z;
	task.erase(_i_task);
}
void Graphics::exe_draw(uint32_t const _i_task) {
	devctx->IASetInputLayout(res.in_lay[IN_F4F2F44]);
	devctx->VSSetShader(res.vs[VS_PASS_ON], 0, 0);
	devctx->PSSetShader(res.ps[PS_SAMPLE_TEX], 0, 0);
	devctx->OMSetRenderTargets(1, &res.back_buf_rtv, res.ds_dsv);
	float const _wart[] = {0.0f, 0.0f, 0.0f, 0.0f};
	devctx->ClearRenderTargetView(res.back_buf_rtv, _wart);
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	res.ob.bind_vert(VS_TFORM_TEX);
	devctx->IASetIndexBuffer(res.ob.ind_buf, DXGI_FORMAT_R32_UINT, 0);

	res.ob.update_vert(g_par.ob.mesh.get_vert(), g_par.ob.mesh.get_vert_size());
	res.ob.update_coord_tex(g_par.ob.mesh.get_coord_tex(), g_par.ob.mesh.get_coord_tex_size());
	res.ob.update_ind(g_par.ob.mesh.get_ind(), g_par.ob.mesh.get_ind_size());
	draw_ob(g_par.ob.mesh_hnd, g_par.ob.tex_hnd, &g_par.ob.mesh, &g_par.ob.tex);

	res.chain->Present(0, 0);
	task.erase(_i_task);
}
void Graphics::exe_graph_defrag(uint32_t const _i_task) {
	uint32_t* _mapa1 = 0,* _mapa2 = 0;
	g_par.ob.mesh_hnd.defrag_comp(_mapa1, g_par.ob.mesh_hnd.get_size());
	g_par.ob.mesh_hnd.defrag_exe(_mapa1);
	g_par.ob.tex_hnd.defrag_exe(_mapa1);
	g_par.ob.mesh.defrag(g_par.ob.mesh.get_mesh_size());
	g_par.ob.tex.defrag(g_par.ob.tex.get_size());
	g_par.ob.mesh_hnd.defrag_comp(_mapa1, g_par.ob.mesh_hnd.get_size());
	g_par.ob.mesh_hnd.defrag_exe(_mapa1);
	g_par.ob.tex_hnd.defrag_exe(_mapa1);
	g_par.ob.mesh.defrag(g_par.ob.mesh.get_mesh_size());
	g_par.ob.tex.defrag(g_par.ob.tex.get_size());
	free(_mapa1);
	free(_mapa2);
	task.erase(_i_task);
}
void Graphics::exe_ob_create(uint32_t const _i_task) {
	TaskObCreate _t = *(TaskObCreate*)task[_i_task];
	ResultObCreate _r;
	_r.code = TASK_OB_CREATE;
	g_par.ob.mesh_hnd.push_back(_t.hnd_mesh);
	g_par.ob.tex_hnd.push_back(_t.hnd_tex);
	g_par.ob.mesh.create(_t.hnd_mesh);
	g_par.ob.tex.create(_t.hnd_tex);
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
	g_par.ob.bbox.push_back(_bbox, 8);
	_r.hnd_ob = g_par.no.wstaw(g_par.ob.mesh_hnd.get_size()-1);
	result.push_back((uint8_t*)&_r, sizeof(_r));
	task.erase(_i_task);
}
void Graphics::exe_ob_sort(uint32_t const _i_task) {
	uint32_t* _map = 0;
	uint32_t _ind = 0, _i;

	// według tekstur
	g_par.ob.tex_hnd.sort_comp(_map);
	g_par.ob.tex_hnd.sort_exe(_map);
	g_par.ob.mesh_hnd.sort_exe(_map);

	// według modeli
	g_par.ob.mesh_hnd.sort_comp(_map);
	g_par.ob.mesh_hnd.sort_exe(_map);
	g_par.ob.tex_hnd.sort_exe(_map);

	// same modele / tekstury
	g_par.ob.mesh.sort_comp(_map);
	g_par.ob.mesh.sort_exe(_map);
	g_par.ob.tex.sort_comp(_map);
	g_par.ob.tex.sort_exe(_map);

	task.erase(_i_task);
}
void Graphics::exe_occl_cull(uint32_t const _i_task) {
	//create_occlu_map();
	//erase_occlu();

	// occlusion test
	memset(&g_par.ob.is_occluder[0], 0, g_par.ob.is_occluder.get_size());
	res.ob.update_is_occluder(&g_par.ob.is_occluder[0], g_par.ob.is_occluder.get_size());

	uint32_t _init_counts = 0;
	devctx->CSSetShader(res.cs[CS_OCCL_CULL], 0, 0);
	devctx->CSSetShaderResources(0, 1, &res.ds_srv);
	devctx->CSSetShaderResources(0, 1, &res.ob.bbox_srv);
	//devctx->CSSetShaderResources(2, 1, &res.ob.wvp_srv);
	devctx->CSSetUnorderedAccessViews(0, 1, &res.ob.is_occluder_uav, &_init_counts);
	devctx->CSSetConstantBuffers(0, 1, &res.scr_size_buf);
	devctx->Dispatch(g_par.ob.bbox.get_col_size(), 1, 1);

	task.erase(_i_task);
}
void Graphics::exe_tasks() {
	if(task.get_col_size() > 1) {
		uint32_t* _map = 0;
		task.sort_comp(_map);
		task.sort_exe(_map);
		task.erase_dupl_comp(_map, FunHasz<uint8_t>(), FunHasz2<uint8_t>());
		task.sort_exe(_map);
		free(_map);
	}

	for(uint32_t _i = 0; _i < task.get_col_size(); ++_i) {
		if(task.get_row(_i) == task.empty) continue;

		switch(((Task*)task[_i])->code) {
		case TASK_CAM_V: exe_cam_v(_i); break;
		case TASK_CAM_ROT: exe_cam_rot(_i); break;
		case TASK_OB_CREATE: exe_ob_create(_i); break;
		case TASK_CAM_UPDATE_POS: exe_cam_update_pos(_i); break;
		case TASK_CAM_UPDATE: exe_cam_update(_i); break;
		case TASK_OB_SORT: exe_ob_sort(_i); break;
		case TASK_WVP_UPDATE: exe_wvp_update(_i); break;
		case TASK_BBOX_UPDATE: exe_bbox_update(_i); break;
		case TASK_OCCL_CULL: exe_occl_cull(_i); break;
		case TASK_DRAW: exe_draw(_i); break;
		case TASK_GRAPH_DEFRAG: exe_graph_defrag(_i); break;
		}
	}
}
void Graphics::exe_wvp_update(uint32_t const _i_task) {
	for(uint32_t _i = 0; _i < g_par.ob.mtx_world.get_size(); ++_i) {
		XMStoreFloat4x4(&g_par.ob.mtx_wvp[_i],
			XMLoadFloat4x4(&g_par.ob.mtx_world[_i]) *
			XMLoadFloat4x4(&cam.mtx_view) *
			XMLoadFloat4x4(&cam.mtx_proj)
		);
	}

	res.ob.update_wvp(&g_par.ob.mtx_wvp[0], g_par.ob.mtx_wvp.get_size());

	task.erase(_i_task);
}
void Graphics::exe_bbox_update(uint32_t const _i_task) {
	res.ob.update_bbox(g_par.ob.bbox[0], g_par.ob.bbox.get_size());

	task.erase(_i_task);
}
char const*const Graphics::get_tex_path(uint32_t const& _hnd_tex) const {
	switch(_hnd_tex) {
	case TEX_TRI:
		return "textures/cursor.jpg";
	case TEX_RECT:
		return "textures/rectangle.jpg";
	case TEX_DIAMENT:
		return "textures/diament.jpg";
	}
}

void Graphics::erase_ob(uint32_t const& _nr) {}
// -------------------------------------------------------




