#pragma once

#include <grafika.h>
#include <uch.h>
// -------------------------------------------------------
CoOb::CoOb()
	: mac_swp(XMMatrixIdentity()) {}
// -------------------------------------------------------
ZasobyGraf::ZasobyGraf()
	: karta(0),
	lanc(0),
	rend(0),
	model_wierz_buf(0),
	model_teks_buf(0),
	model_ind_buf(0),
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
	model_wierz_buf->Release();
	model_teks_buf->Release();
	model_ind_buf->Release();
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
void ZasobyGraf::aktual_model_ind(Wek2<DWORD>const& _model_ind) {
	D3D11_BUFFER_DESC _opis;
	model_ind_buf->GetDesc(&_opis);
	if(_model_ind.wez_il2()*sizeof(XMFLOAT2) > _opis.ByteWidth) {
		tworz_buf<DWORD>(karta, model_ind_buf, _model_ind.wez_il2(), D3D11_BIND_INDEX_BUFFER);
	}
	rend->UpdateSubresource(model_ind_buf, 0, 0, &_model_ind[0], 0, 0);
}
void ZasobyGraf::aktual_model_wierz(Wek2<XMFLOAT3>const& _model_wierz) {
	D3D11_BUFFER_DESC _opis;
	model_wierz_buf->GetDesc(&_opis);
	if(_model_wierz.wez_il2()*sizeof(XMFLOAT3) > _opis.ByteWidth) {
		tworz_buf<XMFLOAT3>(karta, model_wierz_buf, _model_wierz.wez_il2(), D3D11_BIND_VERTEX_BUFFER);
	}
	rend->UpdateSubresource(model_wierz_buf, 0, 0, &_model_wierz[0], 0, 0);
}
void ZasobyGraf::aktual_model_teks(Wek2<XMFLOAT2>const& _model_teks) {
	D3D11_BUFFER_DESC _opis;
	model_teks_buf->GetDesc(&_opis);
	if(_model_teks.wez_il2()*sizeof(XMFLOAT2) > _opis.ByteWidth) {
		tworz_buf<XMFLOAT2>(karta, model_teks_buf, _model_teks.wez_il2(), D3D11_BIND_VERTEX_BUFFER);
	}
	rend->UpdateSubresource(model_teks_buf, 0, 0, &_model_teks[0], 0, 0);
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
void ZasobyGraf::tworz_model_ind() {
	tworz_buf<DWORD>(karta, model_ind_buf, 1, D3D11_BIND_INDEX_BUFFER);
}
void ZasobyGraf::tworz_model_wierz() {
	tworz_buf<XMFLOAT3>(karta, model_wierz_buf, 1, D3D11_BIND_VERTEX_BUFFER);
}
void ZasobyGraf::tworz_model_teks() {
	tworz_buf<XMFLOAT2>(karta, model_teks_buf, 1, D3D11_BIND_VERTEX_BUFFER);
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
void ZasobyGraf::wiaz_model_ind()const {
	rend->IASetIndexBuffer(model_ind_buf, DXGI_FORMAT_R32_UINT, 0);
}
void ZasobyGraf::wiaz_model_wierz(uint32_t const& _il_wierz) const {
	ID3D11Buffer* _buf_we[] = {model_wierz_buf, model_teks_buf};
	uint32_t _kroki[] = {sizeof(XMFLOAT3), sizeof(XMFLOAT2)};
	uint32_t _przes[] = {0, _il_wierz * sizeof(XMFLOAT3)};
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
// -------------------------------------------------------
void Grafika::aktual_co_ob(uint32_t const& _nr) {
	co_ob.mac_swp = XMMatrixTranspose(
		XMMatrixTranslation(fiz->przes[_nr].x, fiz->przes[_nr].y, fiz->przes[_nr].z)
		* XMMatrixLookAtLH(XMLoadFloat3(&kam_poz), XMLoadFloat3(&kam_cel), XMLoadFloat3(&kam_gora))
		* XMMatrixPerspectiveFovLH(proj_kat*3.14f/180, float(szerRend)/wysRend, proj_blizsza, proj_dalsza)
	);
}
void Grafika::aktual_klatka() {
	zas.czysc_ekran();
	zas.aktual_model_wierz(model_wierz);
	zas.aktual_model_teks(model_teks);
	zas.aktual_model_ind(model_ind);
	zas.wiaz_model_wierz(model_wierz.wez_il2());
	zas.wiaz_model_ind();

	for(uint32_t _uch = 0; _uch < ob_nr.wez_poj(); ++_uch) {
		aktual_ob(_uch);
	}

	zas.lanc->Present(0, 0);
}
void Grafika::aktual_ob(uint32_t const& _uch) {
	if(ob_nr[_uch] & 0x80000000) return;
	aktual_co_ob(ob_nr[_uch]);
	zas.aktual_co_ob(co_ob);

	//_rend->PSSetShaderResources(0, 1, &_ob->teks[*_ob->_teks_mapa[_nr]].pierw);
	//_rend->DrawIndexed(_ob->ind.wez_seg(_nr).drug, _ob->ind.wez_seg(_nr).pierw, 0);
}
void Grafika::inic() {
	zas.tworz_karta_rend_lanc();
	zas.tworz_gleb_szab_buf_wid();
	zas.tworz_cel_rend_wid();
	zas.wiaz_cele_rend();
	zas.tworz_model_wierz();
	zas.tworz_model_teks();
	zas.tworz_model_ind();
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
void Grafika::usun_ob(uint32_t const& _uch) {
	// usun model
	uint32_t _uch_model = ob_model[ob_nr[_uch]];
	uint32_t _nr_model = model_nr[_uch_model];
	if(model_odn[_uch_model] > 1) --model_odn[_uch_model];
	else {
		model_nr[_uch_model] = model_nr.pusty;
		model_odn[_uch_model] = 0;
		model_wierz.usun(_nr_model);
		model_teks.usun(_nr_model);
		model_ind.usun(_nr_model);
	}

	// usun teksturę
	uint32_t _uch_teks = ob_teks[ob_nr[_uch]];
	uint32_t _nr_teks = teks_nr[_uch_teks];
	if(teks_odn[_uch_teks] > 1) --teks_odn[_uch_teks];
	else {
		teks_nr[_uch_teks] = teks_nr.pusty;
		teks_odn[_uch_teks] = 0;
		teks_wid[_nr_teks]->Release();
	}

	// usun obiekt
	ob_model.usun(ob_nr[_uch]);
	ob_teks.usun(ob_nr[_uch]);
	ob_nr.usun(_uch);
}
void Grafika::defrag() {
	uint32_t* _mapa1 = 0,* _mapa2 = 0;

	ob_model.defrag_licz(_mapa1, ob_model.wez_il());
	ob_model.defrag_wyk(_mapa1);
	ob_teks.defrag_wyk(_mapa1);
	ob_nr.aktual(_mapa1);

	model_wierz.defrag_licz(_mapa1, _mapa2, model_wierz.wez_il1());
	model_wierz.defrag_wyk(_mapa1, _mapa2);
	model_teks.defrag_wyk(_mapa1, _mapa2);
	model_ind.defrag_wyk(_mapa1, _mapa2);
	model_nr.aktual(_mapa1);
	model_odn.aktual(_mapa1);

	teks_wid.defrag_licz(_mapa1, teks_wid.wez_il());
	teks_wid.defrag_wyk(_mapa1);
	teks_nr.aktual(_mapa1);
	teks_odn.aktual(_mapa1);

	free(_mapa1); free(_mapa2);
}
void Grafika::tworz_model(uint32_t const& _uch) {
	if(model_nr[_uch] != model_nr.pusty) {
		++model_odn[_uch];
		return;
	} else {
		model_nr[_uch] = model_wierz.wez_il1();
		model_odn[_uch] = 1;
	}
	if(_uch == MOD_KURSOR) {
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
		model_wierz.wstaw_kon(&_w[0], 3);
		model_teks.wstaw_kon(&_t[0], 3);
		model_ind.wstaw_kon(&_i[0], 3);
	} else if(_uch == MOD_KWADRAT) {
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
		model_wierz.wstaw_kon(&_w[0], 4);
		model_teks.wstaw_kon(&_t[0], 4);
		model_ind.wstaw_kon(&_i[0], 6);
	} else if(_uch == MOD_DIAMENT) {
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
		model_wierz.wstaw_kon(&_w[0], 6);
		model_teks.wstaw_kon(&_t[0], 6);
		model_ind.wstaw_kon(&_i[0], 24);
	}
}
uint32_t Grafika::tworz_ob(uint32_t const& _nr_model, uint32_t const& _nr_teks) {
	ob_model.wstaw_kon(_nr_model);
	ob_teks.wstaw_kon(_nr_teks);
	tworz_model(_nr_model);
	tworz_teks(_nr_teks);
	return ob_nr.wstaw(ob_model.wez_il()-1);
}
void Grafika::tworz_teks(uint32_t const& _uch) {
	if(teks_nr[_uch] != teks_nr.pusty) {
		++teks_odn[_uch];
		return;
	} else {
		teks_nr[_uch] = teks_wid.wez_il();
		teks_odn[_uch] = 1;
	}
	teks_wid.wstaw_kon(0);
	D3DX11CreateShaderResourceViewFromFile(zas.karta, wez_teks_sciezka(_uch), 0, 0, &teks_wid[teks_wid.wez_il()-1], 0);
}
char const*const Grafika::wez_teks_sciezka(uint32_t const& _nr) const {
	switch(_nr) {
	case TEKS_KURSOR:
		return "kursor.jpg";
	case TEKS_KWADRAT:
		return "kwadrat.jpg";
	case TEKS_DIAMENT:
		return "diament.jpg";
	}
}
// -------------------------------------------------------




