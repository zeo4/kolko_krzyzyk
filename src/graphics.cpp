#pragma once

#include <debugs.h>
#include <graphics.h>
#include <handles.h>
// -------------------------------------------------------
void Graphics::do_tasks() {
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
		case TASK_UPDATE_POS_OB: update_pos_ob(_i); break;
		case TASK_CULL_OCCL: cull_occl(_i); break;
		case TASK_DRAW: draw(_i); break;
		}
	}
}
void Graphics::update_pos_ob(uint32_t const _i_task) {
	XMMATRIX _mtx_temp;

	// camera

	// loc
	XMVECTOR _v_world = XMVectorSet(cam.v.x, 0.0f, cam.v.z, 0.0f);
	_v_world = XMVector3Rotate(_v_world, XMLoadFloat4(&cam.q));
	_v_world = XMVectorSetY(_v_world, cam.v.y);
	XMStoreFloat3(&cam.loc, XMLoadFloat3(&cam.loc) + _v_world);

	// matrices
	XMMATRIX _mtx_view = XMMatrixLookAtLH (
		XMLoadFloat3(&cam.loc),
		XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4(&cam.q)) + XMLoadFloat3(&cam.loc),
		XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMLoadFloat4(&cam.q))
	);
	XMMATRIX _mtx_proj = XMMatrixPerspectiveFovLH(
		cam.fov * 3.14f/180, float(scr_size.width) / scr_size.height, cam.near_z, cam.far_z
	);

	// objects

	for(uint32_t _i = 0; _i < data_e.mtx_world.get_size(); ++_i) {
		// loc
		XMStoreFloat3(&data_e.loc[_i],
			XMLoadFloat3(&data_e.loc[_i]) + XMLoadFloat3(&data_e.v[_i])
		);

		// world
		data_e.mtx_world[_i]._41 = data_e.loc[_i].x;
		data_e.mtx_world[_i]._42 = data_e.loc[_i].y;
		data_e.mtx_world[_i]._43 = data_e.loc[_i].z;

		// wvp
		_mtx_temp = XMLoadFloat4x4(&data_e.mtx_world[_i]) * _mtx_view * _mtx_proj;
		XMStoreFloat4x4(&data_e.mtx_wvp[_i], _mtx_temp);

		// screen space bbox
		for(uint32_t _j = 0; _j < 8; ++_j) {
			XMStoreFloat3(&data_e.bbox_scr[_i][_j], XMVector3TransformCoord(XMLoadFloat3(&data_e.bbox_local[_i][_j]), _mtx_temp));
		}
	}

	res.ob.update_wvp(&data_e.mtx_wvp[0], data_e.mtx_wvp.get_size());
	res.ob.update_bbox(data_e.bbox_scr[0], data_e.bbox_scr.get_size());
	//res.ob.update_so(data_e.bbox_scr.get_size() * 2);
	res.ob.update_so(data_e.bbox_scr.get_size());

	task.erase(_i_task);
}
void Graphics::cull_occl(uint32_t const _i_task) {
	devctx->IASetInputLayout(res.in_lay[IN_F3]);
	uint32_t _strides[] = {sizeof(XMFLOAT3)};
	uint32_t _offsets[] = {0};
	devctx->IASetVertexBuffers(0, 1, &res.ob.bbox_buf, _strides, _offsets);
	devctx->VSSetShader(res.vs[VS_PASS_F3], 0, 0);
	devctx->PSSetShader(res.ps[PS_WRITE_DEPTH], 0, 0);
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	float const _color[] = {0.0f, 0.0f, 0.0f, 0.0f};
	devctx->ClearRenderTargetView(res.rtv, _color);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);

	// render bbox
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DWORD _ind[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		1, 5, 6,
		1, 6, 2,
		2, 6, 7,
		2, 7, 3,
		4, 7, 6,
		4, 6, 5,
	};
	res.ob.update_ind(_ind, 36);
	devctx->IASetIndexBuffer(res.ob.ind_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->GSSetShader(0, 0, 0);
	devctx->DrawIndexed(36, 0, 0);

	// render rect frame
	//devctx->SOSetTargets(1, &res.ob.so_buf, _offsets);
	XMFLOAT3 _vert[] = {
		data_e.bbox_scr[0][0],
		data_e.bbox_scr[0][0],
	};
	for(uint32_t _i = 1; _i < 8; ++_i) {
		if(data_e.bbox_scr[0][_i].x < _vert[0].x)
			_vert[0].x = data_e.bbox_scr[0][_i].x;
		else if(data_e.bbox_scr[0][_i].x > _vert[1].x)
			_vert[1].x = data_e.bbox_scr[0][_i].x;
		if(data_e.bbox_scr[0][_i].y < _vert[0].y)
			_vert[0].y = data_e.bbox_scr[0][_i].y;
		else if(data_e.bbox_scr[0][_i].y > _vert[1].y)
			_vert[1].y = data_e.bbox_scr[0][_i].y;
		if(data_e.bbox_scr[0][_i].z < _vert[0].z)
			_vert[0].z = data_e.bbox_scr[0][_i].z;
		else if(data_e.bbox_scr[0][_i].z > _vert[1].z)
			_vert[1].z = data_e.bbox_scr[0][_i].z;
	}
	res.ob.update_vert(_vert, 2);
	devctx->IASetVertexBuffers(0, 1, &res.ob.vert_buf, _strides, _offsets);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	devctx->GSSetShader(res.gs[GS_GEN_OCCL_RECT_FRAME], 0, 0);
	devctx->Draw(2, 0);

	chain->Present(0, 0);

	task.erase(_i_task);
}
void Graphics::draw(uint32_t const _i_task) {
	//draw_previous();

	task.erase(_i_task);
}

void Graphics::draw_previous() {
	//if(g_analysis != 0) g_analysis->BeginCapture();

	//devctx->IASetInputLayout(res.in_lay[IN_F3F2F44]);
	//devctx->VSSetShader(res.vs[VS_PASS_ON], 0, 0);
	//devctx->PSSetShader(res.ps[PS_SAMPLE_TEX], 0, 0);
	//devctx->OMSetRenderTargets(1, &res.back_buf_rtv, res.ds_dsv);
	//float const _wart[] = {0.0f, 0.0f, 0.0f, 0.0f};
	//devctx->ClearRenderTargetView(res.back_buf_rtv, _wart);
	//devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	//res.ob.bind_vert(VS_TFORM_TEX);
	//devctx->IASetIndexBuffer(res.ob.ind_buf, DXGI_FORMAT_R32_UINT, 0);

	//res.ob.update_vert(data_e.mesh.get_vert(), data_e.mesh.get_vert_size());
	//res.ob.update_coord_tex(data_e.mesh.get_coord_tex(), data_e.mesh.get_coord_tex_size());
	//res.ob.update_ind(data_e.mesh.get_ind(), data_e.mesh.get_ind_size());
	//draw_ob(data_e.mesh_hnd, data_e.tex_hnd, &data_e.mesh, &data_e.tex);

	//res.chain->Present(0, 0);
}
void Graphics::draw_ob(Vec<uint32_t>const& _hnd_mesh, Vec<uint32_t>const& _hnd_tex, Meshes*const _mesh, Textures*const _tex) const {
	//uint32_t _i, _il_kopie = 1, _il_wyrys = 0;
	//for(_i = 1; _i < _hnd_mesh.get_size(); ++_i) {
	//	if(_hnd_mesh[_i-1] == _hnd_mesh[_i] && _hnd_tex[_i-1] == _hnd_tex[_i]) {
	//		++_il_kopie;
	//	} else {
	//		if(_tex != 0) devctx->PSSetShaderResources(0, 1, &_tex->get(_hnd_tex[_i-1]));
	//		devctx->DrawIndexedInstanced(
	//			_mesh->get_ind_row(_hnd_mesh[_i-1]).drug,
	//			_il_kopie,
	//			_mesh->get_ind_row(_hnd_mesh[_i-1]).pierw,
	//			_mesh->get_vert_row(_hnd_mesh[_i-1]).pierw,
	//			_il_wyrys
	//		);
	//		_il_kopie = 1;
	//		_il_wyrys = _i;
	//	}
	//}
	//if(_tex != 0) devctx->PSSetShaderResources(0, 1, &_tex->get(_hnd_tex[_i-1]));
	//devctx->DrawIndexedInstanced(
	//	_mesh->get_ind_row(_hnd_mesh[_i-1]).drug,
	//	_il_kopie,
	//	_mesh->get_ind_row(_hnd_mesh[_i-1]).pierw,
	//	_mesh->get_vert_row(_hnd_mesh[_i-1]).pierw,
	//	_il_wyrys
	//);
}
void Graphics::do_ob_sort(uint32_t const _i_task) {
	uint32_t* _map = 0;
	uint32_t _ind = 0, _i;

	// według tekstur
	data_e.tex_hnd.sort_comp(_map);
	data_e.tex_hnd.sort_exe(_map);
	data_e.mesh_hnd.sort_exe(_map);

	// według modeli
	data_e.mesh_hnd.sort_comp(_map);
	data_e.mesh_hnd.sort_exe(_map);
	data_e.tex_hnd.sort_exe(_map);

	// same modele / tekstury
	data_e.mesh.sort_comp(_map);
	data_e.mesh.sort_exe(_map);
	data_e.tex.sort_comp(_map);
	data_e.tex.sort_exe(_map);

	task.erase(_i_task);
}
// -------------------------------------------------------




