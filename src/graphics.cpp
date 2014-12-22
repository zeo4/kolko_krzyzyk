#pragma once

#include <debugs.h>
#include <graphics.h>
#include <handles.h>
// -------------------------------------------------------
void Graphics::draw_ob(Vec<uint32_t>const& _hnd_mesh, Vec<uint32_t>const& _hnd_tex, Meshes*const _mesh, Textures*const _tex) const {
	uint32_t _i, _il_kopie = 1, _il_wyrys = 0;
	for(_i = 1; _i < _hnd_mesh.get_size(); ++_i) {
		if(_hnd_mesh[_i-1] == _hnd_mesh[_i] && _hnd_tex[_i-1] == _hnd_tex[_i]) {
			++_il_kopie;
		} else {
			if(_tex != 0) res.devctx->PSSetShaderResources(0, 1, &_tex->get(_hnd_tex[_i-1]));
			res.devctx->DrawIndexedInstanced(
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
	if(_tex != 0) res.devctx->PSSetShaderResources(0, 1, &_tex->get(_hnd_tex[_i-1]));
	res.devctx->DrawIndexedInstanced(
		_mesh->get_ind_row(_hnd_mesh[_i-1]).drug,
		_il_kopie,
		_mesh->get_ind_row(_hnd_mesh[_i-1]).pierw,
		_mesh->get_vert_row(_hnd_mesh[_i-1]).pierw,
		_il_wyrys
	);
}
void Graphics::create_occlu_map() {
	res.update_vert(graph_par.occludee_mesh.get_vert(), graph_par.occludee_mesh.get_vert_size());
	res.update_ind(graph_par.occludee_mesh.get_ind(), graph_par.occludee_mesh.get_ind_size());
	
	res.devctx->IASetInputLayout(res.in_lay[IN_F4F2F44]);
	res.devctx->VSSetShader(res.szad_vert[VS_WVP_TRANS], 0, 0);
	res.bind_vert(VS_WVP_TRANS);
	res.devctx->IASetIndexBuffer(res.buf_ind, DXGI_FORMAT_R32_UINT, 0);
	res.devctx->PSSetShader(res.szad_pix[PS_TEX_SAMPLE], 0, 0);
	res.devctx->OMSetRenderTargets(1, &res.buf_back_rtv, res.depth_stencil_dsv);
	res.devctx->ClearDepthStencilView(res.depth_stencil_dsv, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	draw_ob(graph_par.occludee_mesh_hnd, graph_par.occludee_tex_hnd, &graph_par.occludee_mesh, &graph_par.occludee_tex);
	res.chain->Present(0, 0);
}
void Graphics::erase_occlu() {
	XMFLOAT3 _wierz[] = {
		XMFLOAT3(-0.5f, -0.5f, 0.0f),
		XMFLOAT3(-0.5f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f),
	};
	res.update_vert(_wierz, 4);
	XMFLOAT2 _wsp_teks[] = {
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f),
	};
	res.update_coord_tex(_wsp_teks, 4);
	DWORD _ind[] = {
		0, 1, 2,
		0, 2, 3
	};
	res.update_ind(_ind, 6);

	res.devctx->IASetInputLayout(res.in_lay[IN_F4F2]);
	res.devctx->VSSetShader(res.szad_vert[VS_PASS_ON], 0, 0);
	res.bind_vert(VS_PASS_ON);
	res.devctx->IASetIndexBuffer(res.buf_ind, DXGI_FORMAT_R32_UINT, 0);
	res.devctx->PSSetShaderResources(0, 1, &res.depth_stencil_srv);
	res.devctx->PSSetShader(res.szad_pix[PS_TEX_SAMPLE], 0, 0);
	res.devctx->OMSetRenderTargets(1, &res.buf_back_rtv, 0);
	float const _wart[] = {0.0f, 0.0f, 0.0f, 0.0f};
	res.devctx->ClearRenderTargetView(res.buf_back_rtv, _wart);
	res.devctx->DrawIndexed(6, 0, 0);
	res.chain->Present(0, 0);
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
void Graphics::exe_cam_v(uint32_t const _i_zad) {
	XMFLOAT3 _v = ((TaskCamV*)task[_i_zad])->v;
	if(_v.x && _v.y) cam.v.z = _v.z;
	else if(_v.x && _v.z) cam.v.y = _v.y;
	else if(_v.y && _v.z) cam.v.x = _v.x;
	else if(_v.x) cam.v.x = _v.x;
	else if(_v.y) cam.v.y = _v.y;
	else if(_v.z) cam.v.z = _v.z;
	task.erase(_i_zad);
}
void Graphics::exe_cam_rot(uint32_t const _i_zad) {
	XMFLOAT3 _katy = ((TaskCamRot*)task[_i_zad])->angles;

	// obrót poziomy
	XMVECTOR _kwat_obr = XMQuaternionRotationRollPitchYaw(0.0f, _katy.y, 0.0f);
	XMVECTOR _kwat = XMLoadFloat4(&cam.quat);
	_kwat = XMQuaternionMultiply(_kwat, _kwat_obr);

	// obrót pionowy
	_kwat_obr = XMQuaternionRotationRollPitchYaw(_katy.x, 0.0f, 0.0f);
	XMStoreFloat4(&cam.quat, XMQuaternionMultiply(_kwat_obr, _kwat));

	task.erase(_i_zad);
}
void Graphics::exe_ob_create(uint32_t const _i_zad) {
	TaskObCreate _z = *(TaskObCreate*)task[_i_zad];
	ResultObCreate _w;
	_w.kod_zad = OB_CREATE;
	graph_par.occludee_mesh.create(_z.hnd_mesh);
	graph_par.occludee_tex.create(_z.hnd_tex);
	graph_par.occludee_mesh_hnd.push_back(_z.hnd_mesh);
	graph_par.occludee_tex_hnd.push_back(_z.hnd_tex);
	_w.uch_ob = graph_par.no.wstaw(graph_par.occludee_mesh_hnd.get_size()-1);
	result.push_back((uint8_t*)&_w, sizeof(_w));
	task.erase(_i_zad);
}
void Graphics::exe_cam_curr_pos(uint32_t const _i_zad) {
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
	task.erase(_i_zad);
}
void Graphics::exe_cam_curr(uint32_t const _i_zad) {
	XMStoreFloat4x4(&cam.mtx_view, XMMatrixLookAtLH(
		XMLoadFloat3(&cam.pos),
		XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4(&cam.quat)) + XMLoadFloat3(&cam.pos),
		XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMLoadFloat4(&cam.quat))
	));
	XMStoreFloat4x4(&cam.mtx_proj, XMMatrixPerspectiveFovLH(
		cam.angle * 3.14f/180, float(szerRend)/wysRend, cam.near_z, cam.far_z
	));
	task.erase(_i_zad);
}
void Graphics::exe_ob_sort(uint32_t const _i_zad) {
	uint32_t* _map = 0;
	uint32_t _ind = 0, _i;

	// według tekstur
	graph_par.occludee_tex_hnd.sort_comp(_map);
	graph_par.occludee_tex_hnd.sort_exe(_map);
	graph_par.occludee_mesh_hnd.sort_exe(_map);

	// według modeli
	graph_par.occludee_mesh_hnd.sort_comp(_map);
	graph_par.occludee_mesh_hnd.sort_exe(_map);
	graph_par.occludee_tex_hnd.sort_exe(_map);

	// same modele / tekstury
	graph_par.occludee_mesh.sort_comp(_map);
	graph_par.occludee_mesh.sort_exe(_map);
	graph_par.occludee_tex.sort_comp(_map);
	graph_par.occludee_tex.sort_exe(_map);

	task.erase(_i_zad);
}
void Graphics::exe_world_curr(uint32_t const _i_zad) {
	Vec<XMFLOAT4X4> _mac;
	for(uint32_t _i = 0; _i < phys_par.mtx_world.get_size(); ++_i) {
		_mac.push_back(XMFLOAT4X4());
		XMStoreFloat4x4(&_mac[_i], XMMatrixTranspose(
			XMLoadFloat4x4(&phys_par.mtx_world[_i]) *
			XMLoadFloat4x4(&cam.mtx_view) *
			XMLoadFloat4x4(&cam.mtx_proj)
		));
	}
	res.update_world(&_mac[0], _mac.get_size());
	task.erase(_i_zad);
}
void Graphics::exe_occlu_erase(uint32_t const _i_zad) {
	create_occlu_map();
	erase_occlu();
	task.erase(_i_zad);
}
void Graphics::exe_draw(uint32_t const _i_zad) {
	res.devctx->IASetInputLayout(res.in_lay[IN_F4F2F44]);
	res.devctx->VSSetShader(res.szad_vert[VS_PASS_ON], 0, 0);
	res.devctx->PSSetShader(res.szad_pix[PS_TEX_SAMPLE], 0, 0);
	res.devctx->OMSetRenderTargets(1, &res.buf_back_rtv, res.depth_stencil_dsv);
	float const _wart[] = {0.0f, 0.0f, 0.0f, 0.0f};
	res.devctx->ClearRenderTargetView(res.buf_back_rtv, _wart);
	res.devctx->ClearDepthStencilView(res.depth_stencil_dsv, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	res.bind_vert(VS_WVP_TRANS);
	res.devctx->IASetIndexBuffer(res.buf_ind, DXGI_FORMAT_R32_UINT, 0);

	res.update_vert(graph_par.occludee_mesh.get_vert(), graph_par.occludee_mesh.get_vert_size());
	res.update_coord_tex(graph_par.occludee_mesh.get_coord_tex(), graph_par.occludee_mesh.get_coord_tex_size());
	res.update_ind(graph_par.occludee_mesh.get_ind(), graph_par.occludee_mesh.get_ind_size());
	draw_ob(graph_par.occludee_mesh_hnd, graph_par.occludee_tex_hnd, &graph_par.occludee_mesh, &graph_par.occludee_tex);

	res.chain->Present(0, 0);
	task.erase(_i_zad);
}
void Graphics::exe_graph_defrag(uint32_t const _i_zad) {
	uint32_t* _mapa1 = 0,* _mapa2 = 0;
	graph_par.occluder_mesh_hnd.defrag_comp(_mapa1, graph_par.occluder_mesh_hnd.get_size());
	graph_par.occluder_mesh_hnd.defrag_exe(_mapa1);
	graph_par.occluder_tex_hnd.defrag_exe(_mapa1);
	graph_par.occluder_mesh.defrag(graph_par.occluder_mesh.get_mesh_size());
	graph_par.occluder_tex.defrag(graph_par.occluder_tex.get_size());
	graph_par.occludee_mesh_hnd.defrag_comp(_mapa1, graph_par.occludee_mesh_hnd.get_size());
	graph_par.occludee_mesh_hnd.defrag_exe(_mapa1);
	graph_par.occludee_tex_hnd.defrag_exe(_mapa1);
	graph_par.occludee_mesh.defrag(graph_par.occludee_mesh.get_mesh_size());
	graph_par.occludee_tex.defrag(graph_par.occludee_tex.get_size());
	free(_mapa1);
	free(_mapa2);
	task.erase(_i_zad);
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
		case CAM_V: exe_cam_v(_i); break;
		case CAM_ROT: exe_cam_rot(_i); break;
		case OB_CREATE: exe_ob_create(_i); break;
		case CAM_UPDATE_POS: exe_cam_curr_pos(_i); break;
		case CAM_UPDATE: exe_cam_curr(_i); break;
		case OB_SORT: exe_ob_sort(_i); break;
		case WORLD_UPDATE: exe_world_curr(_i); break;
		case OCCLU_ERASE: exe_occlu_erase(_i); break;
		case DRAW: exe_draw(_i); break;
		case GRAPH_DEFRAG: exe_graph_defrag(_i); break;
		}
	}
}

void Graphics::erase_ob(uint32_t const& _nr) {

}
// -------------------------------------------------------




