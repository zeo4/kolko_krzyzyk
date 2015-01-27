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
	XMStoreFloat4x4(&cam.mtx_proj, _mtx_proj);

	// objects

	for(uint32_t _i = 0; _i < data_e.world.get_size(); ++_i) {
		// loc
		XMStoreFloat3(&data_e.loc[_i],
			XMLoadFloat3(&data_e.loc[_i]) + XMLoadFloat3(&data_e.v[_i])
		);

		// world
		data_e.world[_i]._41 = data_e.loc[_i].x;
		data_e.world[_i]._42 = data_e.loc[_i].y;
		data_e.world[_i]._43 = data_e.loc[_i].z;

		// wvp
		XMStoreFloat4x4(
			&data_e.wvp[_i],
			XMLoadFloat4x4(&data_e.world[_i]) * _mtx_view * _mtx_proj
		);
	}
	res.ob.update_wvp(&data_e.wvp[0], data_e.wvp.get_size());

	void* _data[] = {data_e.mesh.bbox[0], data_e.mesh.bbox_idx[0]};
	res.ob.update_bbox(_data, data_e.mesh.bbox.get_col_size());
	//res.ob.update_so(data_e.bbox_scr.get_size() * 2);
	//res.ob.update_so(data_e.mesh.bbox.get_size());

	task.erase(_i_task);
}
void Graphics::cull_occl(uint32_t const _i_task) {
	devctx->PSSetShader(res.ps[PS_WRITE_DEPTH], 0, 0);
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	float const _color[] = {0.0f, 0.0f, 0.0f, 0.0f};
	devctx->ClearRenderTargetView(res.rtv, _color);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);
	ID3D11UnorderedAccessView* _uav;
	ID3D11ShaderResourceView* _srv;

	// render bbox-es
	devctx->IASetInputLayout(res.in_lay[IN_F4F44]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11Buffer* _buf[] = {res.ob.bbox_buf, res.ob.wvp_buf};
	uint32_t _strides[] = {sizeof(XMFLOAT4), sizeof(XMFLOAT4X4)};
	uint32_t _offsets[] = {0, 0};
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	devctx->IASetIndexBuffer(res.ob.bbox_idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShader(res.vs[VS_TFORM_F4F44], 0, 0);
	uint32_t _inst_cnt = get_gr_cnt(0);
	devctx->DrawIndexedInstanced(
		data_e.mesh.bbox_idx.get_row(0).second,
		_inst_cnt,
		data_e.mesh.bbox_idx.get_row(0).first,
		data_e.mesh.bbox.get_row(0).first,
		0
	);
	_buf[0] = 0;
	_buf[1] = 0;
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShader(0, 0, 0);

	// create front rectangles
	res.ob.update_rect_front(0, data_e.wvp.get_size());
	uint32_t _init_counts = -1;
	devctx->CSSetShaderResources(0, 1, &res.ob.bbox_srv);
	devctx->CSSetShaderResources(1, 1, &res.ob.wvp_srv);
	devctx->CSSetUnorderedAccessViews(0, 1, &res.ob.rect_front_uav, &_init_counts);
	devctx->CSSetShader(res.cs[CS_RECT_FRONT], 0, 0);
	devctx->Dispatch(
		data_e.wvp.get_size(),
		1,
		1
	);
	_srv = 0;
	devctx->CSSetShaderResources(0, 1, &_srv);
	devctx->CSSetShaderResources(1, 1, &_srv);
	_uav = 0;
	devctx->CSSetUnorderedAccessViews(0, 1, &_uav, 0);
	devctx->CSSetShader(0, 0, 0);

	// render rect frames
	//devctx->SOSetTargets(1, &res.ob.so_buf, _offsets);
	devctx->IASetInputLayout(res.in_lay[IN_F4]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	devctx->IASetVertexBuffers(0, 1, &res.ob.rect_front_buf, _strides, _offsets);
	devctx->VSSetShader(res.vs[VS_PASS_F4], 0, 0);
	devctx->DrawInstanced(
		4,
		_inst_cnt,
		0,
		0
	);
	devctx->IASetVertexBuffers(0, 1, _buf, _strides, _offsets);
	devctx->VSSetShader(0, 0, 0);

	// reference frame
	XMFLOAT4 _vert[] = {
		XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),
		XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),
		XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),
	};
	//for(uint32_t _i = 0; _i < 5; ++_i) {
	//	XMStoreFloat4(&_vert[_i], XMVector4Transform(XMLoadFloat4(&_vert[_i]), XMLoadFloat4x4(&cam.mtx_proj)));
	//}
	res.ob.update_vert(_vert, 5);
	devctx->IASetInputLayout(res.in_lay[IN_F4]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	_strides[0] = sizeof(XMFLOAT4);
	devctx->IASetVertexBuffers(0, 1, &res.ob.vert_buf, _strides, _offsets);
	devctx->VSSetShader(res.vs[VS_PASS_F4], 0, 0);
	devctx->Draw(5, 0);
	devctx->IASetVertexBuffers(0, 1, _buf, _strides, _offsets);
	devctx->VSSetShader(0, 0, 0);

	chain->Present(0, 0);

	task.erase(_i_task);
}
uint32_t Graphics::get_gr_cnt(uint32_t _hnd_idx) const {
	uint32_t _cnt = 1;
	for(uint32_t _i = _hnd_idx + 1; _i < data_e.mesh_hnd.get_size(); ++_i) {
		if(data_e.mesh_hnd[_i-1] == data_e.mesh_hnd[_i] && data_e.tex_hnd[_i-1] == data_e.tex_hnd[_i])
			++_cnt;
	}
	return _cnt;
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
	uint32_t _i, _inst_cnt = 1, _drawed_cnt = 0;
	for(_i = 1; _i < data_e.mesh_hnd.get_size(); ++_i) {
		if(data_e.mesh_hnd[_i-1] == data_e.mesh_hnd[_i] && data_e.tex_hnd[_i-1] == data_e.tex_hnd[_i]) {
			++_inst_cnt;
		} else {
			if(_tex != 0) devctx->PSSetShaderResources(0, 1, &_tex->srv[_tex->no[data_e.tex_hnd[_i-1]]]);
			devctx->DrawIndexedInstanced(
				data_e.mesh.vert_idx.get_row(data_e.mesh_hnd[_i-1]).second,
				_inst_cnt,
				data_e.mesh.vert_idx.get_row(data_e.mesh_hnd[_i-1]).first,
				data_e.mesh.vert.get_row(data_e.mesh_hnd[_i-1]).first,
				_drawed_cnt
			);
			_inst_cnt = 1;
			_drawed_cnt = _i;
		}
	}
	devctx->PSSetShaderResources(0, 1, &data_e.tex.srv[data_e.tex.no[data_e.tex_hnd[_i-1]]]);
	devctx->DrawIndexedInstanced(
		data_e.mesh.vert_idx.get_row(data_e.mesh_hnd[_i-1]).second,
		_inst_cnt,
		data_e.mesh.vert_idx.get_row(data_e.mesh_hnd[_i-1]).first,
		data_e.mesh.vert.get_row(data_e.mesh_hnd[_i-1]).first,
		_drawed_cnt
	);
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

	//data_e.mesh.sort_comp(_map);
	//data_e.mesh.sort_exe(_map);
	//data_e.tex.sort_comp(_map);
	//data_e.tex.sort_exe(_map);

	task.erase(_i_task);
}
// -------------------------------------------------------




