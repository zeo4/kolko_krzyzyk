#pragma once

#include <debug.h>
#include <grafika.h>
#include <uch.h>
// -------------------------------------------------------
void Grafika::defrag() {
	uint32_t* _mapa1 = 0,* _mapa2 = 0;

	par_graf.mod_wierz.defrag_licz(_mapa1, _mapa2, par_graf.mod_wierz.wez_il_wier());
	par_graf.mod_wierz.defrag_wyk(_mapa1, _mapa2);
	par_graf.mod_teks.defrag_wyk(_mapa1, _mapa2);
	par_graf.mod_ind.defrag_wyk(_mapa1, _mapa2);
	par_graf.mod_nr.aktual(_mapa1);
	par_graf.mod_odn.aktual(_mapa1);

	par_graf.teks_wid.defrag_licz(_mapa1, par_graf.teks_wid.wez_il());
	par_graf.teks_wid.defrag_wyk(_mapa1);
	par_graf.teks_nr.aktual(_mapa1);
	par_graf.teks_odn.aktual(_mapa1);

	par_graf.mod_uch.defrag_licz(_mapa1, par_graf.mod_uch.wez_il());
	par_graf.mod_uch.defrag_wyk(_mapa1);
	par_graf.teks_uch.defrag_wyk(_mapa1);

	free(_mapa1);
	free(_mapa2);
}
void Grafika::rys_klatka() {
	zas.czysc_ekran();
	zas.aktual_wierz(par_graf.mod_wierz);
	zas.aktual_teks(par_graf.mod_teks);
	zas.aktual_ind(par_graf.mod_ind);
	zas.wiaz_mod_wierz();
	zas.wiaz_mod_ind();
	zas.wiaz_teks(par_graf.teks_wid[par_graf.teks_nr[par_graf.teks_uch[0]]]);
	zas.rend->DrawIndexedInstanced(par_graf.mod_ind.wez_wier(0).drug, par_graf.mod_uch.wez_il(), 0, 0, 0);
	zas.lanc->Present(0, 0);
}
void Grafika::tworz_mod(uint32_t const& _uch_mod) {
	if(par_graf.mod_nr[_uch_mod] != par_graf.mod_nr.pusty) {
		++par_graf.mod_odn[_uch_mod];
		return;
	} else {
		par_graf.mod_nr[_uch_mod] = par_graf.mod_wierz.wez_il_wier();
		par_graf.mod_odn[_uch_mod] = 1;
	}
	if(_uch_mod == MOD_TROJKAT) {
		XMFLOAT3 _wierz[] = {
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
		par_graf.mod_wierz.wstaw_kon(_wierz, 3);
		par_graf.mod_teks.wstaw_kon(_teks, 3);
		par_graf.mod_ind.wstaw_kon(_ind, 3);
	} else if(_uch_mod == MOD_KWADRAT) {
		XMFLOAT3 _wierz[] = {
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
		par_graf.mod_wierz.wstaw_kon(_wierz, 4);
		par_graf.mod_teks.wstaw_kon(_teks, 4);
		par_graf.mod_ind.wstaw_kon(_ind, 6);
	} else if(_uch_mod == MOD_DIAMENT) {
		XMFLOAT3 _wierz[] = {
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
		par_graf.mod_wierz.wstaw_kon(_wierz, 6);
		par_graf.mod_teks.wstaw_kon(_teks, 6);
		par_graf.mod_ind.wstaw_kon(_ind, 24);
	}
}
void Grafika::tworz_teks(uint32_t const& _uch_teks) {
	if(par_graf.teks_nr[_uch_teks] != par_graf.teks_nr.pusty) {
		++par_graf.teks_odn[_uch_teks];
		return;
	} else {
		par_graf.teks_nr[_uch_teks] = par_graf.teks_wid.wez_il();
		par_graf.teks_odn[_uch_teks] = 1;
	}
	par_graf.teks_wid.wstaw_kon(0);
	D3DX11CreateShaderResourceViewFromFile(zas.karta, wez_teks_sciezka(_uch_teks), 0, 0, &par_graf.teks_wid[par_graf.teks_wid.wez_il()-1], 0);
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
void Grafika::wyk_zad() {
	if(zad.wez_il_wier() > 1) {
		uint32_t* _mapa = 0;
		zad.uloz_uni_licz(_mapa);
		zad.uloz_wyk(_mapa);
	}

	for(uint32_t _i = 0; _i < zad.wez_il_wier(); ++_i) {
		if(zad.wez_wier(_i) == zad.pusty) continue;

		switch(((Zad*)zad[_i])->kod) {
		case TWORZ_OB: {
			ZadTworzOb z = *(ZadTworzOb*)zad[_i];
			WynTworzOb w;
			w.kod_zad = TWORZ_OB;
			tworz_mod(z.uch_mod);
			tworz_teks(z.uch_teks);
			par_graf.mod_uch.wstaw_kon(z.uch_mod);
			par_graf.teks_uch.wstaw_kon(z.uch_teks);
			w.uch_ob = par_graf.nr.wstaw(par_graf.mod_uch.wez_il()-1);
			wyn.wstaw_kon((uint8_t*)&w, sizeof(w));
			zad.usun(_i);
			break;
		}
		case AKTUAL_KAM: {
			XMStoreFloat4x4(&kam.mac_wid, XMMatrixLookAtLH(
				XMLoadFloat3(&kam.poz), XMLoadFloat3(&kam.cel), XMLoadFloat3(&kam.gora)
			));
			XMStoreFloat4x4(&kam.mac_proj, XMMatrixPerspectiveFovLH(
				kam.kat * 3.14f/180, float(szerRend)/wysRend, kam.blizsza, kam.dalsza
			));
			zad.usun(_i);
			break;
		}
		case AKTUAL_SWIAT: {
			Wek<XMFLOAT4X4> _mac;
			for(uint32_t _i = 0; _i < par_fiz.mac_swiat.wez_il(); ++_i) {
				_mac.wstaw_kon(XMFLOAT4X4());
				XMStoreFloat4x4(&_mac[_i], XMMatrixTranspose(
					XMLoadFloat4x4(&par_fiz.mac_swiat[_i]) *
					XMLoadFloat4x4(&kam.mac_wid) *
					XMLoadFloat4x4(&kam.mac_proj)
				));
			}
			zas.aktual_swiat(_mac);
			zad.usun(_i);
			break;
		}
		case RYSUJ:
			rys_klatka();
			zad.usun(_i);
			break;
		case DEFRAG_GRAF:
			defrag();
			zad.usun(_i);
			break;
		}
	}
}

void Grafika::usun_ob(uint32_t const& _nr) {
	// usuń model
	uint32_t _uch_model = par_graf.mod_uch[_nr];
	uint32_t _nr_model = par_graf.mod_nr[_uch_model];
	if(par_graf.mod_odn[_uch_model] > 1) --par_graf.mod_odn[_uch_model];
	else {
		par_graf.mod_nr[_uch_model] = par_graf.mod_nr.pusty;
		par_graf.mod_odn[_uch_model] = 0;
		par_graf.mod_wierz.usun(_nr_model);
		par_graf.mod_teks.usun(_nr_model);
		par_graf.mod_ind.usun(_nr_model);
	}

	// usuń teksturę
	uint32_t _uch_teks = par_graf.teks_uch[_nr];
	uint32_t _nr_teks = par_graf.teks_nr[_uch_teks];
	if(par_graf.teks_odn[_uch_teks] > 1) --par_graf.teks_odn[_uch_teks];
	else {
		par_graf.teks_nr[_uch_teks] = par_graf.teks_nr.pusty;
		par_graf.teks_odn[_uch_teks] = 0;
		par_graf.teks_wid[_nr_teks]->Release();
	}

	// usuń obiekt
	par_graf.mod_uch.usun(_nr);
	par_graf.teks_uch.usun(_nr);
}
// -------------------------------------------------------




