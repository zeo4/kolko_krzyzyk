#pragma once
#include <resources.h>
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
GraphR::GraphR()
	: dev(0),
	chain(0),
	devctx(0),
	buf_vert(0),
	buf_coord_tex(0),
	buf_world(0),
	buf_ind(0),
	buf_szad_err(0),
	depth_stencil_tex2(0),
	depth_stencil_dsv(0),
	depth_stencil_srv(0),
	buf_back_rtv(0),
	sampl_state(0),
	buf_per_frame(0) {}
GraphR::~GraphR() {
	if(dev == 0) return;
	dev->Release();
	chain->Release();
	devctx->Release();
	buf_vert->Release();
	buf_coord_tex->Release();
	buf_world->Release();
	buf_ind->Release();
	for(uint32_t _i = 0; _i < szad_vert.get_size(); ++_i) {
		szad_vert[_i]->Release();
	}
	for(uint32_t _i = 0; _i < szad_pix.get_size(); ++_i) {
		szad_pix[_i]->Release();
	}
	if(buf_szad_err != 0) {
		buf_szad_err->Release();
	}
	depth_stencil_tex2->Release();
	depth_stencil_dsv->Release();
	depth_stencil_srv->Release();
	buf_back_rtv->Release();
	sampl_state->Release();
	for(uint32_t _i = 0; _i < in_lay.get_size(); ++_i) {
		in_lay[_i]->Release();
	}
	buf_per_frame->Release();
}
void GraphR::update_per_frame() {
	devctx->UpdateSubresource(buf_per_frame, 0, 0, &per_frame, 0, 0);
}
void GraphR::update_ind(DWORD const*const _ind, uint32_t const _size) {
	D3D11_BUFFER_DESC _opis;
	buf_ind->GetDesc(&_opis);
	if(_size * sizeof(DWORD) > _opis.ByteWidth) {
		create_buf<DWORD>(dev, buf_ind, _size, D3D11_BIND_INDEX_BUFFER);
	}
	devctx->UpdateSubresource(buf_ind, 0, 0, _ind, 0, 0);
}
void GraphR::update_world(XMFLOAT4X4 const*const _mtx_world, uint32_t const _size) {
	D3D11_BUFFER_DESC _opis;
	buf_world->GetDesc(&_opis);
	if(_size * sizeof(XMFLOAT4X4) > _opis.ByteWidth) {
		create_buf<XMFLOAT4X4>(dev, buf_world, _size, D3D11_BIND_VERTEX_BUFFER);
	}
	devctx->UpdateSubresource(buf_world, 0, 0, _mtx_world, 0, 0);
}
void GraphR::update_coord_tex(XMFLOAT2 const*const _coord_tex, uint32_t const _size) {
	D3D11_BUFFER_DESC _opis;
	buf_coord_tex->GetDesc(&_opis);
	if(_size * sizeof(XMFLOAT2) > _opis.ByteWidth) {
		create_buf<XMFLOAT2>(dev, buf_coord_tex, _size, D3D11_BIND_VERTEX_BUFFER);
	}
	devctx->UpdateSubresource(buf_coord_tex, 0, 0, _coord_tex, 0, 0);
}
void GraphR::update_vert(XMFLOAT3 const*const _vert, uint32_t const _size) {
	D3D11_BUFFER_DESC _opis;
	buf_vert->GetDesc(&_opis);
	if(_size * sizeof(XMFLOAT3) > _opis.ByteWidth) {
		create_buf<XMFLOAT3>(dev, buf_vert, _size, D3D11_BIND_VERTEX_BUFFER);
	}
	devctx->UpdateSubresource(buf_vert, 0, 0, _vert, 0, 0);
}
void GraphR::init_ind() {
	create_buf<DWORD>(dev, buf_ind, 1, D3D11_BIND_INDEX_BUFFER);
}
void GraphR::init_world() {
	create_buf<XMFLOAT4X4>(dev, buf_world, 1, D3D11_BIND_VERTEX_BUFFER);
}
void GraphR::init_coord_tex() {
	create_buf<XMFLOAT2>(dev, buf_coord_tex, 1, D3D11_BIND_VERTEX_BUFFER);
}
void GraphR::init_vert() {
	create_buf<XMFLOAT3>(dev, buf_vert, 1, D3D11_BIND_VERTEX_BUFFER);
}
void GraphR::create_buf_back_rtv() {
	ID3D11Texture2D* _buf_tylny_teks2;
	HRESULT _w = chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&_buf_tylny_teks2);
	if(_w != S_OK) logi.pisz("", "nie pobrano opisu bufora tylnego");
	_w = dev->CreateRenderTargetView(_buf_tylny_teks2, 0, &buf_back_rtv);
	_buf_tylny_teks2->Release();
}
void GraphR::create_depth_stencil() {
	D3D11_TEXTURE2D_DESC _opis_teks2;
	memset(&_opis_teks2, 0, sizeof(_opis_teks2));
	_opis_teks2.Width = szerRend;
	_opis_teks2.Height = wysRend;
	_opis_teks2.MipLevels = 1;
	_opis_teks2.ArraySize = 1;
	//_opis_teks2.Format = DXGI_FORMAT_R24G8_TYPELESS;
	_opis_teks2.Format = DXGI_FORMAT_R32_TYPELESS;
	_opis_teks2.SampleDesc.Count = 1;
	_opis_teks2.SampleDesc.Quality = 0;
	_opis_teks2.Usage = D3D11_USAGE_DEFAULT;
	_opis_teks2.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	_opis_teks2.CPUAccessFlags = 0;
	_opis_teks2.MiscFlags = 0;
	HRESULT _w = dev->CreateTexture2D(&_opis_teks2, 0, &depth_stencil_tex2);
	if(_w != S_OK) logi.pisz("", "nie stworzono tekstury glebi szablonu");

	D3D11_DEPTH_STENCIL_VIEW_DESC _opis_wgs;
	memset(&_opis_wgs, 0, sizeof(_opis_wgs));
	//_opis_wgs.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	_opis_wgs.Format = DXGI_FORMAT_D32_FLOAT;
	_opis_wgs.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	_opis_wgs.Texture2D.MipSlice = 0;
	_w = dev->CreateDepthStencilView(depth_stencil_tex2, &_opis_wgs, &depth_stencil_dsv);
	if(_w != S_OK) logi.pisz("", "nie stworzono widoku glebi szablonu");

	D3D11_SHADER_RESOURCE_VIEW_DESC _opis_wzs;
	memset(&_opis_wzs, 0, sizeof(_opis_wzs));
	//_opis_wzs.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	_opis_wzs.Format = DXGI_FORMAT_R32_FLOAT;
	_opis_wzs.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	_opis_wzs.Texture2D.MostDetailedMip = 0;
	_opis_wzs.Texture2D.MipLevels = 1;
	_w = dev->CreateShaderResourceView(depth_stencil_tex2, &_opis_wzs, &depth_stencil_srv);
	if(_w != S_OK) logi.pisz("", "nie stworzono widoku zasobu szadera dla glebi szablonu");
}
void GraphR::create_per_frame() {
	create_buf<PerFrame>(dev, buf_per_frame, 1, D3D11_BIND_CONSTANT_BUFFER);
}
void GraphR::create_dev_ctx_chain() {
	DXGI_MODE_DESC buf_tyl_opis;
	ZeroMemory(&buf_tyl_opis, sizeof(buf_tyl_opis));
	buf_tyl_opis.Width = szerRend;
	buf_tyl_opis.Height = wysRend;
	buf_tyl_opis.RefreshRate.Numerator = 60;
	buf_tyl_opis.RefreshRate.Denominator = 1;
	buf_tyl_opis.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	buf_tyl_opis.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buf_tyl_opis.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	DXGI_SWAP_CHAIN_DESC lanc_opis;
	ZeroMemory(&lanc_opis, sizeof(lanc_opis));
	lanc_opis.BufferDesc = buf_tyl_opis;
	lanc_opis.SampleDesc.Count = 1;
	lanc_opis.SampleDesc.Quality = 0;
	lanc_opis.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	lanc_opis.BufferCount = 1;
	lanc_opis.OutputWindow = uch_okno;
	lanc_opis.Windowed = true;
	lanc_opis.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	HRESULT w = D3D11CreateDeviceAndSwapChain(
		0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 0, 0,
		D3D11_SDK_VERSION, &lanc_opis, &chain, &dev, 0, &devctx
	);
	if(w != S_OK) logi.pisz("", "nie stworzono karty devctx chain");
}
void GraphR::create_viewport() {
	ZeroMemory(&viewport, sizeof(viewport));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = szerRend;
	viewport.Height = wysRend;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}
void GraphR::create_sampl_state() {
	D3D11_SAMPLER_DESC stan_prob_opis;
	ZeroMemory(&stan_prob_opis, sizeof(stan_prob_opis));
	stan_prob_opis.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	stan_prob_opis.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.ComparisonFunc = D3D11_COMPARISON_NEVER;
	stan_prob_opis.MinLOD = 0;
	stan_prob_opis.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT w = dev->CreateSamplerState(&stan_prob_opis, &sampl_state);
	if(w != S_OK) logi.pisz("", "nie stworzono stan prob");
}
void GraphR::create_in_lay() {
	HRESULT _w;
	vector<byte> _szad_bytes;
	{
		D3D11_INPUT_ELEMENT_DESC _opis[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		// IN_F4F2
		in_lay.push_back(0);
		_szad_bytes.clear();
		_szad_bytes = read_bytes("szader\\vs_pass_on.cso");
		_w = dev->CreateInputLayout(_opis, ARRAYSIZE(_opis), &_szad_bytes[0], _szad_bytes.size(), &in_lay[IN_F4F2]);
		if(_w != S_OK) logi.pisz("", "nie stworzono strukt we");
	}
	{
		D3D11_INPUT_ELEMENT_DESC _opis[] = {
			{"VERT_COORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEX_COORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{"WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		};

		// IN_F4F2F44
		in_lay.push_back(0);
		_szad_bytes.clear();
		_szad_bytes = read_bytes("szader\\vs_wvp_trans.cso");
		_w = dev->CreateInputLayout(_opis, ARRAYSIZE(_opis), &_szad_bytes[0], _szad_bytes.size(), &in_lay[IN_F4F2F44]);
		if(_w != S_OK) logi.pisz("", "nie stworzono strukt we");
	}
}
void GraphR::create_szad_pix() {
	// PS_TEX_SAMPLE
	vector<byte> _szad_bytes = read_bytes("szader\\ps_tex_sample.cso");
	szad_pix.push_back(0);
	HRESULT _r = dev->CreatePixelShader(
		&_szad_bytes[0],
		_szad_bytes.size(),
		0, &szad_pix[PS_TEX_SAMPLE]
	);
	if(_r != S_OK) logi.pisz("", "nie stworzono szad piks");
}
void GraphR::create_szad_vert() {
	// VS_PASS_ON
	szad_vert.push_back(0);
	vector<byte> _szad_bytes = read_bytes("szader\\vs_pass_on.cso");
	HRESULT _w = dev->CreateVertexShader(
		&_szad_bytes[0],
		_szad_bytes.size(),
		0, &szad_vert[VS_PASS_ON]
	);
	if(_w != S_OK) logi.pisz("", "nie stworzono szad vert");

	// VS_WVP_TRANS
	szad_vert.push_back(0);
	_szad_bytes.clear();
	_szad_bytes = read_bytes("szader\\vs_wvp_trans.cso");
	_w = dev->CreateVertexShader(
		&_szad_bytes[0],
		_szad_bytes.size(),
		0, &szad_vert[VS_WVP_TRANS]
	);
	if(_w != S_OK) logi.pisz("", "nie stworzono szad vert");
}
void GraphR::bind_per_frame() const {
	devctx->VSSetConstantBuffers(0, 1, &buf_per_frame);
}
void GraphR::bind_viewport() const {
	devctx->RSSetViewports(1, &viewport);
}
void GraphR::bind_sampl_state() const {
	devctx->PSSetSamplers(0, 1, &sampl_state);
}
void GraphR::bind_prim_topol() const {
	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void GraphR::bind_vert(uint32_t const _nr_szad) const {
	switch(_nr_szad) {
	case VS_PASS_ON: {
		ID3D11Buffer* _buf[] = {buf_vert, buf_coord_tex};
		uint32_t _kroki[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT2)};
		uint32_t _przes[] = {0, 0};
		devctx->IASetVertexBuffers(0, 2, _buf, _kroki, _przes);
		break;
	}
	case VS_WVP_TRANS: {
		ID3D11Buffer* _buf[] = {buf_vert, buf_coord_tex, buf_world};
		uint32_t _kroki[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4X4)};
		uint32_t _przes[] = {0, 0, 0};
		devctx->IASetVertexBuffers(0, 3, _buf, _kroki, _przes);
		break;
	}
	}
}
GraphR::PerFrame::PerFrame() {
	for(uint32_t _i = 0; _i < 1024; ++_i) {
		mtx_wvp[_i] = XMMatrixIdentity();
	}
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
void Textures::create(uint32_t const _hnd) {
	if(no[_hnd] != no.empty) {
		++ref[_hnd];
		return;
	} else {
		no[_hnd] = view.get_size();
		ref[_hnd] = 1;
	}
	view.push_back(0);
	D3DX11CreateShaderResourceViewFromFile(res.dev, get_path(_hnd), 0, 0, &view[view.get_size()-1], 0);
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
char const*const Textures::get_path(uint32_t const _hnd) const {
	switch(_hnd) {
	case TEX_TRI:
		return "tekstury/kursor.jpg";
	case TEX_RECT:
		return "tekstury/kwadrat.jpg";
	case TEX_DIAMENT:
		return "tekstury/diament.jpg";
	}
}
// -------------------------------------------------------
GraphP::~GraphP() {
	no.destroy();
	occluder_mesh_hnd.destroy();
	occluder_tex_hnd.destroy();
	occludee_mesh_hnd.destroy();
	occludee_tex_hnd.destroy();
}
GraphP GraphPar::graph_par;
// -------------------------------------------------------
PhysP::~PhysP() {
	no.destroy();
	pos.destroy();
	v.destroy();
	mtx_world.destroy();
}
PhysP PhysPar::phys_par;
// -------------------------------------------------------
Cam::Cam()
	: quat(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)),
	pos(XMFLOAT3(0.0f, 0.0f, -0.5f)),
	angle(90),
	near_z(0.5f),
	far_z(100.0f) {}
Cam Camera::cam;
// -------------------------------------------------------





