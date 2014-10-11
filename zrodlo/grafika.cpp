#pragma once

#include <debug.h>
#include <grafika.h>
#include <uch.h>
// -------------------------------------------------------
void Grafika::aktual_co_ob(uint32_t const& _nr_ob) {
	zas.co_ob.mac_swp = XMMatrixTranspose(
		XMLoadFloat4x4(&par_fiz.mac_swiat[_nr_ob]) *
		XMLoadFloat4x4(&kam.mac_wid) *
		XMLoadFloat4x4(&kam.mac_proj)
	);
}
void Grafika::defrag(uint32_t*& _mapa1) {
	free(_mapa1);
	uint32_t* _mapa2 = 0;

	ob.mod_wierz.defrag_licz(_mapa1, _mapa2, ob.mod_wierz.wez_il_wier());
	ob.mod_wierz.defrag_wyk(_mapa1, _mapa2);
	ob.mod_teks.defrag_wyk(_mapa1, _mapa2);
	ob.mod_ind.defrag_wyk(_mapa1, _mapa2);
	ob.mod_nr.aktual(_mapa1);
	ob.mod_odn.aktual(_mapa1);

	ob.teks_wid.defrag_licz(_mapa1, ob.teks_wid.wez_il());
	ob.teks_wid.defrag_wyk(_mapa1);
	ob.teks_nr.aktual(_mapa1);
	ob.teks_odn.aktual(_mapa1);

	ob.mod_uch.defrag_licz(_mapa1, ob.mod_uch.wez_il());
	ob.mod_uch.defrag_wyk(_mapa1);
	ob.teks_uch.defrag_wyk(_mapa1);

	free(_mapa2);
}
void Grafika::rys_klatka() {
	zas.czysc_ekran();
	zas.aktual_mod_wierz(ob.mod_wierz);
	zas.aktual_mod_teks(ob.mod_teks);
	zas.aktual_mod_ind(ob.mod_ind);
	zas.wiaz_mod_wierz(ob.mod_wierz.wez_il_el());
	zas.wiaz_mod_ind();

	for(uint32_t _nr_ob = 0; _nr_ob < ob.mod_uch.wez_il(); ++_nr_ob) {
		rys_ob(_nr_ob);
	}

	zas.lanc->Present(0, 0);
}
void Grafika::rys_ob(uint32_t const& _nr_ob) {
	if(ob.mod_uch[_nr_ob] == ob.mod_uch.pusty) return;

	aktual_co_ob(_nr_ob);
	zas.aktual_co_ob();

	zas.wiaz_teks(ob.teks_wid[ob.teks_nr[ob.teks_uch[_nr_ob]]]);
	zas.rys_model(ob.mod_ind.wez_wier(_nr_ob).drug, ob.mod_ind.wez_wier(_nr_ob).pierw);
}
void Grafika::tworz_mod(uint32_t const& _uch_mod) {
	if(ob.mod_nr[_uch_mod] != ob.mod_nr.pusty) {
		++ob.mod_odn[_uch_mod];
		return;
	} else {
		ob.mod_nr[_uch_mod] = ob.mod_wierz.wez_il_wier();
		ob.mod_odn[_uch_mod] = 1;
	}
	if(_uch_mod == MOD_TROJKAT) {
		XMFLOAT3 _w[] = {
			XMFLOAT3(0.0f, -0.2f, 0.0f),
			XMFLOAT3(0.0f, 0.0f, 0.0f),
			XMFLOAT3(0.2f, -0.0f, 0.0f),
		};
		XMFLOAT2 _t[] = {
			XMFLOAT2(0.0f, 1.0f),
			XMFLOAT2(0.0f, 0.0f),
			XMFLOAT2(1.0f, 0.0f),
		};
		DWORD _i[] = {0, 1, 2};
		ob.mod_wierz.wstaw_kon(&_w[0], 3);
		ob.mod_teks.wstaw_kon(&_t[0], 3);
		ob.mod_ind.wstaw_kon(&_i[0], 3);
	} else if(_uch_mod == MOD_KWADRAT) {
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
		ob.mod_wierz.wstaw_kon(&_w[0], 4);
		ob.mod_teks.wstaw_kon(&_t[0], 4);
		ob.mod_ind.wstaw_kon(&_i[0], 6);
	} else if(_uch_mod == MOD_DIAMENT) {
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
		ob.mod_wierz.wstaw_kon(&_w[0], 6);
		ob.mod_teks.wstaw_kon(&_t[0], 6);
		ob.mod_ind.wstaw_kon(&_i[0], 24);
	}
}
uint32_t Grafika::tworz_ob(uint32_t const& _uch_mod, uint32_t const& _uch_teks) {
	tworz_mod(_uch_mod);
	tworz_teks(_uch_teks);
	ob.mod_uch.wstaw_kon(_uch_mod);
	ob.teks_uch.wstaw_kon(_uch_teks);
	return ob.nr.wstaw(ob.mod_uch.wez_il()-1);
}
void Grafika::tworz_teks(uint32_t const& _uch_teks) {
	if(ob.teks_nr[_uch_teks] != ob.teks_nr.pusty) {
		++ob.teks_odn[_uch_teks];
		return;
	} else {
		ob.teks_nr[_uch_teks] = ob.teks_wid.wez_il();
		ob.teks_odn[_uch_teks] = 1;
	}
	ob.teks_wid.wstaw_kon(0);
	D3DX11CreateShaderResourceViewFromFile(zas.karta, wez_teks_sciezka(_uch_teks), 0, 0, &ob.teks_wid[ob.teks_wid.wez_il()-1], 0);
}
void Grafika::usun_ob(uint32_t const& _nr) {
	// usuń model
	uint32_t _uch_model = ob.mod_uch[_nr];
	uint32_t _nr_model = ob.mod_nr[_uch_model];
	if(ob.mod_odn[_uch_model] > 1) --ob.mod_odn[_uch_model];
	else {
		ob.mod_nr[_uch_model] = ob.mod_nr.pusty;
		ob.mod_odn[_uch_model] = 0;
		ob.mod_wierz.usun(_nr_model);
		ob.mod_teks.usun(_nr_model);
		ob.mod_ind.usun(_nr_model);
	}

	// usuń teksturę
	uint32_t _uch_teks = ob.teks_uch[_nr];
	uint32_t _nr_teks = ob.teks_nr[_uch_teks];
	if(ob.teks_odn[_uch_teks] > 1) --ob.teks_odn[_uch_teks];
	else {
		ob.teks_nr[_uch_teks] = ob.teks_nr.pusty;
		ob.teks_odn[_uch_teks] = 0;
		ob.teks_wid[_nr_teks]->Release();
	}

	// usuń obiekt
	ob.mod_uch.usun(_nr);
	ob.teks_uch.usun(_nr);
}
void Grafika::uwzgl_proj() {
	XMStoreFloat4x4(&kam.mac_proj, XMMatrixPerspectiveFovLH(
		kam.kat*3.14f/180, float(szerRend)/wysRend, kam.blizsza, kam.dalsza
	));
}
void Grafika::uwzgl_wid() {
	XMStoreFloat4x4(&kam.mac_wid, XMMatrixLookAtLH(
		XMLoadFloat3(&kam.poz), XMLoadFloat3(&kam.cel), XMLoadFloat3(&kam.gora)
	));
}
char const*const Grafika::wez_teks_sciezka(uint32_t const& _uch_teks) const {
	switch(_uch_teks) {
	case TEKS_TROJKAT:
		return "tekstury/kursor.jpg";
	case TEKS_KWADRAT:
		return "tekstury/kwadrat.jpg";
	case TEKS_DIAMENT:
		return "tekstury/diament.jpg";
	}
}
void Grafika::wykonaj() {
	uwzgl_wid();
	uwzgl_proj();
}
// -------------------------------------------------------




