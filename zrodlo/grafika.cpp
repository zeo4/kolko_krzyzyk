#pragma once

#include "grafika.h"

Grafika::Grafika()
	: _ob(0),
	_karta(0),
	_lanc_wym(0),
	_rend(0),
	_ob_rend_wid(0),
	_gleb_szab_buf(0),
	_gleb_szab_wid(0),
	_strukt_we(0),
	_stan_prob(0),
	_co_ob_buf(0),
	_wierz_buf(0),
	_wierz_teks_buf(0),
	_wierz_szad(0),
	_wierz_szad_buf(0),
	_ind_buf(0),
	_piks_szad(0),
	_piks_szad_buf(0),
	_blad_szad_buf(0) {
}
Grafika::~Grafika() {
	_karta->Release();
	_lanc_wym->Release();
	_rend->Release();
	_ob_rend_wid->Release();
	_gleb_szab_buf->Release();
	_gleb_szab_wid->Release();
	_strukt_we->Release();
	_stan_prob->Release();
	_co_ob_buf->Release();
	_wierz_buf->Release();
	_wierz_teks_buf->Release();
	_wierz_szad->Release();
	_wierz_szad_buf->Release();
	_ind_buf->Release();
	_piks_szad->Release();
	_piks_szad_buf->Release();
	_blad_szad_buf->Release();
}
void Grafika::aktual_ind_buf() {
	D3D11_BUFFER_DESC buf_opis;
	_ind_buf->GetDesc(&buf_opis);
	if(_ob->ind.wez_il()*sizeof(XMFLOAT2) > buf_opis.ByteWidth) {
		tworz_buf<DWORD>(_karta, _ind_buf, _ob->ind.wez_il(), D3D11_BIND_INDEX_BUFFER);
	}
	_rend->UpdateSubresource(_ind_buf, 0, 0, &_ob->ind[0], 0, 0);
}
void Grafika::aktual_wierz_buf() {
	D3D11_BUFFER_DESC buf_opis;
	_wierz_buf->GetDesc(&buf_opis);
	if(_ob->wierz.wez_il()*sizeof(XMFLOAT3) > buf_opis.ByteWidth) {
		tworz_buf<XMFLOAT3>(_karta, _wierz_buf, _ob->wierz.wez_il(), D3D11_BIND_VERTEX_BUFFER);
	}
	_rend->UpdateSubresource(_wierz_buf, 0, 0, &_ob->wierz[0], 0, 0);
}
void Grafika::aktual_wierz_teks_buf() {
	D3D11_BUFFER_DESC buf_opis;
	_wierz_teks_buf->GetDesc(&buf_opis);
	if(_ob->wierz_teks.wez_il()*sizeof(XMFLOAT2) > buf_opis.ByteWidth) {
		tworz_buf<XMFLOAT2>(_karta, _wierz_teks_buf, _ob->wierz_teks.wez_il(), D3D11_BIND_VERTEX_BUFFER);
	}
	_rend->UpdateSubresource(_wierz_teks_buf, 0, 0, &_ob->wierz_teks[0], 0, 0);
}
void Grafika::czysc() const {
	FLOAT const kolor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	_rend->ClearRenderTargetView(_ob_rend_wid, kolor);
	_rend->ClearDepthStencilView(_gleb_szab_wid, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Grafika::przypisz(Obiekty3w const* obiekty) {
	_ob = obiekty;
}
void Grafika::tworz_gleb_szab_buf_wid() {
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
	_karta->CreateTexture2D(&gleb_szab_opis, 0, &_gleb_szab_buf);
	_karta->CreateDepthStencilView(_gleb_szab_buf, 0, &_gleb_szab_wid);
}
void Grafika::tworz_karta_rend_lanc() {
	DXGI_MODE_DESC buf_tyl_opis;
	ZeroMemory(&buf_tyl_opis, sizeof(buf_tyl_opis));
	buf_tyl_opis.Width = szerRend;
	buf_tyl_opis.Height = wysRend;
	buf_tyl_opis.RefreshRate.Numerator = 60;
	buf_tyl_opis.RefreshRate.Denominator = 1;
	buf_tyl_opis.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	buf_tyl_opis.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buf_tyl_opis.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	DXGI_SWAP_CHAIN_DESC lanc_wym_opis;
	ZeroMemory(&lanc_wym_opis, sizeof(lanc_wym_opis));
	lanc_wym_opis.BufferDesc = buf_tyl_opis;
	lanc_wym_opis.SampleDesc.Count = 1;
	lanc_wym_opis.SampleDesc.Quality = 0;
	lanc_wym_opis.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	lanc_wym_opis.BufferCount = 1;
	lanc_wym_opis.OutputWindow = uchOknoGl;
	lanc_wym_opis.Windowed = true;
	lanc_wym_opis.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	D3D11CreateDeviceAndSwapChain(
		0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 0, 0,
		D3D11_SDK_VERSION, &lanc_wym_opis, &_lanc_wym, &_karta, 0, &_rend
	);
}
void Grafika::tworz_ob_rend_wid() {
	ID3D11Texture2D* buf_tyl;
	_lanc_wym->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buf_tyl);
	_karta->CreateRenderTargetView(buf_tyl, 0, &_ob_rend_wid);
	buf_tyl->Release();
}
void Grafika::tworz_piks_szad() {
	D3DX11CompileFromFile(
		"szader\\efekty.fx", 0, 0, "SP", "ps_4_0", 0, 0, 0,
		&_piks_szad_buf, &_blad_szad_buf, 0
	);
	_karta->CreatePixelShader(
		_piks_szad_buf->GetBufferPointer(),
		_piks_szad_buf->GetBufferSize(),
		0, &_piks_szad
	);
}
void Grafika::tworz_rzutnia() {
	ZeroMemory(&_rzutnia, sizeof(_rzutnia));
	_rzutnia.TopLeftX = 0;
	_rzutnia.TopLeftY = 0;
	_rzutnia.Width = szerRend;
	_rzutnia.Height = wysRend;
	_rzutnia.MinDepth = 0.0f;
	_rzutnia.MaxDepth = 1.0f;
}
void Grafika::tworz_stan_prob() {
	D3D11_SAMPLER_DESC stan_prob_opis;
	ZeroMemory(&stan_prob_opis, sizeof(stan_prob_opis));
	stan_prob_opis.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	stan_prob_opis.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.ComparisonFunc = D3D11_COMPARISON_NEVER;
	stan_prob_opis.MinLOD = 0;
	stan_prob_opis.MaxLOD = D3D11_FLOAT32_MAX;
	_karta->CreateSamplerState(&stan_prob_opis, &_stan_prob);
}
void Grafika::tworz_strukt_we() {
	D3D11_INPUT_ELEMENT_DESC strukt_we_opis[] = {
		{"POZYCJA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEKSTURA", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	_karta->CreateInputLayout(
		strukt_we_opis, ARRAYSIZE(strukt_we_opis), _wierz_szad_buf->GetBufferPointer(),
		_wierz_szad_buf->GetBufferSize(), &_strukt_we
	);
}
void Grafika::tworz_wierz_szad() {
	D3DX11CompileFromFile(
		"szader\\efekty.fx", 0, 0, "SW", "vs_4_0", 0, 0, 0,
		&_wierz_szad_buf, &_blad_szad_buf, 0
	);
	_karta->CreateVertexShader(
		_wierz_szad_buf->GetBufferPointer(),
		_wierz_szad_buf->GetBufferSize(),
		0, &_wierz_szad
	);
}
void Grafika::ustaw_kam_cel(float const& x, float const& y, float const& z) {
	_kam_cel = XMFLOAT3(x, y, z);
}
void Grafika::ustaw_kam_gora(float const& x, float const& y, float const& z) {
	_kam_gora = XMFLOAT3(x, y, z);
}
void Grafika::ustaw_kam_poz(float const& x, float const& y, float const& z) {
	_kam_poz = XMFLOAT3(x, y, z);
}
void Grafika::ustaw_proj_blizsza(float const& odl) {
	_proj_blizsza = odl;
}
void Grafika::ustaw_proj_dalsza(float const& odl) {
	_proj_dalsza = odl;
}
void Grafika::ustaw_proj_kat(float const& kat) {
	_proj_kat = kat;
}
void Grafika::wiaz_wierz_buf() {
	ID3D11Buffer* wierz_buf[] = {_wierz_buf, _wierz_teks_buf};
	uint32_t kroki[] = {_ob->wierz.wez_rozm(), _ob->wierz_teks.wez_rozm()};
	uint32_t przes[] = {0, _ob->wierz.wez_il()*_ob->wierz.wez_rozm()};
	_rend->IASetVertexBuffers(0, 2, wierz_buf, kroki, przes);
}
void Grafika::wyk_co_klatka() {
	aktual_wierz_buf();
	aktual_wierz_teks_buf();
	aktual_ind_buf();
	wiaz_wierz_buf();
	_rend->IASetIndexBuffer(_ind_buf, DXGI_FORMAT_R32_UINT, 0);

	for(uint32_t i = 0; i < _ob->nr.wez_il(); ++i) {
		wyk_co_ob(_ob->nr[i]);
	}

	_lanc_wym->Present(0, 0);
}
void Grafika::wyk_co_ob(uint32_t const& nr) {
	_co_ob_dane.mac_swp = XMMatrixTranspose(
		XMMatrixTranslation(_ob->przes[nr].x, _ob->przes[nr].y, _ob->przes[nr].z) *
		XMMatrixLookAtLH(XMLoadFloat3(&_kam_poz), XMLoadFloat3(&_kam_cel), XMLoadFloat3(&_kam_gora)) *
		XMMatrixPerspectiveFovLH(_proj_kat*3.14f/180, float(szerRend)/wysRend, _proj_blizsza, _proj_dalsza)
	);
	_rend->UpdateSubresource(_co_ob_buf, 0, 0, &_co_ob_dane, 0, 0);

	_rend->VSSetConstantBuffers(0, 1, &_co_ob_buf);
	//_rend->PSSetShaderResources(0, 1, &_ob->teks[*_ob->_teks_mapa[nr]].pierw);
	//_rend->DrawIndexed(_ob->ind.wez_seg(nr).drug, _ob->ind.wez_seg(nr).pierw, 0);
}
void Grafika::wyk_raz() {
	tworz_karta_rend_lanc();
	tworz_ob_rend_wid();
	tworz_gleb_szab_buf_wid();
	_rend->OMSetRenderTargets(1, &_ob_rend_wid, _gleb_szab_wid);
	tworz_buf<XMFLOAT3>(_karta, _wierz_buf, _ob->wierz.wez_il(), D3D11_BIND_VERTEX_BUFFER);
	tworz_buf<XMFLOAT2>(_karta, _wierz_teks_buf, _ob->wierz_teks.wez_il(), D3D11_BIND_VERTEX_BUFFER);
	tworz_wierz_szad();
	_rend->VSSetShader(_wierz_szad, 0, 0);
	tworz_buf<DWORD>(_karta, _ind_buf, _ob->ind.wez_il(), D3D11_BIND_INDEX_BUFFER);
	tworz_piks_szad();
	_rend->PSSetShader(_piks_szad, 0, 0);
	tworz_strukt_we();
	_rend->IASetInputLayout(_strukt_we);
	_rend->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tworz_rzutnia();
	_rend->RSSetViewports(1, &_rzutnia);
	tworz_stan_prob();
	_rend->PSSetSamplers(0, 1, &_stan_prob);
	tworz_buf<CoObDane>(_karta, _co_ob_buf, 1, D3D11_BIND_CONSTANT_BUFFER);
	_rend->VSSetConstantBuffers(0, 1, &_co_ob_buf);
	ustaw_kam_poz(+0.0f, +0.0f, -0.5f);
	ustaw_kam_cel(+0.0f, +0.0f, +0.0f);
	ustaw_kam_gora(+0.0f, +1.0f, +0.0f);
	ustaw_proj_kat(90);
	ustaw_proj_blizsza(+0.5f);
	ustaw_proj_dalsza(+100.0f);
}
Grafika::CoObDane::CoObDane()
	: mac_swp(XMMatrixIdentity()) {
}




