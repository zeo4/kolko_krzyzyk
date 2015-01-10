﻿#pragma once
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
	_back_buf_desc.Width = scr_size.width;
	_back_buf_desc.Height = scr_size.height;
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

	//_r = DXGIGetDebugInterface1(0, __uuidof(g_analysis), reinterpret_cast<void**>(&g_analysis));
	//if(_r != S_OK) logi.pisz("", "failed to create graphics analysis interface");
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
GraphDev::ScrSize GraphDev::scr_size = {640, 480};
ID3D11Device* GraphDev::dev = 0;
IDXGISwapChain* GraphDev::chain = 0;
ID3D11DeviceContext* GraphDev::devctx = 0;
ID3D11Debug* GraphDev::debug = 0;
ID3D11InfoQueue* GraphDev::debug_info = 0;
IDXGraphicsAnalysis* GraphDev::g_analysis = 0;
// -------------------------------------------------------
GraphR::ObGroup::ObGroup()
	: wvp_tposed(new XMFLOAT4X4[1]),
	wvp_buf(0),
	wvp_srv(0),
	bbox_buf(0),
	bbox_srv(0),
	occluder_buf(0),
	occluder_uav(0),
	vert_buf(0),
	vert_uav(0),
	coord_tex_buf(0),
	ind_buf(0),
	test_rect_occl_buf(0),
	test_rect_occl_uav(0),
	test_rect_occl_srv(0),
	test1_buf(0), 
	test1_uav(0), 
	test1_srv(0),
	test2_buf(0),
	test2_srv(0),
	test2_uav(0),
	test_staging_buf(0),
	test_staging_uav(0),
	test_staging_srv(0) {}
GraphR::ObGroup::~ObGroup() {
	delete[] wvp_tposed;
	if(wvp_buf != 0) wvp_buf->Release();
	if(wvp_srv != 0) wvp_srv->Release();
	if(bbox_buf != 0) bbox_buf->Release();
	if(bbox_srv != 0) bbox_srv->Release();
	if(occluder_buf != 0) occluder_buf->Release();
	if(occluder_uav != 0) occluder_uav->Release();
	if(vert_buf != 0) vert_buf->Release();
	if(vert_uav != 0) vert_uav->Release();
	if(coord_tex_buf != 0) coord_tex_buf->Release();
	if(ind_buf != 0) ind_buf->Release();
	if(test_rect_occl_buf != 0) test_rect_occl_buf->Release();
	if(test_rect_occl_uav != 0) test_rect_occl_uav->Release();
	if(test_rect_occl_srv != 0) test_rect_occl_srv->Release();
	if(test1_buf != 0) test1_buf->Release();
	if(test1_uav != 0) test1_uav->Release();
	if(test1_srv != 0) test1_srv->Release();
	if(test2_buf != 0) test2_buf->Release();
	if(test2_uav != 0) test2_uav->Release();
	if(test2_srv != 0) test2_srv->Release();
	if(test_staging_buf != 0) test_staging_buf->Release();
	if(test_staging_uav != 0) test_staging_uav->Release();
	if(test_staging_srv != 0) test_staging_srv->Release();
}
void GraphR::ObGroup::update_wvp(XMFLOAT4X4 const*const _mtx_wvp, uint32_t const _size) {
	if(_mtx_wvp == 0 || _size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(wvp_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else wvp_buf->GetDesc(&_buf_desc);
	
	// resize buffer
	if(_size * sizeof(XMFLOAT4X4) > _buf_desc.ByteWidth) {
		// transposed mtx
		delete[] wvp_tposed;
		wvp_tposed = new XMFLOAT4X4[_size];

		// buf
		_buf_desc.ByteWidth = sizeof(XMFLOAT4X4) * _size;
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(wvp_buf != 0) wvp_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &wvp_buf);
		if(_r != S_OK) logi.pisz("", "failed to create world buf");

		// srv
		if(wvp_srv != 0) wvp_srv->Release();
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size * 4;
		_r = dev->CreateShaderResourceView(wvp_buf, &_srv_desc, &wvp_srv);
		if(_r != S_OK) logi.pisz("", "failed to create world srv");
	}

	// transpose mtx
	for(uint32_t _i = 0; _i < _size; ++_i) {
		XMStoreFloat4x4(&wvp_tposed[_i], XMMatrixTranspose(XMLoadFloat4x4(&_mtx_wvp[_i])));
	}

	devctx->UpdateSubresource(wvp_buf, 0, 0, wvp_tposed, 0, 0);
}
void GraphR::ObGroup::update_bbox(float const*const _bbox, uint32_t const _size) {
	if(_bbox == 0 || _size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(bbox_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else bbox_buf->GetDesc(&_buf_desc);
	
	// resize buffer
	if(_size * sizeof(float) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = sizeof(float) * _size;
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(bbox_buf != 0) bbox_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &bbox_buf);
		if(_r != S_OK) logi.pisz("", "failed to create bbox_buf");

		// srv
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size;
		if(bbox_srv != 0) bbox_srv->Release();
		_r = dev->CreateShaderResourceView(bbox_buf, &_srv_desc, &bbox_srv);
		if(_r != S_OK) logi.pisz("", "failed to create bbox_srv");
	}

	devctx->UpdateSubresource(bbox_buf, 0, 0, _bbox, 0, 0);
}
void GraphR::ObGroup::update_coord_tex(XMFLOAT2 const*const _coord_tex, uint32_t const _size) {
	if(_coord_tex == 0 || _size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(coord_tex_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else coord_tex_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(XMFLOAT2) > _buf_desc.ByteWidth) {
		_buf_desc.ByteWidth = sizeof(XMFLOAT2) * _size;
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(coord_tex_buf != 0) coord_tex_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &coord_tex_buf);
		if(_r != S_OK) logi.pisz("", "failed to create texture coordinate buffer");
	}
	devctx->UpdateSubresource(coord_tex_buf, 0, 0, _coord_tex, 0, 0);
}
void GraphR::ObGroup::update_ind(DWORD const*const _ind, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(ind_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else ind_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(DWORD) > _buf_desc.ByteWidth) {
		_buf_desc.ByteWidth = sizeof(DWORD) * _size;
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(ind_buf != 0) ind_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &ind_buf);
		if(_r != S_OK) logi.pisz("", "failed to create index buffer");
	}
	if(_ind != 0) devctx->UpdateSubresource(ind_buf, 0, 0, _ind, 0, 0);
}
void GraphR::ObGroup::update_is_occluder(bool const*const _is_occluder, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(occluder_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else occluder_buf->GetDesc(&_buf_desc);

	if(_size * sizeof(bool) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = sizeof(bool) * _size;
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(occluder_buf != 0) occluder_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &occluder_buf);
		if(_r != S_OK) logi.pisz("", "failed to create is occluder buf");

		// srv
		if(occluder_srv != 0) occluder_srv->Release();
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R8_UINT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size;
		_r = dev->CreateShaderResourceView(occluder_buf, &_srv_desc, &occluder_srv);
		if(_r != S_OK) logi.pisz("", "failed to create is occluder srv");

		// uav
		if(occluder_uav != 0) occluder_uav->Release();
		D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
		_uav_desc.Format = DXGI_FORMAT_R8_UINT;
		_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		_uav_desc.Buffer.FirstElement = 0;
		_uav_desc.Buffer.NumElements = _size;
		_uav_desc.Buffer.Flags = 0;
		_r = dev->CreateUnorderedAccessView(occluder_buf, &_uav_desc, &occluder_uav);
		if(_r != S_OK) logi.pisz("", "failed to create is occluder uav");
	}
	devctx->UpdateSubresource(occluder_buf, 0, 0, _is_occluder, 0, 0);
}
void GraphR::ObGroup::update_vert(XMFLOAT3 const*const _vert, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(vert_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else vert_buf->GetDesc(&_buf_desc);
	
	if(_size * sizeof(XMFLOAT3) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = sizeof(XMFLOAT3) * _size;
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
	case VS_TFORM_TEX: {
		ID3D11Buffer* _buf[] = {vert_buf, coord_tex_buf, wvp_buf};
		uint32_t _strides[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4X4)};
		uint32_t _offsets[] = {0, 0, 0};
		devctx->IASetVertexBuffers(0, 3, _buf, _strides, _offsets);
		break;
	}
	case TEST_VS_RECT_OCCL: {
		uint32_t _strides[] = {sizeof(XMFLOAT3)};
		uint32_t _offsets[] = {0};
		devctx->IASetVertexBuffers(0, 1, &vert_buf, _strides, _offsets);
	}
	}
}
void GraphR::ObGroup::test_update_rect_occl(float const*const _data, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(test_rect_occl_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else test_rect_occl_buf->GetDesc(&_buf_desc);

	if(_size * 4 * sizeof(XMFLOAT4) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * 4 * sizeof(XMFLOAT4);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = 0;
		if(test_rect_occl_buf != 0) test_rect_occl_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &test_rect_occl_buf);
		if(_r != S_OK) logi.pisz("", "failed to create test_rect_occl_buf");

		// uav
		if(test_rect_occl_uav != 0) test_rect_occl_uav->Release();
		D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
		_uav_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		_uav_desc.Buffer.FirstElement = 0;
		_uav_desc.Buffer.NumElements = _size * 4;
		_uav_desc.Buffer.Flags = 0;
		_r = dev->CreateUnorderedAccessView(test_rect_occl_buf, &_uav_desc, &test_rect_occl_uav);
		if(_r != S_OK) logi.pisz("", "failed to create test_rect_occl_uav");

		// srv
		if(test_rect_occl_srv != 0) test_rect_occl_srv->Release();
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size * 4;
		_r = dev->CreateShaderResourceView(test_rect_occl_buf, &_srv_desc, &test_rect_occl_srv);
		if(_r != S_OK) logi.pisz("", "failed to create test_rect_occl_srv");
	}
	if(_data != 0) devctx->UpdateSubresource(test_rect_occl_buf, 0, 0, _data, 0, 0);
}
void GraphR::ObGroup::test_update(float const*const _data1, uint32_t const _size, float const*const _data2) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(test1_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else test1_buf->GetDesc(&_buf_desc);

	if(_size * sizeof(float) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * sizeof(float);
		_buf_desc.Usage = D3D11_USAGE_DEFAULT;
		_buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = 0;
		_buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		_buf_desc.StructureByteStride = sizeof(float);
		if(test1_buf != 0) test1_buf->Release();
		if(test2_buf != 0) test2_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &test1_buf);
		if(_r != S_OK) logi.pisz("", "failed to create test_buf");
		_r = dev->CreateBuffer(&_buf_desc, 0, &test2_buf);
		if(_r != S_OK) logi.pisz("", "failed to create test_buf");

		// uav
		D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
		_uav_desc.Format = DXGI_FORMAT_UNKNOWN;
		_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		_uav_desc.Buffer.FirstElement = 1;
		_uav_desc.Buffer.NumElements = _size - 1;
		_uav_desc.Buffer.Flags = 0;
		if(test1_uav != 0) test1_uav->Release();
		if(test2_uav != 0) test2_uav->Release();
		_r = dev->CreateUnorderedAccessView(test1_buf, &_uav_desc, &test1_uav);
		if(_r != S_OK) logi.pisz("", "failed to create test_uav");
		_r = dev->CreateUnorderedAccessView(test2_buf, &_uav_desc, &test2_uav);
		if(_r != S_OK) logi.pisz("", "failed to create test_uav");

		// srv
		D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		_srv_desc.Format = DXGI_FORMAT_UNKNOWN;
		_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		_srv_desc.Buffer.FirstElement = 0;
		_srv_desc.Buffer.NumElements = _size;
		if(test1_srv != 0) test1_srv->Release();
		if(test2_srv != 0) test2_srv->Release();
		_r = dev->CreateShaderResourceView(test1_buf, &_srv_desc, &test1_srv);
		if(_r != S_OK) logi.pisz("", "failed to create test_srv");
		_r = dev->CreateShaderResourceView(test2_buf, &_srv_desc, &test2_srv);
		if(_r != S_OK) logi.pisz("", "failed to create test_srv");
	}
	if(_data1 != 0) devctx->UpdateSubresource(test1_buf, 0, 0, _data1, 0, 0);
	if(_data2 != 0) devctx->UpdateSubresource(test2_buf, 0, 0, _data2, 0, 0);
}
void GraphR::ObGroup::test_update_staging(float const*const _data, uint32_t const _size) {
	if(_size == 0) return;

	D3D11_BUFFER_DESC _buf_desc;
	if(test_staging_buf == 0) memset(&_buf_desc, 0, sizeof(_buf_desc));
	else test_staging_buf->GetDesc(&_buf_desc);

	if(_size * sizeof(float) > _buf_desc.ByteWidth) {
		// buf
		_buf_desc.ByteWidth = _size * sizeof(float);
		_buf_desc.Usage = D3D11_USAGE_STAGING;
		//_buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		_buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		//_buf_desc.CPUAccessFlags = 0;
		//_buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		//_buf_desc.StructureByteStride = sizeof(float);
		if(test_staging_buf != 0) test_staging_buf->Release();
		HRESULT _r = dev->CreateBuffer(&_buf_desc, 0, &test_staging_buf);
		if(_r != S_OK) logi.pisz("", "failed to create test_buf");

		//// uav
		//D3D11_UNORDERED_ACCESS_VIEW_DESC _uav_desc;
		//_uav_desc.Format = DXGI_FORMAT_R32_FLOAT;
		//_uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		//_uav_desc.Buffer.FirstElement = 0;
		//_uav_desc.Buffer.NumElements = _size;
		//_uav_desc.Buffer.Flags = 0;
		//if(test_staging_uav != 0) test_staging_uav->Release();
		//_r = dev->CreateUnorderedAccessView(test_staging_buf, &_uav_desc, &test_staging_uav);
		//if(_r != S_OK) logi.pisz("", "failed to create test_uav");

		//// srv
		//D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
		//_srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
		//_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		//_srv_desc.Buffer.FirstElement = 0;
		//_srv_desc.Buffer.NumElements = _size;
		//if(test_staging_srv != 0) test_staging_srv->Release();
		//_r = dev->CreateShaderResourceView(test_staging_buf, &_srv_desc, &test_staging_srv);
		//if(_r != S_OK) logi.pisz("", "failed to create test_srv");
	}
	if(_data != 0) devctx->UpdateSubresource(test_staging_buf, 0, 0, _data, 0, 0);
}
// -------------------------------------------------------
GraphR::GraphR()
	: scr_size_buf(0),
	ds_tex2(0),
	ds_dsv(0),
	ds_srv(0),
	back_buf_rtv(0),
	ss(0) {}
GraphR::~GraphR() {
	for(uint32_t _i = 0; _i < vs.get_size(); ++_i) {
		vs[_i]->Release();
	}
	for(uint32_t _i = 0; _i < ps.get_size(); ++_i) {
		ps[_i]->Release();
	}
	scr_size_buf->Release();
	ds_tex2->Release();
	ds_dsv->Release();
	ds_srv->Release();
	back_buf_rtv->Release();
	ss->Release();
	for(uint32_t _i = 0; _i < in_lay.get_size(); ++_i) {
		in_lay[_i]->Release();
	}
}
void GraphR::bind_prim_topol() const {
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void GraphR::bind_ss() const {
	devctx->PSSetSamplers(0, 1, &ss);
}
void GraphR::bind_viewport() const {
	devctx->RSSetViewports(1, &viewport);
}
void GraphR::create_back_buf() {
	ID3D11Texture2D* _back_buf_tex2;

	// RTV
	HRESULT _r = chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&_back_buf_tex2);
	if(_r != S_OK) logi.pisz("", "nie pobrano opisu bufora tylnego");
	_r = dev->CreateRenderTargetView(_back_buf_tex2, 0, &back_buf_rtv);

	_back_buf_tex2->Release();
}
void GraphR::create_buf_struct() {
	//struct BufferStruct { UINT color[4]; };
	//D3D11_BUFFER_DESC sbDesc;
	//sbDesc.ByteWidth = sizeof(BufferStruct) * THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
	//sbDesc.Usage = D3D11_USAGE_DEFAULT;
	//sbDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	//sbDesc.CPUAccessFlags = 0;
	//sbDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	//sbDesc.StructureByteStride = sizeof(BufferStruct);
	////InitData.pSysMem = NULL;
	//HRESULT hr = dev->CreateBuffer(&sbDesc, NULL, &buf_struct);

	//D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
	//sbUAVDesc.Buffer.FirstElement = 0;
	//sbUAVDesc.Buffer.Flags = 0;
	//sbUAVDesc.Buffer.NumElements = THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
	//sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	//sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	//hr = dev->CreateUnorderedAccessView(buf_struct, &sbUAVDesc, &g_pStructuredBufferUAV);

	//D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc;
	//sbSRVDesc.Buffer.ElementOffset = 0;
	//sbSRVDesc.Buffer.ElementWidth = sizeof(BufferStruct);
	//sbSRVDesc.Buffer.FirstElement = 0;
	//sbSRVDesc.Buffer.NumElements = THREAD_GRID_SIZE_X * THREAD_GRID_SIZE_Y;
	//sbSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	//sbSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//hr = dev->CreateShaderResourceView(buf_struct, &sbSRVDesc, &g_pStructuredBufferSRV);

	//UINT initCounts = 0;
	//devctx->CSSetUnorderedAccessViews(0, 1, &g_pStructuredBufferUAV, &initCounts);
	//devctx->CSSetShader(shad_compute[CS_OCCLUDE], NULL, 0);
	//devctx->Dispatch(THREAD_GROUPS_X, THREAD_GROUPS_Y, 1);
	//ID3D11UnorderedAccessView* pNullUAV = NULL;
	//devctx->CSSetUnorderedAccessViews(0, 1, &pNullUAV, &initCounts);
}
void GraphR::create_scr_size() {
	D3D11_BUFFER_DESC _scr_size_desc;
	memset(&_scr_size_desc, 0, sizeof(_scr_size_desc));
	_scr_size_desc.ByteWidth = 16 * (sizeof(scr_size) / 16) + 16; // multiple of 16
	_scr_size_desc.Usage = D3D11_USAGE_DEFAULT;
	_scr_size_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA _scr_size_data;
	memset(&_scr_size_data, 0, sizeof(_scr_size_data));
	_scr_size_data.pSysMem = &scr_size;
	HRESULT _r = dev->CreateBuffer(&_scr_size_desc, &_scr_size_data, &scr_size_buf);
	if(_r != S_OK) logi.pisz("", "failed to create scr_size_buf");
}
void GraphR::create_ds() {
	// tex
	D3D11_TEXTURE2D_DESC _tex2_desc;
	memset(&_tex2_desc, 0, sizeof(_tex2_desc));
	_tex2_desc.Width = scr_size.width;
	_tex2_desc.Height = scr_size.height;
	_tex2_desc.MipLevels = 0;
	_tex2_desc.ArraySize = 1;
	_tex2_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	_tex2_desc.SampleDesc.Count = 1;
	_tex2_desc.SampleDesc.Quality = 0;
	_tex2_desc.Usage = D3D11_USAGE_DEFAULT;
	_tex2_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	_tex2_desc.CPUAccessFlags = 0;
	_tex2_desc.MiscFlags = 0;
	HRESULT _r = dev->CreateTexture2D(&_tex2_desc, 0, &ds_tex2);
	if(_r != S_OK) logi.pisz("", "failed to create depth stencil tex2");

	// dsv
	D3D11_DEPTH_STENCIL_VIEW_DESC _ds_desc;
	memset(&_ds_desc, 0, sizeof(_ds_desc));
	//_ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_ds_desc.Format = DXGI_FORMAT_D32_FLOAT;
	_ds_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	_ds_desc.Texture2D.MipSlice = 0;
	_r = dev->CreateDepthStencilView(ds_tex2, &_ds_desc, &ds_dsv);
	if(_r != S_OK) logi.pisz("", "failed to create depth stencil dsv");

	// srv
	D3D11_SHADER_RESOURCE_VIEW_DESC _srv_desc;
	_srv_desc.Format = DXGI_FORMAT_R32_FLOAT;
	_srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	_srv_desc.Texture2D.MostDetailedMip = 0;
	_srv_desc.Texture2D.MipLevels = -1;
	_r = dev->CreateShaderResourceView(ds_tex2, &_srv_desc, &ds_srv);
	if(_r != S_OK) logi.pisz("", "failed to create depth stencil srv");
}
void GraphR::create_in_lay() {
	HRESULT _r;
	vector<byte> _shad_bytes;
	{
		// IN_F3
		D3D11_INPUT_ELEMENT_DESC _desc[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		in_lay.push_back(0);
		_shad_bytes.clear();
		_shad_bytes = read_bytes("shader\\vs_test.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F3]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}
	{
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
	}
	{
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
		_shad_bytes = read_bytes("shader\\vs_tform.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F3F44]);
		if(_r != S_OK) logi.pisz("", "input layout not created");
	}
	{
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
		_shad_bytes = read_bytes("shader\\vs_tform_tex.cso");
		if(_shad_bytes.size() == 0) logi.pisz("", "shader not read");
		_r = dev->CreateInputLayout(_desc, ARRAYSIZE(_desc), &_shad_bytes[0], _shad_bytes.size(), &in_lay[IN_F3F2F44]);
		if(_r != S_OK) logi.pisz("", "input structure not created");
	}
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
	if(w != S_OK) logi.pisz("", "nie stworzono stan prob");
}
void GraphR::create_cs() {
	// CS_OCCL_CULL
	cs.push_back(0);
	vector<byte> _shad_bytes = read_bytes("shader\\cs_occl_cull.cso");
	HRESULT _r = dev->CreateComputeShader(&_shad_bytes[0], _shad_bytes.size(), 0, &cs[CS_OCCL_CULL]);
	if(_r != S_OK) logi.pisz("", "compute shader not created");

	// TEST_CS_RECT_OCCL
	cs.push_back(0);
	_shad_bytes = read_bytes("shader\\test_cs_rect_occl.cso");
	_r = dev->CreateComputeShader(&_shad_bytes[0], _shad_bytes.size(), 0, &cs[TEST_CS_RECT_OCCL]);
	if(_r != S_OK) logi.pisz("", "compute shader not created");
}
void GraphR::create_ps() {
	// PS_SAMPLE_TEX
	vector<byte> _shad_bytes = read_bytes("shader\\ps_sample_tex.cso");
	ps.push_back(0);
	HRESULT _r = dev->CreatePixelShader(&_shad_bytes[0], _shad_bytes.size(), 0, &ps[PS_SAMPLE_TEX]);
	if(_r != S_OK) logi.pisz("", "pixel shader not created");

	// PS_WRITE_DEPTH
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\ps_write_depth.cso");
	ps.push_back(0);
	_r = dev->CreatePixelShader(&_shad_bytes[0], _shad_bytes.size(), 0, &ps[PS_WRITE_DEPTH]);
	if(_r != S_OK) logi.pisz("", "pixel shader not created");

	// TEST_PS_RECT_OCCL
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\test_ps_rect_occl.cso");
	ps.push_back(0);
	_r = dev->CreatePixelShader(&_shad_bytes[0], _shad_bytes.size(), 0, &ps[TEST_PS_RECT_OCCL]);
	if(_r != S_OK) logi.pisz("", "pixel shader not created");
}
void GraphR::create_vs() {
	// VS_PASS_ON
	vs.push_back(0);
	vector<byte> _shad_bytes = read_bytes("shader\\vs_pass_on.cso");
	HRESULT _r = dev->CreateVertexShader(
		&_shad_bytes[0],
		_shad_bytes.size(),
		0, &vs[VS_PASS_ON]
	);
	if(_r != S_OK) logi.pisz("", "vertex shader not created");

	// VS_TFORM
	vs.push_back(0);
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\vs_tform.cso");
	_r = dev->CreateVertexShader(&_shad_bytes[0], _shad_bytes.size(), 0, &vs[VS_TFORM]);
	if(_r != S_OK) logi.pisz("", "vertex shader not created");

	// VS_TFORM_TEX
	vs.push_back(0);
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\vs_tform_tex.cso");
	_r = dev->CreateVertexShader(&_shad_bytes[0], _shad_bytes.size(), 0, &vs[VS_TFORM_TEX]);
	if(_r != S_OK) logi.pisz("", "vertex shader not created");

	// TEST_VS_RECT_OCCL
	vs.push_back(0);
	_shad_bytes.clear();
	_shad_bytes = read_bytes("shader\\test_vs_rect_occl.cso");
	_r = dev->CreateVertexShader(&_shad_bytes[0], _shad_bytes.size(), 0, &vs[TEST_VS_RECT_OCCL]);
	if(_r != S_OK) logi.pisz("", "vertex shader not created");
}
void GraphR::create_viewport() {
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = scr_size.width;
	viewport.Height = scr_size.height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}
GraphR GraphRes::res;
// -------------------------------------------------------
Meshes::~Meshes() {
	no.destroy();
	ref.destroy();
	vert.destroy();
	coord_tex.destroy();
	ind.destroy();
}
void Meshes::create(uint32_t const _hnd) {
	if(no[_hnd] != no.empty) {
		++ref[_hnd];
		return;
	} else {
		no[_hnd] = vert.get_col_size();
		ref[_hnd] = 1;
	}
	if(_hnd == MESH_TRI) {
		XMFLOAT3 _vert[] = {
			XMFLOAT3(0.0f, -0.2f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT3(0.2f, -0.0f, 0.0f),
		};
		XMFLOAT2 _teks[] = {
			XMFLOAT2(0.0f, 1.0f),
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT2(1.0f, 0.0f),
		};
		DWORD _ind[] = {0, 1, 2};
		vert.push_back(_vert, 3);
		coord_tex.push_back(_teks, 3);
		ind.push_back(_ind, 3);
	} else if(_hnd == MESH_RECT) {
		XMFLOAT3 _vert[] = {
			XMFLOAT3(+0.5f, -0.5f, +0.0f),
			XMFLOAT3(-0.5f, -0.5f, +0.0f),
			XMFLOAT3(-0.5f, +0.5f, +0.0f),
			XMFLOAT3(+0.5f, +0.5f, +0.0f),
		};
		XMFLOAT2 _teks[] = {
			XMFLOAT2(+1.0f, +0.0f),
			XMFLOAT2(+1.0f, +1.0f),
			XMFLOAT2(+0.0f, +1.0f),
			XMFLOAT2(+0.0f, +0.0f),
		};
		DWORD _ind[] = {
			0, 1, 2,
			0, 2, 3
		};
		vert.push_back(_vert, 4);
		coord_tex.push_back(_teks, 4);
		ind.push_back(_ind, 6);
	} else if(_hnd == MESH_DIAMENT) {
		XMFLOAT3 _vert[] = {
			XMFLOAT3(-0.25f, +0.0f, -0.25f),
			XMFLOAT3(+0.25f, +0.0f, -0.25f),
			XMFLOAT3(+0.25f, +0.0f, +0.25f),
			XMFLOAT3(-0.25f, +0.0f, +0.25f),
			XMFLOAT3(+0.0f, +0.5f, +0.0f),
			XMFLOAT3(+0.0f, -0.5f, +0.0f),
		};
		XMFLOAT2 _teks[] = {
			XMFLOAT2(+0.0f, +0.5f),
			XMFLOAT2(+0.5f, +0.5f),
			XMFLOAT2(+1.0f, +0.5f),
			XMFLOAT2(+1.0f, +1.0f),
			XMFLOAT2(+0.5f, +0.0f),
			XMFLOAT2(+0.5f, +1.0f),
		};
		DWORD _ind[] = {
			0, 4, 1,
			1, 5, 0,
			1, 4, 2,
			2, 5, 1,
			2, 4, 3,
			3, 5, 2,
			3, 4, 0,
			0, 5, 3,
		};
		vert.push_back(_vert, 6);
		coord_tex.push_back(_teks, 6);
		ind.push_back(_ind, 24);
	}
}
void Meshes::defrag(uint32_t const _size) {
	uint32_t* _mapa1 = 0,* _mapa2 = 0;
	vert.defrag_comp(_mapa1, _mapa2, _size);
	vert.defrag_exe(_mapa1, _mapa2);
	coord_tex.defrag_exe(_mapa1, _mapa2);
	ind.defrag_exe(_mapa1, _mapa2);
	no.update(_mapa1);
	ref.update(_mapa1);
}
void Meshes::destroy(uint32_t const _hnd) {
	if(no[_hnd] == no.empty) {
		return;
	} else if(ref[_hnd] > 1) {
		--ref[_hnd];
		return;
	}
	vert.erase(no[_hnd]);
	coord_tex.erase(no[_hnd]);
	ind.erase(no[_hnd]);
	no[_hnd] = no.empty;
	ref[_hnd] = 0;
}
void Meshes::sort_comp(uint32_t*& _map) {
	free(_map);
	_map = (uint32_t*)malloc(vert.get_col_size() * 4);
	uint32_t _ind = 0;
	for(uint32_t _i = 0; _i < no.get_cap(); ++_i) {
		if(no[_i] == no.empty) continue;
		_map[no[_i]] = _ind++;
	}
}
void Meshes::sort_exe(uint32_t const*const _map) {
	no.update(_map);
	vert.sort_exe(_map);
	coord_tex.sort_exe(_map);
	ind.sort_exe(_map);
}
// -------------------------------------------------------
Textures::~Textures() {
	no.destroy();
	ref.destroy();
	for(uint32_t _i = 0; _i < view.get_size(); ++_i) {
		view[_i]->Release();
	}
	view.destroy();
}
void Textures::create(uint32_t const _hnd) {
	if(no[_hnd] != no.empty) {
		++ref[_hnd];
		return;
	} else {
		no[_hnd] = view.get_size();
		ref[_hnd] = 1;
	}
	view.push_back(0);
	D3DX11CreateShaderResourceViewFromFile(dev, get_path(_hnd), 0, 0, &view[view.get_size()-1], 0);
}
void Textures::defrag(uint32_t const _size) {
	uint32_t* _map = 0;
	view.defrag_comp(_map, _size);
	view.defrag_exe(_map);
	no.update(_map);
	ref.update(_map);
}
void Textures::destroy(uint32_t const _hnd) {
	if(no[_hnd] == no.empty) {
		return;
	} else if(ref[_hnd] > 1) {
		--ref[_hnd];
		return;
	}
	view[no[_hnd]]->Release();
	view.erase(no[_hnd]);
	no[_hnd] = no.empty;
	ref[_hnd] = 0;
}
char const*const Textures::get_path(uint32_t const _hnd) const {
	switch(_hnd) {
	case TEX_TRI:
		return "texture/cursor.jpg";
	case TEX_RECT:
		return "texture/rectangle.jpg";
	case TEX_DIAMENT:
		return "texture/diament.jpg";
	}
}
void Textures::sort_comp(uint32_t*& _map) {
	free(_map);
	_map = (uint32_t*)malloc(view.get_size() * 4);
	uint32_t _ind = 0;
	for(uint32_t _i = 0; _i < no.get_cap(); ++_i) {
		if(no[_i] == no.empty) continue;
		_map[no[_i]] = _ind++;
	}
}
// -------------------------------------------------------
DataEngine::Data::~Data() {
	no.destroy();
	loc.destroy();
	v.destroy();
	mtx_world.destroy();
	mtx_wvp.destroy();
	bbox_local.destroy();
	bbox_scr.destroy();
	occluder.destroy();
	mesh_hnd.destroy();
	tex_hnd.destroy();
}
DataEngine::Data DataEngine::data_e;
// -------------------------------------------------------
Cam::Cam()
	: q(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)),
	loc(XMFLOAT3(0.0f, 0.0f, -0.5f)),
	fov(90),
	near_z(0.5f),
	far_z(100.0f) {}
Cam Camera::cam;
// -------------------------------------------------------





