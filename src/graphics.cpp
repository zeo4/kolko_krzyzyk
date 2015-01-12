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
	//XMMATRIX _mtx_temp;

	//// cam
	//XMMATRIX _mtx_view = XMMatrixLookAtLH (
	//	XMLoadFloat3(&cam.loc),
	//	XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4(&cam.q)) + XMLoadFloat3(&cam.loc),
	//	XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMLoadFloat4(&cam.q))
	//);
	//XMMATRIX _mtx_proj = XMMatrixPerspectiveFovLH(
	//	cam.fov * 3.14f/180, float(scr_size.width)/scr_size.height, cam.near_z, cam.far_z
	//);

	//// ob
	//for(uint32_t _i = 0; _i < data_e.mtx_world.get_size(); ++_i) {
	//	// loc
	//	XMStoreFloat3(&data_e.loc[_i],
	//		XMLoadFloat3(&data_e.loc[_i]) + XMLoadFloat3(&data_e.v[_i])
	//	);

	//	// world
	//	data_e.mtx_world[_i]._41 = data_e.loc[_i].x;
	//	data_e.mtx_world[_i]._42 = data_e.loc[_i].y;
	//	data_e.mtx_world[_i]._43 = data_e.loc[_i].z;

	//	// wvp
	//	_mtx_temp = XMLoadFloat4x4(&data_e.mtx_world[_i]) * _mtx_view * _mtx_proj;
	//	XMStoreFloat4x4(&data_e.mtx_wvp[_i], _mtx_temp);

	//	// screen space bbox
	//	for(uint32_t _j = 0; _j < 8; ++_j) {
	//		XMStoreFloat4(&data_e.bbox_scr[_i][_j], XMVector3TransformCoord(XMLoadFloat4(&data_e.bbox_local[_i][_j]), _mtx_temp));
	//	}
	//}

	//res.ob.update_wvp(&data_e.mtx_wvp[0], data_e.mtx_wvp.get_size());
	////res.ob.update_bbox(data_e.bbox_scr[0], data_e.bbox_scr.get_size());

	task.erase(_i_task);
}
void Graphics::cull_occl(uint32_t const _i_task) {
	// occlusion test

	//memset(&data_e.occluder[0], 0, data_e.occluder.get_size());
	//res.ob.update_is_occluder(&data_e.occluder[0], data_e.occluder.get_size());

	//uint32_t _init_counts = 0;
	//devctx->CSSetShader(res.cs[CS_OCCL_CULL], 0, 0);
	//devctx->CSSetShaderResources(0, 1, &res.ds_srv);
	//devctx->CSSetShaderResources(0, 1, &res.ob.bbox_srv);
	//devctx->CSSetUnorderedAccessViews(0, 1, &res.ob.occluder_uav, &_init_counts);
	//devctx->CSSetConstantBuffers(0, 1, &res.scr_size_buf);
	//devctx->Dispatch(data_e.bbox_scr.get_col_size(), 1, 1);

	//// test of occlusion test
	//devctx->IASetInputLayout(res.in_lay[IN_F4]);
	//devctx->VSSetShader(res.vs[VS_TEST], 0, 0);
	//devctx->VSSetShaderResources(0, 1, &res.ob.occluder_srv);
	//devctx->PSSetShader(0, 0, 0);
	//res.ob.bind_vert(VS_TEST);
	//XMFLOAT3 _vert[] = {XMFLOAT3(), XMFLOAT3(), XMFLOAT3()};
	//res.ob.update_vert(_vert, 3);
	//devctx->Draw(3, 0);

	task.erase(_i_task);
}
void Graphics::draw(uint32_t const _i_task) {
	//draw_previous();

	task.erase(_i_task);
}

//void Graphics::test_vsga() {
//	vector<float> _data1, _data2, _data_bbox;
//	vector<XMFLOAT4> _data_f4;
//	++_val;
//	for(uint32_t _i = 0; _i < data_e.bbox_scr.get_col_size(); ++_i) {
//		_data1.push_back(_val);
//		_data2.push_back(_val);
//		_data_bbox.push_back(321.0f);
//		//_data_f4.push_back(XMFLOAT4(_i + 0.1f, _i + 0.2f, _i + 0.3f, _i + 0.4f));
//	}
//	//res.ob.update_bbox(data_e.bbox_scr[0], data_e.bbox_scr.get_col_size());
//	//res.ob.update_bbox(&_data_bbox[0], _data1.size());
//	res.ob.test_update(0, _data1.size(), 0);
//	//if(g_analysis != 0) g_analysis->BeginCapture();
//	devctx->UpdateSubresource(res.ob.test1_buf, 0, 0, &_data1[0], 0, 0);
//	chain->Present(0, 0);
//	devctx->UpdateSubresource(res.ob.test2_buf, 0, 0, &_data1[0], 0, 0);
//	//devctx->Dispatch(1,1,1);
//	res.ob.test_update_rect_occl(0, _data1.size());
//	//res.ob.test_update_staging(0, _data1.size());
//
//	// compute rectangle vertices
//	uint32_t _init_counts = 0;
//	ID3D11ShaderResourceView* _ptr_srv = 0;
//	ID3D11UnorderedAccessView* _ptr_uav = 0;
//	//devctx->CSSetShaderResources(0, 1, &_ptr_srv);
//	devctx->CSSetUnorderedAccessViews(0, 1, &_ptr_uav, &_init_counts);
//	devctx->CSSetUnorderedAccessViews(1, 1, &_ptr_uav, &_init_counts);
//	devctx->CSSetShader(res.cs[TEST_CS_RECT_OCCL], 0, 0);
//	//devctx->CSSetShaderResources(0, 1, &res.ob.test1_srv);
//	devctx->CSSetUnorderedAccessViews(0, 1, &res.ob.test1_uav, &_init_counts);
//	devctx->CSSetUnorderedAccessViews(1, 1, &res.ob.test2_uav, &_init_counts);
//	//devctx->Dispatch(data_e.bbox_scr.get_col_size(), 1, 1);
//	//devctx->Dispatch(data_e.bbox_scr.get_col_size(), 1, 1);
//	devctx->Dispatch(1, 1, 1);
//	chain->Present(0, 0);
//
//	//if(g_analysis != 0) g_analysis->BeginCapture();
//	//vector<float> _data;
//	//for(uint32_t _i = 0; _i < 3; ++_i) {
//	//	_data.push_back(_i+1);
//	//}
//	//res.ob.test_update(&_data[0], 3, &_data[0]);
//	devctx->OMSetRenderTargetsAndUnorderedAccessViews(1, &res.rtv, res.ds_dsv, 1, 1, &res.ob.test2_uav, &_init_counts);
//	devctx->VSSetShader(res.vs[TEST_VS_RECT_OCCL], 0, 0);
//	devctx->PSSetShader(res.ps[TEST_PS_RECT_OCCL], 0, 0);
//	XMFLOAT3 _v[] = {
//		XMFLOAT3(-0.5f, -0.5f, 0.0f),
//		XMFLOAT3(-0.5f, 0.5f, 0.0f),
//		XMFLOAT3(0.5f, 0.5f, 0.0f),
//	};
//	res.ob.update_vert(_v, 3);
//	res.ob.bind_vert(TEST_VS_RECT_OCCL);
//	devctx->IASetInputLayout(res.in_lay[IN_F3]);
//	float _color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
//	devctx->ClearRenderTargetView(res.rtv, _color);
//	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
//	devctx->Draw(3, 0);
//	chain->Present(0, 0);
//}
//float Graphics::_val = 0;

void Graphics::do_cam_update_pos(uint32_t const _i_task) {
	XMVECTOR _v = XMVectorSet(cam.v.x, 0.0f, cam.v.z, 0.0f);
	XMVECTOR _dl_v = XMVector3LengthEst(_v);
	if(XMVectorGetX(_dl_v) != 0.0f) {
		_v = XMVector3Rotate(_v, XMLoadFloat4(&cam.q));
		_v = XMVectorSetY(_v, 0.0f);
		XMVECTOR _v_modul = XMVectorAbs(_v);
		_v = _v / (XMVectorGetX(_v_modul) + XMVectorGetZ(_v_modul)) * _dl_v;
	}
	_v = XMVectorSetY(_v, cam.v.y);
	XMStoreFloat3(&cam.loc, XMLoadFloat3(&cam.loc) + _v);
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




