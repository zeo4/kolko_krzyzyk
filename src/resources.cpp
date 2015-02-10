#pragma once
#include <resources.h>
#include <tasks.h>
#include <fstream>
// -------------------------------------------------------
vector<byte> read_bytes(string _file_path) {
	vector<byte> _bytes;
	std::fstream _file(_file_path, std::ios::in | std::ios::ate | std::ios::binary);
	if(_file.is_open()){
		_bytes.resize(_file.tellg());
		_file.seekg(0 , std::ios::beg);
		_file.read(reinterpret_cast<char*>(&_bytes[0]), _bytes.size());
		_file.close();
	}
	return _bytes;
}
// -------------------------------------------------------
void GraphDev::create_dev_ctx_chain() {
	if(dev != 0) return;

	DXGI_MODE_DESC _back_buf_desc;
	ZeroMemory(&_back_buf_desc, sizeof(_back_buf_desc));
	_back_buf_desc.Width = screen.width;
	_back_buf_desc.Height = screen.height;
	_back_buf_desc.RefreshRate.Numerator = 60;
	_back_buf_desc.RefreshRate.Denominator = 1;
	_back_buf_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	_back_buf_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_back_buf_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	DXGI_SWAP_CHAIN_DESC _chain_desc;
	ZeroMemory(&_chain_desc, sizeof(_chain_desc));
	_chain_desc.BufferDesc = _back_buf_desc;
	_chain_desc.SampleDesc.Count = 1;
	_chain_desc.SampleDesc.Quality = 0;
	_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_chain_desc.BufferCount = 1;
	_chain_desc.OutputWindow = uch_okno;
	_chain_desc.Windowed = true;
	_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	HRESULT _r = D3D11CreateDeviceAndSwapChain(
		0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG, 0, 0,
		D3D11_SDK_VERSION, &_chain_desc, &chain, &dev, 0, &devctx
	);
	if(_r != S_OK) logi.pisz("", "nie stworzono karty devctx chain");

	_r = dev->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);
	if(_r != S_OK) logi.pisz("", "failed to create debug interface");
	_r = debug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&debug_info);
	if(_r != S_OK) logi.pisz("", "failed to create debug info queue");
	_r = debug_info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
	if(_r != S_OK) logi.pisz("", "failed to set debug break on corruption level");
	_r = debug_info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
	if(_r != S_OK) logi.pisz("", "failed to set debug break on error level");

	//D3D11_MESSAGE_ID _msg_hide[] = {
	//	D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
	//};
	//D3D11_INFO_QUEUE_FILTER _msg_filter;
	//memset(&_msg_filter, 0, sizeof(_msg_filter));
	//_msg_filter.DenyList.NumIDs = 1;
	//_msg_filter.DenyList.pIDList = _msg_hide;
	//debug_info->AddStorageFilterEntries(&_msg_filter);

	_r = DXGIGetDebugInterface1(0, __uuidof(g_analysis), reinterpret_cast<void**>(&g_analysis));
	if(_r != S_OK) logi.pisz("", "failed to create graphics analysis interface");
}
void GraphDev::destroy_dev_ctx_chain() {
	dev->Release();
	chain->Release();
	devctx->Release();
	debug_info->Release();
	//HRESULT _r = debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	//if(_r != S_OK) logi.pisz("", "failed to set report live device objects details level");
	debug->Release();
}
GraphDev::Screen GraphDev::screen = {640, 480};
ID3D11Device* GraphDev::dev = 0;
IDXGISwapChain* GraphDev::chain = 0;
ID3D11DeviceContext* GraphDev::devctx = 0;
ID3D11Debug* GraphDev::debug = 0;
ID3D11InfoQueue* GraphDev::debug_info = 0;
IDXGraphicsAnalysis* GraphDev::g_analysis = 0;
// -------------------------------------------------------
GraphR::ObGroup::ObGroup()
	: wvp_buf(0),
	wvp_srv(0),
	wvp_map_buf(0),
	bbox_buf(0),
	bbox_srv(0),
	occl_rect_buf(0),
	occl_rect_uav(0),
	occl_rect_srv(0),
	occluders_buf(0),
	occluders_uav(0),
	occluders_srv(0),
	vert_buf(0),
	vert_uav(0),
	coord_tex_buf(0),
	idx_buf(0),
	const_buf(0),
	so_buf(0) {}
GraphR::ObGroup::~ObGroup() {
	wvp_tposed.destroy();
	if(wvp_buf != 0) wvp_buf->Release();
	if(wvp_srv != 0) wvp_srv->Release();
	if(wvp_map_buf != 0) wvp_map_buf->Release();
	if(bbox_buf != 0) bbox_buf->Release();
	if(bbox_srv != 0) bbox_srv->Release();
	if(occl_rect_buf != 0) occl_rect_buf->Release();
	if(occl_rect_uav != 0) occl_rect_uav->Release();
	if(occl_rect_srv != 0) occl_rect_srv->Release();
	if(occluders_buf != 0) occluders_buf->Release();
	if(occluders_uav != 0) occluders_uav->Release();
	if(occluders_srv != 0) occluders_srv->Release();
	if(vert_buf != 0) vert_buf->Release();
	if(vert_uav != 0) vert_uav->Release();
	if(coord_tex_buf != 0) coord_tex_buf->Release();
	if(idx_buf != 0) idx_buf->Release();
	if(const_buf != 0) const_buf->Release();
	if(so_buf != 0) so_buf->Release();
}
void GraphR::ObGroup::update_wvp(void*const*const _data, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(wvp_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else wvp_buf->GetDesc(&_buf_desc);
	if(_size * sizeof(XMFLOAT4X4) > _buf_desc.ByteWidth) {
		// main view wvp buf
		_buf_desc.ByteWidth = _size * sizeof(XMFLOAT4X4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(wvp_buf != 0) wvp_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &wvp_buf);
		if(_r != S_OK) logi.pisz("", "failed to create wvp_buf");

		// main view wvp srv
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size * 4;
		if(wvp_srv != 0) wvp_srv->Release();
		_r = dev->CreateShaderResourceView(wvp_buf, &_srv_desc, &wvp_srv);
		if(_r != S_OK) logi.pisz("", "failed to create wvp_srv");

		// map view wvp buf
		_buf_desc.ByteWidth = _size * sizeof(XMFLOAT4X4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(wvp_map_buf != 0) wvp_map_buf->Release();
		_r = dev->CreateBuffer(&_buf_desc, 0, &wvp_map_buf);
		if(_r != S_OK) logi.pisz("", "failed to create wvp_map_buf");
	}

	// main view wvp update
	if(_data[0] != 0) {
		wvp_tposed.erase(0, wvp_tposed.get_size());
		XMFLOAT4X4 _mtx_tmp = XMFLOAT4X4();
		for(uint32_t _i = 0; _i < _size; ++_i) {
			wvp_tposed.push_back(_mtx_tmp);
			XMStoreFloat4x4(
				&wvp_tposed[_i],
				XMMatrixTranspose(XMLoadFloat4x4(&((XMFLOAT4X4**)_data)[0][_i]))
			);
		}
		devctx->UpdateSubresource(wvp_buf, 0, 0, &wvp_tposed[0], 0, 0);
	}

	// map view wvp update
	if(_data[1] != 0) {
		wvp_tposed.erase(0, wvp_tposed.get_size());
		XMFLOAT4X4 _mtx_tmp = XMFLOAT4X4();
		for(uint32_t _i = 0; _i < _size; ++_i) {
			wvp_tposed.push_back(_mtx_tmp);
			XMStoreFloat4x4(
				&wvp_tposed[_i],
				XMMatrixTranspose(XMLoadFloat4x4(&((XMFLOAT4X4**)_data)[1][_i]))
			);
		}
		devctx->UpdateSubresource(wvp_map_buf, 0, 0, &wvp_tposed[0], 0, 0);
	}
}
void GraphR::ObGroup::update_bbox(XMFLOAT4 const*const _data, uint32_t const _size) {
	if(_size == 0) return;
	D3D11_BUFFER_DESC _buf_desc;

	// vertices
	if(bbox_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else bbox_buf->GetDesc(&_buf_desc);
	if(_size * 8 * sizeof(XMFLOAT4) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * 8 * sizeof(XMFLOAT4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(bbox_buf != 0) bbox_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &bbox_buf);
		if(_r != S_OK) logi.pisz("", "failed to create bbox_buf");

		// srv
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size * 8;
		if(bbox_srv != 0) bbox_srv->Release();
		_r = dev->CreateShaderResourceView(bbox_buf, &_srv_desc, &bbox_srv);
		if(_r != S_OK) logi.pisz("", "failed to create wvp srv");
	}
	if(_data != 0) devctx->UpdateSubresource(bbox_buf, 0, 0, _data, 0, 0);
}
void GraphR::ObGroup::update_coord_tex(XMFLOAT2 const*const _coord_tex, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(coord_tex_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else coord_tex_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(XMFLOAT2) > _buf_desc.ByteWidth) {
		_buf_desc.ByteWidth = _size * sizeof(XMFLOAT2);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(coord_tex_buf != 0) coord_tex_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &coord_tex_buf);
		if(_r != S_OK) logi.pisz("", "failed to create texture coordinate buffer");
	}
	if(_coord_tex != 0) devctx->UpdateSubresource(coord_tex_buf, 0, 0, _coord_tex, 0, 0);
}
void GraphR::ObGroup::update_idx(DWORD const*const _data, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(idx_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else idx_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(DWORD) > _buf_desc.ByteWidth) {
		_buf_desc.ByteWidth = _size * sizeof(DWORD);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(idx_buf != 0) idx_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &idx_buf);
		if(_r != S_OK) logi.pisz("", "failed to create index buffer");
	}
	if(_data != 0) devctx->UpdateSubresource(idx_buf, 0, 0, _data, 0, 0);
}
void GraphR::ObGroup::update_occl_rect(XMFLOAT4 const*const _data, uint32_t const _size) {
	if(_size == 0) return;
	D3D11_BUFFER_DESC _buf_desc;

	if(occl_rect_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else occl_rect_buf->GetDesc(&_buf_desc);
	if(_size * 4 * sizeof(XMFLOAT4) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * 4 * sizeof(XMFLOAT4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(occl_rect_buf != 0) occl_rect_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &occl_rect_buf);
		if(_r != S_OK) logi.pisz("", "failed to create occl_rect_buf");

		// uav
		D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
		_uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		_uav_desc.Buffer.FirstElement = 0;
		_uav_desc.Buffer.NumElements = _size * 4;
		_uav_desc.Buffer.Flags = 0;
		if(occl_rect_uav != 0) occl_rect_uav->Release();
		_r = dev->CreateUnorderedAccessView(occl_rect_buf, &_uav_desc, &occl_rect_uav);
		if(_r != S_OK) logi.pisz("", "failed to create occl_rect_uav");

		// srv
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size * 4;
		if(occl_rect_srv != 0) occl_rect_srv->Release();
		_r = dev->CreateShaderResourceView(occl_rect_buf, &_srv_desc, &occl_rect_srv);
		if(_r != S_OK) logi.pisz("", "failed to create occl_rect_srv");
	}
	if(_data != 0) devctx->UpdateSubresource(occl_rect_buf, 0, 0, _data, 0, 0);
}
void GraphR::ObGroup::update_occluders(bool const*const _data, uint32_t const _size) {
	if(_size == 0) return;
	D3D11_BUFFER_DESC _buf_desc;

	if(occluders_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else occluders_buf->GetDesc(&_buf_desc);
	if(_size * sizeof(bool) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * 4;
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(occluders_buf != 0) occluders_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &occluders_buf);
		if(_r != S_OK) logi.pisz("", "failed to create occluders_buf");

		// uav
		D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
		_uav_desc.Format = DXGI_FORMAT_R32_UINT;
		_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		_uav_desc.Buffer.FirstElement = 0;
		_uav_desc.Buffer.NumElements = _size;
		_uav_desc.Buffer.Flags = 0;
		if(occluders_uav != 0) occluders_uav->Release();
		_r = dev->CreateUnorderedAccessView(occluders_buf, &_uav_desc, &occluders_uav);
		if(_r != S_OK) logi.pisz("", "failed to create occluders_uav");

		// srv
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32_UINT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size;
		if(occluders_srv != 0) occluders_srv->Release();
		_r = dev->CreateShaderResourceView(occluders_buf, &_srv_desc, &occluders_srv);
		if(_r != S_OK) logi.pisz("", "failed to create occluders_srv");
	}
	if(_data != 0) devctx->UpdateSubresource(occluders_buf, 0, 0, _data, 0, 0);
}
void GraphR::ObGroup::update_vert(XMFLOAT3 const*const _vert, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(vert_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else vert_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(XMFLOAT3) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * sizeof(XMFLOAT3);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(vert_buf != 0) vert_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &vert_buf);
		if(_r != S_OK) logi.pisz("", "failed to create vertex buffer");
	}
	if(_vert != 0) devctx->UpdateSubresource(vert_buf, 0, 0, _vert, 0, 0);
}
void GraphR::ObGroup::update_vert(XMFLOAT4 const*const _vert, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(vert_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else vert_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(XMFLOAT4) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * sizeof(XMFLOAT4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(vert_buf != 0) vert_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &vert_buf);
		if(_r != S_OK) logi.pisz("", "failed to create vertex buffer");
	}
	if(_vert != 0) devctx->UpdateSubresource(vert_buf, 0, 0, _vert, 0, 0);
}
void GraphR::ObGroup::update_const_buf(void const*const _data, uint32_t const _size) {
	if(_data == 0) return;
	D3D11_BUFFER_DESC _buf_desc;
	
	// buf
	_buf_desc.ByteWidth = _size * 16;
	_buf_desc.Usage = D3D11_USAGE_DEFAULT;
	_buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	_buf_desc.CPUAccessFlags = 0;
	_buf_desc.MiscFlags = 0;
	if(const_buf != 0) const_buf->Release();
	HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &const_buf);
	if(_r != S_OK) logi.pisz("", "failed to create const_buf");
	devctx->UpdateSubresource(const_buf, 0, 0, _data, 0, 0);
}
void GraphR::ObGroup::update_so(uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(so_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else so_buf->GetDesc(&_buf_desc);

	if(_size * sizeof(XMFLOAT4) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * sizeof(XMFLOAT4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(so_buf != 0) so_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &so_buf);
		if(_r != S_OK) logi.pisz("", "failed to create stream output buffer");
	}
}
void GraphR::ObGroup::bind_vert(uint32_t const _nr_szad) const {
	switch(_nr_szad) {
	case VS_PASS_ON: {
		ID3D11Buffer* _buf[] = {vert_buf, coord_tex_buf};
		uint32_t _strides[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT2)};
		uint32_t _offsets[] = {0, 0};
		devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
		break;
	}
	case VS_TFORM: {
		ID3D11Buffer* _buf[] = {vert_buf, wvp_buf};
		uint32_t _strides[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT4X4)};
		uint32_t _offsets[] = {0, 0, 0};
		devctx->IASetVertexBuffers(0, 2, _buf, _strides, _offsets);
		break;
	}
	case TEST_VS_RECT_OCCL: {
		uint32_t _strides[] = {sizeof(XMFLOAT3)};
		uint32_t _offsets[] = {0};
		devctx->IASetVertexBuffers(0, 1, &vert_buf, _strides, _offsets);
	}
	}
}
// -------------------------------------------------------
GraphR::GraphR()
	: scr_size_buf(0),
	rtv(0),
	ds_tex2(0),
	ds_dsv(0),
	ds_srv(0),
	occl_rep_tex2(0),
	occl_rep_srv(0),
	map_tex2(0),
	map_rtv(0),
	map_srv(0),
	ss(0) {}
GraphR::~GraphR() {
	scr_size_buf->Release();
	rtv->Release();
	ds_tex2->Release();
	ds_dsv->Release();
	ds_srv->Release();
	occl_rep_tex2->Release();
	occl_rep_mips_uav.destroy();
	occl_rep_srv->Release();
	map_tex2->Release();
	map_rtv->Release();
	map_srv->Release();
	for(uint32_t _i = 0; _i < cs.get_size(); ++_i) {
		cs[_i]->Release();
	}
	for(uint32_t _i = 0; _i < in_lay.get_size(); ++_i) {
		in_lay[_i]->Release();
	}
	for(uint32_t _i = 0; _i < vs.get_size(); ++_i) {
		vs[_i]->Release();
	}
	for(uint32_t _i = 0; _i < gs.get_size(); ++_i) {
		gs[_i]->Release();
	}
	for(uint32_t _i = 0; _i < ps.get_size(); ++_i) {
		ps[_i]->Release();
	}
	ss->Release();
}
void GraphR::bind_ss() const {
	devctx->PSSetSamplers(0, 1, &ss);
}
void GraphR::create_scr_size() {
	D3D11_BUFFER_DESC _scr_size_desc;
	memset(&_scr_size_desc, 0, sizeof(_scr_size_desc));
	_scr_size_desc.ByteWidth = 16 * (sizeof(screen) / 16) + 16; // multiple of 16
	_scr_size_desc.Usage = D3D11_USAGE_DEFAULT;
	_scr_size_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA _scr_size_data;
	memset(&_scr_size_data, 0, sizeof(_scr_size_data));
	_scr_size_data.pSysMem = &screen;
	HRESULT _r = dev->CreateBuffer(&_scr_size_desc, &_scr_size_data, &scr_size_buf);
	if(_r != S_OK) logi.pisz("", "failed to create scr_size_buf");
}
void GraphR::create_rtv() {
	ID3D11Texture2D* _back_buf_tex2;

	// RTV
	HRESULT _r = chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&_back_buf_tex2);
	if(_r != S_OK) logi.pisz("", "nie pobrano opisu bufora tylnego");
	_r = dev->CreateRenderTargetView(_back_buf_tex2, 0, &rtv);

	_back_buf_tex2->Release();
}
void GraphR::create_ds() {
	// depth stencil

	// tex
	D3D11_TEXTURE2D_DESC _tex2_desc;
	memset(&_tex2_desc, 0, sizeof(_tex2_desc));
	_tex2_desc.Width = screen.width;
	_tex2_desc.Height = screen.height;
	_tex2_desc.MipLevels = 1;
	_tex2_desc.ArraySize = 1;
	_tex2_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	_tex2_desc.SampleDesc.Count = 1;
	_tex2_desc.SampleDesc.Quality = 0;
	_tex2_desc.Usage = D3D11_USAGE_DEFAULT;
	_tex2_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	_tex2_desc.CPUAccessFlags = 0;
	_tex2_desc.MiscFlags = 0;
	HRESULT _r = dev->CreateTexture2D(&_tex2_desc, 0, &ds_tex2);
	if(_r != S_OK) logi.pisz("", "failed to create ds_tex2");

	// dsv
	D3D11_DEPTH_STENCIL_VIEW_DESC _ds_desc;
	memset(&_ds_desc, 0, sizeof(_ds_desc));
	//_ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_ds_desc.Format = DXGI_FORMAT_D32_FLOAT;
	_ds_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	_ds_desc.Texture2D.MipSlice = 0;
	_r = dev->CreateDepthStencilView(ds_tex2, &_ds_desc, &ds_dsv);
	if(_r != S_OK) logi.pisz("", "failed to create ds_dsv");

	// srv
	D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
	_srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	_srv_desc.Texture2D.MostDetailedMip = 0;
	_srv_desc.Texture2D.MipLevels = -1;
	_r = dev->CreateShaderResourceView(ds_tex2, &_srv_desc, &ds_srv);
	if(_r != S_OK) logi.pisz("", "failed to create ds_srv");

	// occlusion representation

	// occlusiono representation size
	uint32_t _width = pow(2, (uint32_t)log2(screen.width));
	uint32_t _height = pow(2, (uint32_t)log2(screen.height));

	// tex
	memset(&_tex2_desc, 0, sizeof(_tex2_desc));
	_tex2_desc.Width = _width;
	_tex2_desc.Height = _height;
	_tex2_desc.MipLevels = 0;
	_tex2_desc.ArraySize = 1;
	_tex2_desc.Format = DXGI_FORMAT_R32_FLOAT;
	_tex2_desc.SampleDesc.Count = 1;
	_tex2_desc.SampleDesc.Quality = 0;
	_tex2_desc.Usage = D3D11_USAGE_DEFAULT;
	_tex2_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	_tex2_desc.CPUAccessFlags = 0;
	_tex2_desc.MiscFlags = 0;
	_r = dev->CreateTexture2D(&_tex2_desc, 0, &occl_rep_tex2);
	if(_r != S_OK) logi.pisz("", "failed to create occl_rep_tex2");

	// uav and srv (1 per mip)
	D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
	_uav_desc.Format = DXGI_FORMAT_R32_FLOAT;
	_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	memset(&_srv_desc, 0, sizeof(_srv_desc));
	_srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	_srv_desc.Texture2D.MipLevels = 1;
	occl_rep_tex2->GetDesc(&_tex2_desc);
	for(uint32_t _mip = 0; _mip < _tex2_desc.MipLevels; ++_mip) {
		occl_rep_mips_uav.push_back(0);
		occl_rep_mips_srv.push_back(0);
		_uav_desc.Texture2D.MipSlice = _mip;
		_srv_desc.Texture2D.MostDetailedMip = _mip;
		_r = dev->CreateUnorderedAccessView(occl_rep_tex2, &_uav_desc, &occl_rep_mips_uav[_mip]);
		if(_r != S_OK) logi.pisz("", "failed to create occl_rep_mips_uav[" + to_string(_mip) + "]");
		_r = dev->CreateShaderResourceView(occl_rep_tex2, &_srv_desc, &occl_rep_mips_srv[_mip]);
		if(_r != S_OK) logi.pisz("", "failed to create occl_rep_mips_srv[" + to_string(_mip) + "]");
	}

	// srv (all mips)
	memset(&_srv_desc, 0, sizeof(_srv_desc));
	_srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	_srv_desc.Texture2D.MostDetailedMip = 0;
	_srv_desc.Texture2D.MipLevels = -1;
	_r = dev->CreateShaderResourceView(occl_rep_tex2, &_srv_desc, &occl_rep_srv);
	if(_r != S_OK) logi.pisz("", "failed to create occl_rep_srv");
}
void GraphR::create_map() {
	// tex
	D3D11_TEXTURE2D_DESC _tex2_desc;
	memset(&_tex2_desc, 0, sizeof(_tex2_desc));
	_tex2_desc.Width = screen.width/4;
	_tex2_desc.Height = screen.height/4;
	_tex2_desc.MipLevels = 1;
	_tex2_desc.ArraySize = 1;
	_tex2_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	_tex2_desc.SampleDesc.Count = 1;
	_tex2_desc.SampleDesc.Quality = 0;
	_tex2_desc.Usage = D3D11_USAGE_DEFAULT;
	_tex2_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	_tex2_desc.CPUAccessFlags = 0;
	_tex2_desc.MiscFlags = 0;
	HRESULT _r = dev->CreateTexture2D(&_tex2_desc, 0, &map_tex2);
	if(_r != S_OK) logi.pisz("", "failed to create map_tex2");

	// rtv
	D3D11_RENDER_TARGET_VIEW_DESC _rtv_desc;
	_rtv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	_rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	_rtv_desc.Texture2D.MipSlice = 0;
	_r = dev->CreateRenderTargetView(map_tex2, &_rtv_desc, &map_rtv);
	if(_r != S_OK) logi.pisz("", "failed to create map_rtv");

	// srv
	D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
	_srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	_srv_desc.Texture2D.MostDetailedMip = 0;
	_srv_desc.Texture2D.MipLevels = 1;
	_r = dev->CreateShaderResourceView(map_tex2, &_srv_desc, &map_srv);
	if(_r != S_OK) logi.pisz("", "failed to create map_srv");
}
void GraphR::create_viewport() {
	// MAIN
	viewport.push_back(D3D11_VIEWPORT());
	memset(&viewport[VIEWPORT_MAIN], 0, sizeof(D3D11_VIEWPORT));
	viewport[VIEWPORT_MAIN].TopLeftX = 0;
	viewport[VIEWPORT_MAIN].TopLeftY = 0;
	viewport[VIEWPORT_MAIN].Width = screen.width;
	viewport[VIEWPORT_MAIN].Height = screen.height;
	viewport[VIEWPORT_MAIN].MinDepth = 0.0f;
	viewport[VIEWPORT_MAIN].MaxDepth = 1.0f;

	// MAP
	viewport.push_back(D3D11_VIEWPORT());
	memset(&viewport[VIEWPORT_MAP], 0, sizeof(D3D11_VIEWPORT));
	viewport[VIEWPORT_MAP].TopLeftX = 0;
	viewport[VIEWPORT_MAP].TopLeftY = 0;
	viewport[VIEWPORT_MAP].Width = screen.width/4;
	viewport[VIEWPORT_MAP].Height = screen.height/4;
	viewport[VIEWPORT_MAP].MinDepth = 0.0f;
	viewport[VIEWPORT_MAP].MaxDepth = 1.0f;
}
void GraphR::create_cs() {
	vector<byte> _shad_bytes;
	HRESULT _r;

	// CS_CREATE_OCCL_SHAPE
	cs.push_back(0);
	_shad_bytes = read_bytes("shader\\cs_create_occl_shape.cso");
	_r = dev->CreateComputeShader(&_shad_bytes[0], _shad_bytes.size(), 0, &cs[CS_CREATE_OCCL_SHAPE]);
	if(_r != S_OK) logi.pisz("", "failed to create compute shader");

	// CS_MARK_OCCLUDERS
	cs.push_back(0);
	_shad_bytes = read_bytes("shader\\cs_mark_occluders.cso");
	_r = dev->CreateComputeShader(&_shad_bytes[0], _shad_bytes.size(), 0, &cs[CS_MARK_OCCLUDERS]);
	if(_r != S_OK) logi.pisz("", "failed to create compute shader");

	// CS_RESIZE_DS_POW2
	cs.push_back(0);
	_shad_bytes = read_bytes("shader\\cs_resize_ds_pow2.cso");
	_r = dev->CreateComputeShader(&_shad_bytes[0], _shad_bytes.size(), 0, &cs[CS_RESIZE_DS_POW2]);
	if(_r != S_OK) logi.pisz("", "failed to create compute shader");

	// CS_DOWNSAMPLE_OCCL_REP
	cs.push_back(0);
	_shad_bytes = read_bytes("shader\\cs_downsample_occl_rep.cso");
	_r = dev->CreateComputeShader(&_shad_bytes[0], _shad_bytes.size(), 0, &cs[CS_DOWNSAMPLE_OCCL_REP]);
	if(_r != S_OK) logi.pisz("", "failed to create compute shader");
}
void GraphR::create_in_lay() {
	HRESULT _r;
	vector<byte> _shad_bytes;
	{
		// IN_VOID
		D3D11_INPUT_ELEMENT_DESC _desc[1];
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_void_pass_occl_rect.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, 0, &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_VOID]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}{
		// IN_F4
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_f4_pass.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F4]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}{
		// IN_F4F2
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_f4f2_pass.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F4F2]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}{
		// IN_F3F2
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_pass_on.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F3F2]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}{
		// IN_F3F44
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_f4f44_tform.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F3F44]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}{
		// IN_F4F44
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_f4f44_tform.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F4F44]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}{
		// IN_F3F2F44
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_tform.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F3F2F44]);
		if(_r != S_OK) logi.pisz("", "input structure not created");
	}{
		// IN_F4F2F44
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_f4f2f44_tform.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F4F2F44]);
		if(_r != S_OK) logi.pisz("", "input structure not created");
	}
}
void GraphR::create_vs() {
	vector<byte> _shad_bytes;
	HRESULT _r;

	// VS_VOID_PASS_OCCL_RECT
	vs.push_back(0);
	_shad_bytes = read_bytes("shader\\vs_void_pass_occl_rect.cso");
	_r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_VOID_PASS_OCCL_RECT]
	);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// VS_F4_PASS
	vs.push_back(0);
	_shad_bytes = read_bytes("shader\\vs_f4_pass.cso");
	_r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_F4_PASS]
	);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// VS_F4F2_PASS
	vs.push_back(0);
	_shad_bytes = read_bytes("shader\\vs_f4f2_pass.cso");
	_r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_F4F2_PASS]
	);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// VS_F4F44_TFORM
	vs.push_back(0);
	_shad_bytes = read_bytes("shader\\vs_f4f44_tform.cso");
	_r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_F4F44_TFORM]
	);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// VS_F4F2F44_TFORM
	vs.push_back(0);
	_shad_bytes = read_bytes("shader\\vs_f4f2f44_tform.cso");
	_r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_F4F2F44_TFORM]
	);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// VS_PASS_ON
	vs.push_back(0);
	_shad_bytes = read_bytes("shader\\vs_pass_on.cso");
	_r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_PASS_ON]
	);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// VS_TFORM
	vs.push_back(0);
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\vs_tform.cso");
	_r = dev->CreateVertexShader(&_shad_bytes[0], _shad_bytes.size(), 0, &vs[VS_TFORM]);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");

	// TEST_VS_RECT_OCCL
	vs.push_back(0);
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\test_vs_rect_occl.cso");
	_r = dev->CreateVertexShader(&_shad_bytes[0], _shad_bytes.size(), 0, &vs[TEST_VS_RECT_OCCL]);
	if(_r != S_OK) logi.pisz("", "failed to create vertex shader");
}
void GraphR::create_gs() {
	vector<byte> _shad_bytes;
	HRESULT _r;

	// GS_GEN_OCCL_RECT_FRAME
	{
		D3D11_SO_DECLARATION_ENTRY _so_lay[] = {
			{0, "SV_Position", 0, 0, 4, 0},
		};
		uint32_t a = sizeof(_so_lay);
		_shad_bytes = read_bytes("shader\\gs_gen_occl_rect_frame.cso");
		gs.push_back(0);
		_r = dev->CreateGeometryShaderWithStreamOutput(&_shad_bytes[0], _shad_bytes.size(), _so_lay, 1, 0, 0, 0, 0, &gs[GS_GEN_OCCL_RECT_FRAME]);
		if(_r != S_OK) logi.pisz("", "failed to create geometry shader");
	}
}
void GraphR::create_ps() {
	vector<byte> _shad_bytes;
	HRESULT _r;

	// PS_SAMPLE_TEX
	_shad_bytes = read_bytes("shader\\ps_sample_tex.cso");
	ps.push_back(0);
	_r = dev->CreatePixelShader(&_shad_bytes[0], _shad_bytes.size(), 0, &ps[PS_SAMPLE_TEX]);
	if(_r != S_OK) logi.pisz("", "failed to create pixel shader");

	// PS_DRAW_DEPTH
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\ps_draw_depth.cso");
	ps.push_back(0);
	_r = dev->CreatePixelShader(&_shad_bytes[0], _shad_bytes.size(), 0, &ps[PS_DRAW_DEPTH]);
	if(_r != S_OK) logi.pisz("", "failed to create pixel shader");

	// TEST_PS_RECT_OCCL
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\test_ps_rect_occl.cso");
	ps.push_back(0);
	_r = dev->CreatePixelShader(&_shad_bytes[0], _shad_bytes.size(), 0, &ps[TEST_PS_RECT_OCCL]);
	if(_r != S_OK) logi.pisz("", "failed to create pixel shader");
}
void GraphR::create_ss() {
	D3D11_SAMPLER_DESC stan_prob_opis;
	ZeroMemory(&stan_prob_opis, sizeof(stan_prob_opis));
	stan_prob_opis.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	stan_prob_opis.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.ComparisonFunc = D3D11_COMPARISON_NEVER;
	stan_prob_opis.MinLOD = 0;
	stan_prob_opis.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT w = dev->CreateSamplerState(&stan_prob_opis, &ss);
	if(w != S_OK) logi.pisz("", "failed to create sampler state");
}
GraphR GraphRes::res;
// -------------------------------------------------------
Meshes::~Meshes() {
	no.destroy();
	ref_cnt.destroy();
	bbox.destroy();
	vert.destroy();
	tex_coord.destroy();
	vert_idx.destroy();
}
void Meshes::insert(uint32_t const _id, void*const*const _data, uint32_t const*const _size) {
	if(no[_id] != no.empty) {
		++ref_cnt[_id];
		return;
	}
	no[_id] = vert.get_col_size();
	ref_cnt[_id] = 1;
	bbox.push_back((XMFLOAT4*)_data[0], _size[0]);
	vert.push_back((XMFLOAT3*)_data[1], _size[1]);
	tex_coord.push_back((XMFLOAT2*)_data[2], _size[2]);
	vert_idx.push_back((DWORD*)_data[3], _size[3]);
}
void Meshes::erase(uint32_t const _id) {
	if(no[_id] == no.empty)
		return;
	else if(ref_cnt[_id] > 1) {
		--ref_cnt[_id];
		return;
	}
	bbox.erase(no[_id]);
	vert.erase(no[_id]);
	tex_coord.erase(no[_id]);
	vert_idx.erase(no[_id]);
	no[_id] = no.empty;
	ref_cnt[_id] = 0;
}
// -------------------------------------------------------
Textures::~Textures() {
	no.destroy();
	ref_cnt.destroy();
	for(uint32_t _i = 0; _i < tex.get_size(); ++_i) {
		if(tex[_i] != tex.empty) {
			tex[_i]->Release();
			srv[_i]->Release();
		}
	}
	tex.destroy();
	srv.destroy();
}
void Textures::insert(uint32_t const _id, void*const*const _data) {
	if(no[_id] != no.empty) {
		++ref_cnt[_id];
		return;
	}
	no[_id] = srv.get_size();
	ref_cnt[_id] = 1;
	tex.push_back((ID3D11Texture2D*)_data[0]);
	srv.push_back((ID3D11ShaderResourceView*)_data[1]);
}
void Textures::erase(uint32_t const _id) {
	if(no[_id] == no.empty)
		return;
	else if(ref_cnt[_id] > 1) {
		--ref_cnt[_id];
		return;
	}
	tex.erase(no[_id]);
	srv.erase(no[_id]);
	no[_id] = no.empty;
	ref_cnt[_id] = 0;
}
// -------------------------------------------------------
DataEngine::Data::~Data() {
	no.destroy();
	loc.destroy();
	v.destroy();
	world.destroy();
	wvp.destroy();
	occluder.destroy();
	mesh_hnd.destroy();
	tex_hnd.destroy();
}
DataEngine::Data DataEngine::data_e;
// -------------------------------------------------------
Cam::Cam(XMFLOAT4 _q, XMFLOAT3 _loc, float _fov, float _near, float _far)
	: q(_q),
	loc(_loc),
	fov(_fov),
	near_z(_near),
	far_z(_far) {}
Cam Camera::cam(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -0.5f), 90.0f, 0.5f, 100.0f);
Cam Camera::cam_map(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -0.5f), 90.0f, 0.5f, 10.0f);
// -------------------------------------------------------

//void GraphR::create_buf_struct() {
//	//struct BufferStruct { UINT color[4]; };
//	//D3D11_BUFFER_DESC sbDesc;
//	//sbDesc.ByteWidth = sizeof(BufferStruct) * THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
//	//sbDesc.Usage = D3D11_USAGE_DEFAULT;
//	//sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
//	//sbDesc.CPUAccessFlags = 0;
//	//sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
//	//sbDesc.StructureByteStride = sizeof(BufferStruct);
//	////InitData.pSysMem = NULL;
//	//HRESULT hr = dev->CreateBuffer(&sbDesc, NULL, &buf_struct);
//
//	//D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
//	//sbUAVDesc.Buffer.FirstElement = 0;
//	//sbUAVDesc.Buffer.Flags = 0;
//	//sbUAVDesc.Buffer.NumElements = THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
//	//sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
//	//sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
//	//hr = dev->CreateUnorderedAccessView(buf_struct, &sbUAVDesc, &g_pStructuredBufferUAV);
//
//	//D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc;
//	//sbSRVDesc.Buffer.ElementOffset = 0;
//	//sbSRVDesc.Buffer.ElementWidth = sizeof(BufferStruct);
//	//sbSRVDesc.Buffer.FirstElement = 0;
//	//sbSRVDesc.Buffer.NumElements = THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
//	//sbSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
//	//sbSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
//	//hr = dev->CreateShaderResourceView(buf_struct, &sbSRVDesc, &g_pStructuredBufferSRV);
//
//	//UINT initCounts = 0;
//	//devctx->CSSetUnorderedAccessViews(0, 1, &g_pStructuredBufferUAV, &initCounts);
//	//devctx->CSSetShader(shad_compute[CS_OCCLUDE], NULL, 0);
//	//devctx->Dispatch(THREAD_GROUPS_X, THREAD_GROUPS_Y, 1);
//	//ID3D11UnorderedAccessView* pNullUAV = NULL;
//	//devctx->CSSetUnorderedAccessViews(0, 1, &pNullUAV, &initCounts);
//}



