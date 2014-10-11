#pragma once
#include <zasoby.h>
// -------------------------------------------------------
ZGraf::ZGraf()
	: karta(0),
	lanc(0),
	rend(0),
	mod_wierz_buf(0),
	mod_teks_buf(0),
	mod_ind_buf(0),
	szad_wierz(0),
	szad_wierz_buf(0),
	szad_piks(0),
	szad_piks_buf(0),
	szad_blad_buf(0),
	gleb_szab_teks2(0),
	gleb_szab_wid(0),
	cel_rend_wid(0),
	stan_prob(0),
	strukt_we(0),
	co_ob_buf(0) {}
ZGraf::~ZGraf() {
	karta->Release();
	lanc->Release();
	rend->Release();
	mod_wierz_buf->Release();
	mod_teks_buf->Release();
	mod_ind_buf->Release();
	szad_wierz->Release();
	szad_wierz_buf->Release();
	szad_piks->Release();
	szad_piks_buf->Release();
	if(szad_blad_buf != 0) {
		szad_blad_buf->Release();
	}
	gleb_szab_teks2->Release();
	gleb_szab_wid->Release();
	cel_rend_wid->Release();
	stan_prob->Release();
	strukt_we->Release();
	co_ob_buf->Release();
}
void ZGraf::aktual_co_ob() {
	rend->UpdateSubresource(co_ob_buf, 0, 0, &co_ob, 0, 0);
}
void ZGraf::aktual_mod_ind(Wek2<DWORD>const& _mod_ind) {
	D3D11_BUFFER_DESC _opis;
	mod_ind_buf->GetDesc(&_opis);
	if(_mod_ind.wez_il_el()*sizeof(XMFLOAT2) > _opis.ByteWidth) {
		tworz_buf<DWORD>(karta, mod_ind_buf, _mod_ind.wez_il_el(), D3D11_BIND_INDEX_BUFFER);
	}
	rend->UpdateSubresource(mod_ind_buf, 0, 0, _mod_ind[0], 0, 0);
}
void ZGraf::aktual_mod_wierz(Wek2<XMFLOAT3>const& _mod_wierz) {
	D3D11_BUFFER_DESC _opis;
	mod_wierz_buf->GetDesc(&_opis);
	if(_mod_wierz.wez_il_el()*sizeof(XMFLOAT3) > _opis.ByteWidth) {
		tworz_buf<XMFLOAT3>(karta, mod_wierz_buf, _mod_wierz.wez_il_el(), D3D11_BIND_VERTEX_BUFFER);
	}
	rend->UpdateSubresource(mod_wierz_buf, 0, 0, _mod_wierz[0], 0, 0);
}
void ZGraf::aktual_mod_teks(Wek2<XMFLOAT2>const& _mod_teks) {
	D3D11_BUFFER_DESC _opis;
	mod_teks_buf->GetDesc(&_opis);
	if(_mod_teks.wez_il_el()*sizeof(XMFLOAT2) > _opis.ByteWidth) {
		tworz_buf<XMFLOAT2>(karta, mod_teks_buf, _mod_teks.wez_il_el(), D3D11_BIND_VERTEX_BUFFER);
	}
	rend->UpdateSubresource(mod_teks_buf, 0, 0, _mod_teks[0], 0, 0);
}
void ZGraf::czysc_ekran() const {
	FLOAT const kolor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	rend->ClearRenderTargetView(cel_rend_wid, kolor);
	rend->ClearDepthStencilView(gleb_szab_wid, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void ZGraf::inic_mod_ind() {
	tworz_buf<DWORD>(karta, mod_ind_buf, 1, D3D11_BIND_INDEX_BUFFER);
}
void ZGraf::inic_mod_wierz() {
	tworz_buf<XMFLOAT3>(karta, mod_wierz_buf, 1, D3D11_BIND_VERTEX_BUFFER);
}
void ZGraf::inic_mod_teks() {
	tworz_buf<XMFLOAT2>(karta, mod_teks_buf, 1, D3D11_BIND_VERTEX_BUFFER);
}
void ZGraf::rys_model(uint32_t const& _il_ind, uint32_t const& _nr_ind) const {
	rend->DrawIndexed(_il_ind, _nr_ind, 0);
}
void ZGraf::tworz_cel_rend_wid() {
	ID3D11Texture2D* _buf_tyl;
	lanc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&_buf_tyl);
	HRESULT w = karta->CreateRenderTargetView(_buf_tyl, 0, &cel_rend_wid);
	if(w != S_OK) logi.pisz("", "nie stworzono wid cel rend");
	_buf_tyl->Release();
}
void ZGraf::tworz_co_ob() {
	tworz_buf<CoOb>(karta, co_ob_buf, 1, D3D11_BIND_CONSTANT_BUFFER);
}
void ZGraf::tworz_gleb_szab_wid() {
	D3D11_TEXTURE2D_DESC gleb_szab_opis;
	gleb_szab_opis.Width = szerRend;
	gleb_szab_opis.Height = wysRend;
	gleb_szab_opis.MipLevels = 1;
	gleb_szab_opis.ArraySize = 1;
	gleb_szab_opis.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	gleb_szab_opis.SampleDesc.Count = 1;
	gleb_szab_opis.SampleDesc.Quality = 0;
	gleb_szab_opis.Usage = D3D11_USAGE_DEFAULT;
	gleb_szab_opis.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	gleb_szab_opis.CPUAccessFlags = 0;
	gleb_szab_opis.MiscFlags = 0;
	HRESULT w = karta->CreateTexture2D(&gleb_szab_opis, 0, &gleb_szab_teks2);
	if(w != S_OK) logi.pisz("", "nie stworzono teks");
	w = karta->CreateDepthStencilView(gleb_szab_teks2, 0, &gleb_szab_wid);
	if(w != S_OK) logi.pisz("", "nie stworzono wid gleb szab");
}
void ZGraf::tworz_karta_rend_lanc() {
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
		D3D11_SDK_VERSION, &lanc_opis, &lanc, &karta, 0, &rend
	);
	if(w != S_OK) logi.pisz("", "nie stworzono karty rend lanc");
}
void ZGraf::tworz_rzutnia() {
	ZeroMemory(&rzutnia, sizeof(rzutnia));
	rzutnia.TopLeftX = 0;
	rzutnia.TopLeftY = 0;
	rzutnia.Width = szerRend;
	rzutnia.Height = wysRend;
	rzutnia.MinDepth = 0.0f;
	rzutnia.MaxDepth = 1.0f;
}
void ZGraf::tworz_stan_prob() {
	D3D11_SAMPLER_DESC stan_prob_opis;
	ZeroMemory(&stan_prob_opis, sizeof(stan_prob_opis));
	stan_prob_opis.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	stan_prob_opis.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.ComparisonFunc = D3D11_COMPARISON_NEVER;
	stan_prob_opis.MinLOD = 0;
	stan_prob_opis.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT w = karta->CreateSamplerState(&stan_prob_opis, &stan_prob);
	if(w != S_OK) logi.pisz("", "nie stworzono stan prob");
}
void ZGraf::tworz_strukt_we() {
	D3D11_INPUT_ELEMENT_DESC _strukt_we_opis[] = {
		{"POZYCJA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEKSTURA", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	HRESULT w = karta->CreateInputLayout(
		_strukt_we_opis, ARRAYSIZE(_strukt_we_opis), szad_wierz_buf->GetBufferPointer(),
		szad_wierz_buf->GetBufferSize(), &strukt_we
	);
	if(w != S_OK) logi.pisz("", "nie stworzono strukt we");
}
void ZGraf::tworz_szad_piks() {
	HRESULT w = D3DX11CompileFromFile(
		"szader\\efekty.fx", 0, 0, "SP", "ps_4_0", 0, 0, 0,
		&szad_piks_buf, &szad_blad_buf, 0
	);
	if(w != S_OK) logi.pisz("", "nie skompilowano szad piks");
	w = karta->CreatePixelShader(
		szad_piks_buf->GetBufferPointer(),
		szad_piks_buf->GetBufferSize(),
		0, &szad_piks
	);
	if(w != S_OK) logi.pisz("", "nie stworzono szad piks");
}
void ZGraf::tworz_szad_wierz() {
	HRESULT w = D3DX11CompileFromFile(
		"szader\\efekty.fx", 0, 0, "SW", "vs_4_0", 0, 0, 0,
		&szad_wierz_buf, &szad_blad_buf, 0
	);
	if(w != S_OK) logi.pisz("", "nie skompilowano szad wierz");
	w = karta->CreateVertexShader(
		szad_wierz_buf->GetBufferPointer(),
		szad_wierz_buf->GetBufferSize(),
		0, &szad_wierz
	);
	if(w != S_OK) logi.pisz("", "nie stworzono szad wierz");
}
void ZGraf::wiaz_co_ob() const {
	rend->VSSetConstantBuffers(0, 1, &co_ob_buf);
}
void ZGraf::wiaz_cele_rend() const {
	rend->OMSetRenderTargets(1, &cel_rend_wid, gleb_szab_wid);
}
void ZGraf::wiaz_mod_ind()const {
	rend->IASetIndexBuffer(mod_ind_buf, DXGI_FORMAT_R32_UINT, 0);
}
void ZGraf::wiaz_mod_wierz(uint32_t const& _il_wierz) const {
	ID3D11Buffer* _buf_we[] = {mod_wierz_buf, mod_teks_buf};
	uint32_t _kroki[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT2)};
	uint32_t _przes[] = {0, 0};
	rend->IASetVertexBuffers(0, 2, _buf_we, _kroki, _przes);
}
void ZGraf::wiaz_rzutnia() const {
	rend->RSSetViewports(1, &rzutnia);
}
void ZGraf::wiaz_stan_prob() const {
	rend->PSSetSamplers(0, 1, &stan_prob);
}
void ZGraf::wiaz_strukt_we() const {
	rend->IASetInputLayout(strukt_we);
}
void ZGraf::wiaz_szad_piks() const {
	rend->PSSetShader(szad_piks, 0, 0);
}
void ZGraf::wiaz_szad_wierz() const {
	rend->VSSetShader(szad_wierz, 0, 0);
}
void ZGraf::wiaz_teks(ID3D11ShaderResourceView*const& _teks_wid) const {
	rend->PSSetShaderResources(0, 1, &_teks_wid);
}
void ZGraf::wiaz_topol_prym() const {
	rend->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
ZGraf::CoOb::CoOb()
	: mac_swp(XMMatrixIdentity()) {}
ZGraf ZasGraf::zas;
// -------------------------------------------------------
Ob Obiekty::ob;
// -------------------------------------------------------
PFiz ParFiz::par_fiz;
// -------------------------------------------------------
Kam::Kam()
	: poz(XMFLOAT3(0.0f, 0.0f, -0.5f)),
	cel(XMFLOAT3(0.0f, 0.0f, 0.0f)),
	gora(XMFLOAT3(0.0f, 1.0f, 0.0f)),
	kat(90),
	blizsza(0.5f),
	dalsza(100.0f) {}
Kam Kamera::kam;
// -------------------------------------------------------




