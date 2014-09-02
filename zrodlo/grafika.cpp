#pragma once

#include "grafika.h"

CoOb::CoOb()
	: mac_swp(XMMatrixIdentity()) {}

ZasobyGraf::ZasobyGraf()
	: karta(0),
	lanc(0),
	rend(0),
	ob_wierz_buf(0),
	ob_wierz_teks_buf(0),
	ob_ind_buf(0),
	wierz_szad(0),
	wierz_szad_buf(0),
	piks_szad(0),
	piks_szad_buf(0),
	blad_szad_buf(0),
	gleb_szab_teks2(0),
	gleb_szab_wid(0),
	cel_rend_wid(0),
	stan_prob(0),
	strukt_we(0),
	co_ob_buf(0) {}
ZasobyGraf::~ZasobyGraf() {
	karta->Release();
	lanc->Release();
	rend->Release();
	ob_wierz_buf->Release();
	ob_wierz_teks_buf->Release();
	ob_ind_buf->Release();
	wierz_szad->Release();
	wierz_szad_buf->Release();
	piks_szad->Release();
	piks_szad_buf->Release();
	if(blad_szad_buf != 0) {
		blad_szad_buf->Release();
	}
	gleb_szab_teks2->Release();
	gleb_szab_wid->Release();
	cel_rend_wid->Release();
	stan_prob->Release();
	strukt_we->Release();
	co_ob_buf->Release();
}
void ZasobyGraf::aktual_co_ob(CoOb const& _co_ob) {
	rend->UpdateSubresource(co_ob_buf, 0, 0, &_co_ob, 0, 0);
}
void ZasobyGraf::aktual_ob_ind(WekSegPula<DWORD>const& _ob_ind) {
	D3D11_BUFFER_DESC _opis;
	ob_ind_buf->GetDesc(&_opis);
	if(_ob_ind.wez_el().wez_il()*sizeof(XMFLOAT2) > _opis.ByteWidth) {
		tworz_buf<DWORD>(karta, ob_ind_buf, _ob_ind.wez_el().wez_il(), D3D11_BIND_INDEX_BUFFER);
	}
	rend->UpdateSubresource(ob_ind_buf, 0, 0, &_ob_ind[0], 0, 0);
}
void ZasobyGraf::aktual_ob_wierz(WekSegPula<XMFLOAT3>const& _ob_wierz) {
	D3D11_BUFFER_DESC _opis;
	ob_wierz_buf->GetDesc(&_opis);
	if(_ob_wierz.wez_el().wez_il()*sizeof(XMFLOAT3) > _opis.ByteWidth) {
		tworz_buf<XMFLOAT3>(karta, ob_wierz_buf, _ob_wierz.wez_el().wez_il(), D3D11_BIND_VERTEX_BUFFER);
	}
	rend->UpdateSubresource(ob_wierz_buf, 0, 0, &_ob_wierz[0], 0, 0);
}
void ZasobyGraf::aktual_ob_wierz_teks(WekSegPula<XMFLOAT2>const& _ob_wierz_teks) {
	D3D11_BUFFER_DESC _opis;
	ob_wierz_teks_buf->GetDesc(&_opis);
	if(_ob_wierz_teks.wez_el().wez_il()*sizeof(XMFLOAT2) > _opis.ByteWidth) {
		tworz_buf<XMFLOAT2>(karta, ob_wierz_teks_buf, _ob_wierz_teks.wez_el().wez_il(), D3D11_BIND_VERTEX_BUFFER);
	}
	rend->UpdateSubresource(ob_wierz_teks_buf, 0, 0, &_ob_wierz_teks[0], 0, 0);
}
void ZasobyGraf::czysc_ekran() const {
	FLOAT const kolor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	rend->ClearRenderTargetView(cel_rend_wid, kolor);
	rend->ClearDepthStencilView(gleb_szab_wid, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void ZasobyGraf::tworz_cel_rend_wid() {
	ID3D11Texture2D* buf_tyl;
	lanc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buf_tyl);
	karta->CreateRenderTargetView(buf_tyl, 0, &cel_rend_wid);
	buf_tyl->Release();
}
void ZasobyGraf::tworz_co_ob() {
	tworz_buf<CoOb>(karta, co_ob_buf, 1, D3D11_BIND_CONSTANT_BUFFER);
}
void ZasobyGraf::tworz_gleb_szab_buf_wid() {
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
	karta->CreateTexture2D(&gleb_szab_opis, 0, &gleb_szab_teks2);
	karta->CreateDepthStencilView(gleb_szab_teks2, 0, &gleb_szab_wid);
}
void ZasobyGraf::tworz_karta_rend_lanc() {
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
	lanc_opis.OutputWindow = uchOknoGl;
	lanc_opis.Windowed = true;
	lanc_opis.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	D3D11CreateDeviceAndSwapChain(
		0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 0, 0,
		D3D11_SDK_VERSION, &lanc_opis, &lanc, &karta, 0, &rend
	);
}
void ZasobyGraf::tworz_ob_ind() {
	tworz_buf<DWORD>(karta, ob_ind_buf, 1, D3D11_BIND_INDEX_BUFFER);
}
void ZasobyGraf::tworz_ob_wierz() {
	tworz_buf<XMFLOAT3>(karta, ob_wierz_buf, 1, D3D11_BIND_VERTEX_BUFFER);
}
void ZasobyGraf::tworz_ob_wierz_teks() {
	tworz_buf<XMFLOAT2>(karta, ob_wierz_teks_buf, 1, D3D11_BIND_VERTEX_BUFFER);
}
void ZasobyGraf::tworz_piks_szad() {
	D3DX11CompileFromFile(
		"szader\\efekty.fx", 0, 0, "SP", "ps_4_0", 0, 0, 0,
		&piks_szad_buf, &blad_szad_buf, 0
	);
	karta->CreatePixelShader(
		piks_szad_buf->GetBufferPointer(),
		piks_szad_buf->GetBufferSize(),
		0, &piks_szad
	);
}
void ZasobyGraf::tworz_rzutnia() {
	ZeroMemory(&rzutnia, sizeof(rzutnia));
	rzutnia.TopLeftX = 0;
	rzutnia.TopLeftY = 0;
	rzutnia.Width = szerRend;
	rzutnia.Height = wysRend;
	rzutnia.MinDepth = 0.0f;
	rzutnia.MaxDepth = 1.0f;
}
void ZasobyGraf::tworz_stan_prob() {
	D3D11_SAMPLER_DESC stan_prob_opis;
	ZeroMemory(&stan_prob_opis, sizeof(stan_prob_opis));
	stan_prob_opis.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	stan_prob_opis.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	stan_prob_opis.ComparisonFunc = D3D11_COMPARISON_NEVER;
	stan_prob_opis.MinLOD = 0;
	stan_prob_opis.MaxLOD = D3D11_FLOAT32_MAX;
	karta->CreateSamplerState(&stan_prob_opis, &stan_prob);
}
void ZasobyGraf::tworz_strukt_we() {
	D3D11_INPUT_ELEMENT_DESC strukt_we_opis[] = {
		{"POZYCJA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEKSTURA", 0, DXGI_FORMAT_R32G32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	karta->CreateInputLayout(
		strukt_we_opis, ARRAYSIZE(strukt_we_opis), wierz_szad_buf->GetBufferPointer(),
		wierz_szad_buf->GetBufferSize(), &strukt_we
	);
}
void ZasobyGraf::tworz_wierz_szad() {
	D3DX11CompileFromFile(
		"szader\\efekty.fx", 0, 0, "SW", "vs_4_0", 0, 0, 0,
		&wierz_szad_buf, &blad_szad_buf, 0
	);
	karta->CreateVertexShader(
		wierz_szad_buf->GetBufferPointer(),
		wierz_szad_buf->GetBufferSize(),
		0, &wierz_szad
	);
}
void ZasobyGraf::wiaz_co_ob() const {
	rend->VSSetConstantBuffers(0, 1, &co_ob_buf);
}
void ZasobyGraf::wiaz_cele_rend() const {
	rend->OMSetRenderTargets(1, &cel_rend_wid, gleb_szab_wid);
}
void ZasobyGraf::wiaz_ob_ind(WekSegPula<DWORD>const&)const {
	rend->IASetIndexBuffer(ob_ind_buf, DXGI_FORMAT_R32_UINT, 0);
}
void ZasobyGraf::wiaz_ob_wierz(WekSegPula<XMFLOAT3>const& _ob_wierz, WekSegPula<XMFLOAT2>const& _ob_wierz_teks) const {
	ID3D11Buffer* _buf_we[] = {ob_wierz_buf, ob_wierz_teks_buf};
	uint32_t _kroki[] = {_ob_wierz.wez_el().wez_rozm(), _ob_wierz_teks.wez_el().wez_rozm()};
	uint32_t _przes[] = {0, _ob_wierz.wez_el().wez_il()*_ob_wierz.wez_el().wez_rozm()};
	rend->IASetVertexBuffers(0, 2, _buf_we, _kroki, _przes);
}
void ZasobyGraf::wiaz_piks_szad() const {
	rend->PSSetShader(piks_szad, 0, 0);
}
void ZasobyGraf::wiaz_rzutnia() const {
	rend->RSSetViewports(1, &rzutnia);
}
void ZasobyGraf::wiaz_stan_prob() const {
	rend->PSSetSamplers(0, 1, &stan_prob);
}
void ZasobyGraf::wiaz_strukt_we() const {
	rend->IASetInputLayout(strukt_we);
}
void ZasobyGraf::wiaz_topol_prym() const {
	rend->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void ZasobyGraf::wiaz_wierz_szad() const {
	rend->VSSetShader(wierz_szad, 0, 0);
}

void Grafika::aktual_co_ob(uint32_t const& _nr) {
	co_ob.mac_swp = XMMatrixTranspose(
		XMMatrixTranslation(fiz->przes[_nr].x, fiz->przes[_nr].y, fiz->przes[_nr].z)
		* XMMatrixLookAtLH(XMLoadFloat3(&kam_poz), XMLoadFloat3(&kam_cel), XMLoadFloat3(&kam_gora))
		* XMMatrixPerspectiveFovLH(proj_kat*3.14f/180, float(szerRend)/wysRend, proj_blizsza, proj_dalsza)
	);
}
void Grafika::inic() {
	zas.tworz_karta_rend_lanc();
	zas.tworz_gleb_szab_buf_wid();
	zas.tworz_cel_rend_wid();
	zas.wiaz_cele_rend();
	zas.tworz_ob_wierz();
	zas.tworz_ob_wierz_teks();
	zas.tworz_ob_ind();
	zas.tworz_wierz_szad();
	zas.wiaz_wierz_szad();
	zas.tworz_piks_szad();
	zas.wiaz_piks_szad();
	zas.tworz_strukt_we();
	zas.wiaz_strukt_we();
	zas.wiaz_topol_prym();
	zas.tworz_rzutnia();
	zas.wiaz_rzutnia();
	zas.tworz_stan_prob();
	zas.wiaz_stan_prob();
	zas.tworz_co_ob();
	zas.wiaz_co_ob();
	kam_poz = XMFLOAT3(+0.0f, +0.0f, -0.5f);
	kam_cel = XMFLOAT3(+0.0f, +0.0f, +0.0f);
	kam_gora = XMFLOAT3(+0.0f, +1.0f, +0.0f);
	proj_kat = 90;
	proj_blizsza = +0.5f;
	proj_dalsza = +100.0f;
}
void Grafika::lacz_fiz(Fizyka*const& _fiz) {
	fiz = _fiz;
}
void Grafika::maz_ob(uint32_t const& _nr_ob) {
	ob_wierz.maz(_nr_ob);
	ob_wierz_teks.maz(_nr_ob);
	ob_ind.maz(_nr_ob);
	--teks_ref_il[ob_teks[_nr_ob]];
	if(--teks_ref_il[ob_teks[_nr_ob]] < 1) {
		// niszcz teksturę
		teks_wid[ob_teks[_nr_ob]]->Release();
	}
	ob_teks.maz(_nr_ob);
}
void Grafika::pakuj() {
	ob_wierz.pakuj();
	ob_wierz_teks.pakuj();
	ob_ind.pakuj();
}
uint32_t Grafika::wpisz_model(uint32_t const& _nr_model) {
	uint32_t _uch_ob;
	if(_nr_model == MOD_KURSOR) {
		XMFLOAT3 _w[] = {
			XMFLOAT3(+0.0f, -0.2f, +0.0f),
			XMFLOAT3(+0.0f, +0.0f, +0.0f),
			XMFLOAT3(+0.2f, -0.0f, +0.0f),
		};
		XMFLOAT2 _t[] = {
			XMFLOAT2(+0.0f, +1.0f),
			XMFLOAT2(+0.5f, +0.0f),
			XMFLOAT2(+1.0f, +1.0f),
		};
		DWORD _i[] = {0, 1, 2};
		_uch_ob = ob_wierz.wstaw_kon(_w[0], 3);
		ob_wierz_teks.wstaw_kon(_t[0], 3);
		ob_ind.wstaw_kon(_i[0], 3);
	} else if(_nr_model == MOD_KWADRAT) {
		XMFLOAT3 _w[] = {
			XMFLOAT3(+0.5f, -0.5f, +0.0f),
			XMFLOAT3(-0.5f, -0.5f, +0.0f),
			XMFLOAT3(-0.5f, +0.5f, +0.0f),
			XMFLOAT3(+0.5f, +0.5f, +0.0f),
		};
		XMFLOAT2 _t[] = {
			XMFLOAT2(+1.0f, +0.0f),
			XMFLOAT2(+1.0f, +1.0f),
			XMFLOAT2(+0.0f, +1.0f),
			XMFLOAT2(+0.0f, +0.0f),
		};
		DWORD _i[] = {
			0, 1, 2,
			0, 2, 3
		};
		_uch_ob = ob_wierz.wstaw_kon(_w[0], 4);
		ob_wierz_teks.wstaw_kon(_t[0], 4);
		ob_ind.wstaw_kon(_i[0], 6);
	} else if(_nr_model == MOD_DIAMENT) {
		XMFLOAT3 _w[] = {
			XMFLOAT3(-0.25f, +0.0f, -0.25f),
			XMFLOAT3(+0.25f, +0.0f, -0.25f),
			XMFLOAT3(+0.25f, +0.0f, +0.25f),
			XMFLOAT3(-0.25f, +0.0f, +0.25f),
			XMFLOAT3(+0.0f, +0.5f, +0.0f),
			XMFLOAT3(+0.0f, -0.5f, +0.0f),
		};
		XMFLOAT2 _t[] = {
			XMFLOAT2(+0.0f, +0.5f),
			XMFLOAT2(+0.5f, +0.5f),
			XMFLOAT2(+1.0f, +0.5f),
			XMFLOAT2(+1.0f, +1.0f),
			XMFLOAT2(+0.5f, +0.0f),
			XMFLOAT2(+0.5f, +1.0f),
		};
		DWORD _i[] = {
			0, 4, 1,
			1, 5, 0,
			1, 4, 2,
			2, 5, 1,
			2, 4, 3,
			3, 5, 2,
			3, 4, 0,
			0, 5, 3,
		};
		_uch_ob = ob_wierz.wstaw_kon(_w[0], 6);
		ob_wierz_teks.wstaw_kon(_t[0], 6);
		ob_ind.wstaw_kon(_i[0], 24);
	}
	return _uch_ob;
}
uint32_t Grafika::wpisz_ob(uint32_t const& _nr_model, uint32_t const& _nr_teks) {
	uint32_t _nr_ob = wpisz_model(_nr_model);
	ob_teks.wpisz(_nr_teks, _nr_ob);
	wpisz_teks(_nr_teks);
	return _nr_ob;
}
void Grafika::wpisz_teks(uint32_t const& _nr_teks) {
	teks_wid.wpisz(0, _nr_teks);
	if(teks_ref_il[_nr_teks] < 1) {
		D3DX11CreateShaderResourceViewFromFile(zas.karta, wez_teks_sciezka(_nr_teks), 0, 0, &teks_wid[_nr_teks], 0);
	}
	++teks_ref_il[_nr_teks];
}
char const*const Grafika::wez_teks_sciezka(uint32_t const& _nr_teks) const {
	switch(_nr_teks) {
	case TEKS_KURSOR:
		return "kursor.jpg";
	case TEKS_KWADRAT:
		return "kwadrat.jpg";
	case TEKS_DIAMENT:
		return "diament.jpg";
	}
}
void Grafika::wyk_co_klatka() {
	zas.czysc_ekran();
	zas.aktual_ob_wierz(ob_wierz);
	zas.aktual_ob_wierz_teks(ob_wierz_teks);
	zas.aktual_ob_ind(ob_ind);
	zas.wiaz_ob_wierz(ob_wierz, ob_wierz_teks);
	zas.wiaz_ob_ind(ob_ind);

	for(uint32_t _i = 0; _i < ob_wierz.wez_seg().wez_poj(); ++_i) {
		if(ob_wierz.wez_seg().sprawdz_pusty(_i)) continue;
		wyk_co_ob(_i);
	}

	zas.lanc->Present(0, 0);
}
void Grafika::wyk_co_ob(uint32_t const& _nr) {
	aktual_co_ob(_nr);
	zas.aktual_co_ob(co_ob);

	//_rend->PSSetShaderResources(0, 1, &_ob->teks[*_ob->_teks_mapa[_nr]].pierw);
	//_rend->DrawIndexed(_ob->ind.wez_seg(_nr).drug, _ob->ind.wez_seg(_nr).pierw, 0);
}





