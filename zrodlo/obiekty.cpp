#pragma once

#include <stdint.h>
#include <obiekty.h>
#include <string>

using std::string;

//Obiekty3w::Obiekty3w() 
//	: _teks_dane((TeksturaDane_*)malloc(8*sizeof(TeksturaDane_))) {
//}
//void Obiekty3w::pakuj() {
//	WektWpis<uint32_t> el_do_usun = _wierz.wez_do_usun();
//	for(uint32_t i = 0; i < el_do_usun.wez_il(); ++i) {
//		_nr.usun(el_do_usun[i]); /*tutaj źle*/
//		--_teks_dane[*_teks_mapa[el_do_usun[i]]].drug;
//		if(_teks_dane[*_teks_mapa[el_do_usun[i]]].drug == 0) {
//			_teks_dane[*_teks_mapa[el_do_usun[i]]].pierw->Release();
//			_teks_dane[*_teks_mapa[el_do_usun[i]]].pierw = 0;
//			_teks_mapa.usun_zbierz(el_do_usun[i]);
//		}
//	}
//	_wierz.usun_wyk();
//	_wierz_teks.usun_wyk();
//	_teks_mapa.usun_wyk();
//	_ind.usun_wyk();
//}
//void Obiekty3w::usun(uint32_t const& _nr) {
//	wierz.usun_zbierz(_nr);
//	wierz_teks.usun_zbierz(_nr);
//	ind.usun_zbierz(_nr);
//}
void Obiekty3w::tworz_ob(XMFLOAT3* wierz, XMFLOAT2* wierz_teks, uint32_t const& il_wierz, DWORD* indeksy, uint32_t const& il_indeksy, uint32_t const& teks_nr) {
	//_nr.wstaw_kon(_wierz.wstaw_kon(wierz, il_wierz));
	//_wierz_teks.wstaw_kon(wierz_teks, il_wierz);

	//_ind.wstaw_kon(indeksy, il_indeksy);

	//_teks_mapa.wstaw_kon(&teks_nr, 1); /*tutaj*/
	///*tutaj sprawdzać i powiększać _teks_dane zgodnie z _teks_mapa*/
	//++_teks_dane[teks_nr].drug;
	//if(_teks_dane[teks_nr].pierw != 0) return;
	//D3DX11CreateShaderResourceViewFromFile(zasoby.karta, wez_sciez_teks(teks_nr), 0, 0, &_teks_dane[teks_nr].pierw, 0);

	//_przes.wstaw_kon(&XMFLOAT3(+0.0f, +0.0f, +0.0f), 1);
}
//char const* Obiekty3w::wez_sciez_teks(uint32_t const& _nr) const {
//	switch(_nr) {
//	case 1: return "tekstura\\1.jpg";
//	case 2: return "tekstura\\2.jpg";
//	}
//}

/*
void Obiekt3w::wezKolizjePromien(MapaFloatObiekt_* const odlKolizje, XMVECTOR const pocz, XMVECTOR const kier) const {
	XMVECTOR p = pocz;
	XMVECTOR k = kier;
	fiz->usunSwiatPkt(&p);
	fiz->usunSwiatWektor(&k);
	fiz->wezKolizjePromien(odlKolizje, p, k);
}*/





