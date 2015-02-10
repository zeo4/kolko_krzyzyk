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
		case TASK_DRAW: draw(_i); break;
		}
	}
}
void Graphics::update_pos_ob(uint32_t const _i_task) {
	// loc of main camera
	XMVECTOR _v_cam = XMVectorSet(cam.v.x, 0.0f, cam.v.z, 0.0f);
	_v_cam = XMVector3Rotate(_v_cam, XMLoadFloat4(&cam.q));
	_v_cam = XMVectorSetY(_v_cam, cam.v.y);
	XMStoreFloat3(&cam.loc, XMLoadFloat3(&cam.loc) + _v_cam);

	// matrices of main camera
	XMMATRIX _mtx_view = XMMatrixLookAtLH(
		XMLoadFloat3(&cam.loc),
		XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XMLoadFloat4(&cam.q)) + XMLoadFloat3(&cam.loc),
		XMVector3Rotate(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMLoadFloat4(&cam.q))
	);
	XMMATRIX _mtx_proj = XMMatrixPerspectiveFovLH(
		cam.fov * 3.14f/180, float(screen.width) / screen.height, cam.near_z, cam.far_z
	);
	XMStoreFloat4x4(&cam.mtx_proj, _mtx_proj);

	// matrices of map camera
	XMMATRIX _mtx_view_map = XMMatrixLookAtLH(
		XMLoadFloat3(&cam.loc) + XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f),
		XMLoadFloat3(&cam.loc),
		XMLoadFloat3(&cam.loc) + XMVectorSet(0.0f, 3.0f, 1.0f, 0.0f)
		//XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
	);
	XMMATRIX _mtx_proj_map = XMMatrixOrthographicLH(20.0f, 20.0f, 1.0f, 100.0f);

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

		// wvp of map
		XMStoreFloat4x4(
			&data_e.wvp_map[_i],
			XMLoadFloat4x4(&data_e.world[_i]) * _mtx_view_map * _mtx_proj_map
		);
	}
	void* _data[] = {&data_e.wvp[0], &data_e.wvp_map[0]};
	res.ob.update_wvp(_data, data_e.wvp.get_size());

	res.ob.update_bbox(data_e.mesh.bbox[0], data_e.mesh.bbox.get_col_size());
	res.ob.update_occl_rect(0, data_e.wvp.get_size());
	res.ob.update_occluders(0, data_e.wvp.get_size());
	//res.ob.update_so(data_e.bbox_scr.get_size() * 2);
	//res.ob.update_so(data_e.mesh.bbox.get_size());

	task.erase(_i_task);
}
void Graphics::draw(uint32_t const _i_task) {
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	devctx->RSSetViewports(1, &res.viewport[VIEWPORT_MAIN]);
	float const _color[] = {0.0f, 0.0f, 0.0f, 0.0f};
	devctx->ClearRenderTargetView(res.rtv, _color);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);
	
	cull_occl();

	//draw_bbox();
	//draw_rect_front_frame();
	//draw_rect_front_face();
	//draw_previous();

	chain->Present(0, 0);

	task.erase(_i_task);
}
void Graphics::cull_occl() {
	create_occl_shape();
	draw_depth_occl_shape();
	downsample_ds();
	mark_occluders();
	draw_occl_map();
	//apply_occl_rep();
}
void Graphics::create_occl_shape() {
	ID3D11ShaderResourceView* _srv_null = 0;
	ID3D11UnorderedAccessView* _uav_null = 0;

	devctx->CSSetShaderResources(0, 1, &res.ob.bbox_srv);
	devctx->CSSetShaderResources(1, 1, &res.ob.wvp_srv);
	devctx->CSSetUnorderedAccessViews(0, 1, &res.ob.occl_rect_uav, 0);
	devctx->CSSetShader(res.cs[CS_CREATE_OCCL_SHAPE], 0, 0);
	devctx->Dispatch(
		data_e.wvp.get_size(),
		1,
		1
	);
	devctx->CSSetShaderResources(0, 1, &_srv_null);
	devctx->CSSetShaderResources(1, 1, &_srv_null);
	devctx->CSSetUnorderedAccessViews(0, 1, &_uav_null, 0);
	devctx->CSSetShader(0, 0, 0);
}
void Graphics::draw_depth_occl_shape() {
	ID3D11RenderTargetView* _rtv_null = 0;
	ID3D11ShaderResourceView* _srv_null = 0;

	devctx->VSSetShader(res.vs[VS_VOID_PASS_OCCL_RECT], 0, 0);
	devctx->PSSetShader(0, 0, 0);
	devctx->IASetInputLayout(res.in_lay[IN_VOID]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DWORD _idx[] = {
		0, 1, 2,
		0, 2, 3,
	};
	res.ob.update_idx(_idx, 6);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShaderResources(0, 1, &res.ob.occl_rect_srv);
	devctx->OMSetRenderTargets(1, &_rtv_null, res.ds_dsv);
	devctx->DrawIndexedInstanced(
		6,
		data_e.wvp.get_size(),
		0,
		0,
		0
	);
	devctx->VSSetShader(0, 0, 0);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShaderResources(0, 1, &_srv_null);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);
}
void Graphics::downsample_ds() {
	ID3D11Buffer*const _buf_null = 0;
	ID3D11RenderTargetView*const _rtv_null = 0;
	ID3D11DepthStencilView*const _dsv_null = 0;
	ID3D11ShaderResourceView*const _srv_null = 0;
	ID3D11UnorderedAccessView*const _uav_null = 0;
	D3D11_TEXTURE2D_DESC _ds_desc, _occl_rep_desc;
	res.occl_rep_tex2->GetDesc(&_occl_rep_desc);
	uint32_t _width = _occl_rep_desc.Width;
	uint32_t _height = _occl_rep_desc.Height;
	
	// resize ds to pow2 size
	res.ds_tex2->GetDesc(&_ds_desc);
	float _size_ratio[] = {
		float(_width) / _ds_desc.Width,
		float(_height) / _ds_desc.Height,
	};
	res.ob.update_const_buf(_size_ratio, 1);
	devctx->CSSetShader(res.cs[CS_RESIZE_DS_POW2], 0, 0);
	devctx->OMSetRenderTargets(1, &_rtv_null, _dsv_null);
	devctx->CSSetConstantBuffers(0, 1, &res.ob.const_buf);
	devctx->CSSetShaderResources(0, 1, &res.ds_srv);
	devctx->CSSetUnorderedAccessViews(0, 1, &res.occl_rep_mips_uav[0], 0);
	devctx->Dispatch(
		_ds_desc.Width - 1,
		_ds_desc.Height - 1,
		1
	);
	devctx->CSSetUnorderedAccessViews(0, 1, &_uav_null, 0);
	devctx->CSSetShaderResources(0, 1, &_srv_null);
	devctx->CSSetConstantBuffers(0, 1, &_buf_null);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);
	devctx->CSSetShader(0, 0, 0);

	// downsample
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	float const _color[] = {0.0f, 0.0f, 0.0f, 0.0f};
	devctx->ClearRenderTargetView(res.rtv, _color);
	devctx->CSSetShader(res.cs[CS_DOWNSAMPLE_OCCL_REP], 0, 0);
	for(uint32_t _mip = 1; _mip < _occl_rep_desc.MipLevels; ++_mip) {
		devctx->CSSetShaderResources(0, 1, &res.occl_rep_mips_srv[_mip-1]);
		devctx->CSSetUnorderedAccessViews(0, 1, &res.occl_rep_mips_uav[_mip], 0);
		_width /= 2;
		_height /= 2;
		if(_width == 0) _width = 1;
		if(_height == 0) _height = 1;
		devctx->Dispatch(
			_width,
			_height,
			1
		);
		devctx->CSSetUnorderedAccessViews(0, 1, &_uav_null, 0);
		devctx->CSSetShaderResources(0, 1, &_srv_null);
	}
	devctx->CSSetShader(0, 0, 0);

	// test
	XMFLOAT4 _vert[] = {
		XMFLOAT4(-1.0f, -1.0f, 0.5f, 1.0f),
		XMFLOAT4(-1.0f, 1.0f, 0.5f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f),
		XMFLOAT4(1.0f, -1.0f, 0.5f, 1.0f),
	};
	res.ob.update_vert(_vert, 4);
	XMFLOAT2 _coord_tex[] = {
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f),
	};
	res.ob.update_coord_tex(_coord_tex, 4);
	DWORD _idx[] = {
		0, 1, 2,
		0, 2, 3,
	};
	res.ob.update_idx(_idx, 6);
	ID3D11Buffer* _buf[] = {res.ob.vert_buf, res.ob.coord_tex_buf};
	uint32_t _strides[] = {sizeof(XMFLOAT4), sizeof(XMFLOAT2)};
	uint32_t _offsets[] = {0, 0};
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->IASetInputLayout(res.in_lay[IN_F4F2]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devctx->VSSetShader(res.vs[VS_F4F2_PASS], 0, 0);
	devctx->PSSetShader(res.ps[PS_SAMPLE_TEX], 0, 0);
	devctx->PSSetShaderResources(0, 1, &res.occl_rep_srv);
	devctx->DrawIndexed(
		6,
		0,
		0
	);
	devctx->PSSetShaderResources(0, 1, &_srv_null);
	devctx->VSSetShader(0, 0, 0);
	devctx->PSSetShader(0, 0, 0);
}
void Graphics::mark_occluders() {
	ID3D11UnorderedAccessView* _uav_null = 0;
	ID3D11ShaderResourceView* _srv_null = 0;

	devctx->CSSetShaderResources(0, 1, &res.ob.occl_rect_srv);
	devctx->CSSetShaderResources(1, 1, &res.occl_rep_srv);
	devctx->CSSetUnorderedAccessViews(0, 1, &res.ob.occluders_uav, 0);
	devctx->CSSetShader(res.cs[CS_MARK_OCCLUDERS], 0, 0);
	devctx->Dispatch(
		data_e.wvp.get_size(),
		1,
		1
	);
	devctx->CSSetShaderResources(0, 1, &_srv_null);
	devctx->CSSetShaderResources(1, 1, &_srv_null);
	devctx->CSSetUnorderedAccessViews(0, 1, &_uav_null, 0);
	devctx->CSSetShader(0, 0, 0);
}
void Graphics::draw_occl_map() {
	ID3D11Buffer* _buf_null[] = {0, 0};
	ID3D11ShaderResourceView* _srv_null = 0;

	// draw map to texture
	XMFLOAT3 _vert1[] = {XMFLOAT3(0.0f, 0.0f, 0.0f)};
	res.ob.update_vert(_vert1, 1);
	devctx->VSSetShader(res.vs[VS_F4F44_TFORM], 0, 0);
	devctx->PSSetShader(res.ps[PS_DRAW_DEPTH], 0, 0);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	devctx->IASetInputLayout(res.in_lay[IN_F3F44]);
	ID3D11Buffer* _buf[] = {res.ob.vert_buf, res.ob.wvp_map_buf};
	uint32_t _strides[] = {0, sizeof(XMFLOAT4X4)};
	uint32_t _offsets[] = {0, 0};
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	devctx->RSSetViewports(1, &res.viewport[VIEWPORT_MAP]);
	float const _color[] = {0.9f, 0.9f, 0.9f, 0.0f};
	devctx->ClearRenderTargetView(res.map_rtv, _color);
	devctx->OMSetRenderTargets(1, &res.map_rtv, 0);
	devctx->DrawInstanced(
		1,
		data_e.wvp_map.get_size(),
		0,
		0
	);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);
	devctx->RSSetViewports(1, &res.viewport[VIEWPORT_MAIN]);
	devctx->IASetVertexBuffers(0, 2, _buf_null, _strides, _offsets);
	devctx->PSSetShader(0, 0, 0);
	devctx->VSSetShader(0, 0, 0);

	// draw map
	XMFLOAT4 _vert[] = {
		XMFLOAT4(0.5f, -1.0f, 0.5f, 1.0f),
		XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),
		XMFLOAT4(1.0f, -0.5f, 0.5f, 1.0f),
		XMFLOAT4(1.0f, -1.0f, 0.5f, 1.0f),
	};
	res.ob.update_vert(_vert, 4);
	XMFLOAT2 _coord_tex[] = {
		XMFLOAT2(0.0f, 1.0f),
		XMFLOAT2(0.0f, 0.0f),
		XMFLOAT2(1.0f, 0.0f),
		XMFLOAT2(1.0f, 1.0f),
	};
	res.ob.update_coord_tex(_coord_tex, 4);
	DWORD _idx[] = {
		0, 1, 2,
		0, 2, 3,
	};
	_buf[0] = res.ob.vert_buf;
	_buf[1] = res.ob.coord_tex_buf;
	_strides[0] = sizeof(XMFLOAT4);
	_strides[1] = sizeof(XMFLOAT2);
	res.ob.update_idx(_idx, 6);
	devctx->ClearDepthStencilView(res.ds_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	devctx->VSSetShader(res.vs[VS_F4F2_PASS], 0, 0);
	devctx->PSSetShader(res.ps[PS_SAMPLE_TEX], 0, 0);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devctx->IASetInputLayout(res.in_lay[IN_F4F2]);
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->PSSetShaderResources(0, 1, &res.map_srv);
	devctx->DrawIndexed(
		6,
		0,
		0
	);
	devctx->PSSetShaderResources(0, 1, &_srv_null);
	devctx->IASetIndexBuffer(_buf_null[0], DXGI_FORMAT_R32_UINT, 0);
	devctx->IASetVertexBuffers(0, 2, _buf_null, _strides, _offsets);
	devctx->PSSetShader(0, 0, 0);
	devctx->VSSetShader(0, 0, 0);
}
void Graphics::apply_occl_rep() {
	devctx->IASetInputLayout(res.in_lay[IN_VOID]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DWORD _idx[] = {
		0, 1, 2,
		0, 2, 3,
	};
	res.ob.update_idx(_idx, 6);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShaderResources(0, 1, &res.ob.occl_rect_srv);
	devctx->VSSetShader(res.vs[VS_VOID_PASS_OCCL_RECT], 0, 0);
	devctx->PSSetShader(0, 0, 0);
	ID3D11RenderTargetView* _rtv = 0;
	devctx->OMSetRenderTargets(1, &_rtv, res.ds_dsv);
	devctx->DrawIndexedInstanced(
		6,
		data_e.wvp.get_size(),
		0,
		0,
		0
	);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	ID3D11ShaderResourceView* _srv = 0;
	devctx->VSSetShaderResources(0, 1, &_srv);
	devctx->VSSetShader(0, 0, 0);
	devctx->OMSetRenderTargets(1, &res.rtv, res.ds_dsv);
}
void Graphics::draw_bbox() {
	devctx->IASetInputLayout(res.in_lay[IN_F4F44]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ID3D11Buffer* _buf[] = {res.ob.bbox_buf, res.ob.wvp_buf};
	uint32_t _strides[] = {sizeof(XMFLOAT4), sizeof(XMFLOAT4X4)};
	uint32_t _offsets[] = {0, 0};
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	//DWORD _idx[] = {
	//	0, 1,
	//	1, 2,
	//	2, 3,
	//	3, 0,
	//	0, 4,
	//	1, 5,
	//	2, 6,
	//	3, 7,
	//	4, 5,
	//	5, 6,
	//	6, 7,
	//	7, 4,
	//};
	DWORD _idx[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 7,
		0, 7, 4,
		0, 4, 5,
		0, 5, 1,
		6, 2, 1,
		6, 1, 5,
		6, 5, 4,
		6, 4, 7,
		6, 7, 3,
		6, 3, 2,
	};
	res.ob.update_idx(_idx, 36);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShader(res.vs[VS_F4F44_TFORM], 0, 0);
	devctx->PSSetShader(res.ps[PS_DRAW_DEPTH], 0, 0);
	uint32_t _inst_cnt = get_gr_cnt(0);
	devctx->DrawIndexedInstanced(
		36,
		_inst_cnt,
		0,
		0,
		0
	);
	_buf[0] = 0;
	_buf[1] = 0;
	devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShader(0, 0, 0);
	devctx->PSSetShader(0, 0, 0);
}
void Graphics::draw_rect_front_frame() {
	devctx->IASetInputLayout(res.in_lay[IN_VOID]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	DWORD _idx[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
	};
	res.ob.update_idx(_idx, 8);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShaderResources(0, 1, &res.ob.occl_rect_srv);
	devctx->VSSetShader(res.vs[VS_VOID_PASS_OCCL_RECT], 0, 0);
	devctx->PSSetShader(res.ps[PS_DRAW_DEPTH], 0, 0);
	devctx->DrawIndexedInstanced(
		8,
		data_e.wvp.get_size(),
		0,
		0,
		0
	);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	ID3D11ShaderResourceView* _srv = 0;
	devctx->VSSetShaderResources(0, 1, &_srv);
	devctx->VSSetShader(0, 0, 0);
	devctx->PSSetShader(0, 0, 0);
}
void Graphics::draw_rect_front_face() {
	devctx->IASetInputLayout(res.in_lay[IN_VOID]);
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DWORD _idx[] = {
		0, 1, 2,
		0, 2, 3,
	};
	res.ob.update_idx(_idx, 6);
	devctx->IASetIndexBuffer(res.ob.idx_buf, DXGI_FORMAT_R32_UINT, 0);
	devctx->VSSetShaderResources(0, 1, &res.ob.occl_rect_srv);
	devctx->VSSetShader(res.vs[VS_VOID_PASS_OCCL_RECT], 0, 0);
	devctx->PSSetShader(res.ps[PS_DRAW_DEPTH], 0, 0);
	devctx->DrawIndexedInstanced(
		6,
		data_e.wvp.get_size(),
		0,
		0,
		0
	);
	devctx->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	ID3D11ShaderResourceView* _srv_null = 0;
	devctx->VSSetShaderResources(0, 1, &_srv_null);
	devctx->VSSetShader(0, 0, 0);
	devctx->PSSetShader(0, 0, 0);
}
uint32_t Graphics::get_gr_cnt(uint32_t _hnd_idx) const {
	uint32_t _cnt = 1;
	for(uint32_t _i = _hnd_idx + 1; _i < data_e.mesh_hnd.get_size(); ++_i) {
		if(data_e.mesh_hnd[_i-1] == data_e.mesh_hnd[_i] && data_e.tex_hnd[_i-1] == data_e.tex_hnd[_i])
			++_cnt;
	}
	return _cnt;
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




