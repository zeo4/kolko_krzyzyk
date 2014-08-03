#pragma once

#include <wekt.h>

void MenPam::licz_uloz_unikat(uint32_t*& el_ind) {
	if(_il < 2) return;

	uint32_t hasz_min, hasz_maks, il_zakres, i, il;
	licz_zakres(hasz_min, hasz_maks);
	il_zakres = hasz_maks - hasz_min + 1;
	free(el_ind);
	el_ind = (uint32_t*const)malloc(_il*4);
	uint32_t*const hasz_ind = (uint32_t*const)malloc(il_zakres*4);
	memset(hasz_ind, 0, il_zakres*4);

	// licz hasze, oznacz policzone hasze 1-nką
	for(i = 0; i < _il; ++i) {
		el_ind[i] = wez_hasz(operator[](i)) - hasz_min;
		if(hasz_ind[el_ind[i]]) {
			el_ind[i] = -1;
		} else {
			hasz_ind[el_ind[i]] = 1;
		}
	}

	// licz indeksy haszy
	il = 0;
	for(i = 0; i < il_zakres; ++i) {
		if(hasz_ind[i]) {
			hasz_ind[i] = il++;
		}
	}

	// wpisz nowe indeksy elementów do mapy "element na indeks"
	for(i = 0; i < _il; ++i) {
		if(el_ind[i] != -1) {
			el_ind[i] = hasz_ind[el_ind[i]];
		}
	}
}
void MenPam::licz_uloz(uint32_t*& el_ind) {
	if(_il < 2) return;

	uint32_t hasz_min, hasz_maks, il_zakres, i, il;
	licz_zakres(hasz_min, hasz_maks);
	il_zakres = hasz_maks - hasz_min + 1;
	free(el_ind);
	el_ind = (uint32_t*const)malloc(_il*4);
	uint32_t*const hasz_ind = (uint32_t*const)malloc(il_zakres*4);
	uint32_t*const hasz_powt = (uint32_t*const)malloc(il_zakres*4);
	memset(hasz_powt, 0, il_zakres*4);

	// licz hasze i ilość powtórzonych haszy
	for(i = 0; i < _il; ++i) {
		el_ind[i] = wez_hasz(operator[](i)) - hasz_min;
		++hasz_powt[el_ind[i]];
	}

	// licz indeksy haszy
	il = 0;
	for(i = 0; i < il_zakres; ++i) {
		if(hasz_powt[i]) {
			hasz_ind[i] = il;
			il += hasz_powt[i];
		}
	}

	// licz indeksy elementów
	memset(hasz_powt, 0, il_zakres*4);
	uint32_t hasz;
	for(i = 0; i < _il; ++i) {
		hasz = el_ind[i];
		el_ind[i] = hasz_ind[hasz] + hasz_powt[hasz]++;
	}
}
void MenPam::licz_zakres(uint32_t& hasz_min, uint32_t& hasz_maks) const {
	hasz_min = wez_hasz(operator[](0));
	hasz_maks = wez_hasz(operator[](0));
	uint32_t hasz;
	for(uint32_t i = 1; i < _il; ++i) {
		hasz = wez_hasz(operator[](i));
		if(hasz < hasz_min) {
			hasz_min = hasz;
		} else if(hasz > hasz_maks) {
			hasz_maks = hasz;
		}
	}
}
void MenPam::rezerw_kon(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	char* pam = (char*)malloc(il*_typ_rozm);
	memmove(pam, *_p, _il*_typ_rozm);
	free(*_p);
	*_p = pam;
	_il_rezerw = il;
}
void MenPam::rezerw_pocz(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	char* pam = (char*)malloc(il*_typ_rozm);
	memmove(pam+(il-_il_rezerw)*_typ_rozm, *_p, _il*_typ_rozm);
	free(*_p);
	*_p = pam;
	_il_rezerw = il;
	_il += il-_il_rezerw;
}
void MenPam::uloz(uint32_t const*const& el_ind) {
	char*const pam = (char*const)malloc(_il_rezerw*_typ_rozm);
	uint32_t il = 0;
	for(uint32_t i = 0; i < _il; ++i) {
		if(el_ind[i] != -1) {
			memmove(pam+el_ind[i]*_typ_rozm, operator[](i), _typ_rozm);
			++il;
		}
	}
	free(*_p);
	*_p = pam;
	_il = il;
}
uint32_t MenPam::wez_hasz(char const*const& pam) const {
	return *(uint32_t*)pam;
}
void MenPam::zamien(uint32_t const& nr1, uint32_t const& nr2) {
	char* pam0 = (char*)malloc(_typ_rozm);
	char* pam1 = operator[](nr1);
	char* pam2 = operator[](nr2);
	memmove(pam0, pam1, _typ_rozm);
	memmove(pam1, pam2, _typ_rozm);
	memmove(pam2, pam0, _typ_rozm);
}

WektWolne::WektWolne() {
	_il = _il_rezerw;
	for(uint32_t i = 0; i < _il; ++i) {
		_tab[i] = _il-1-i;
	}
}
void WektWolne::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	uint32_t il_nowych = il-_il_rezerw;
	uint32_t* pam = (uint32_t*)malloc(il*_rozm_el);
	memmove(pam+il_nowych, _tab, _il*_rozm_el);
	for(uint32_t i = 0; i < il_nowych; ++i) {
		pam[i] = il-1-i;
	}
	free(_tab);
	_tab = pam;
	_il_rezerw = il;
	_il += il_nowych;
}

void MenUch::niszcz(uint32_t const& uch) {

}
uint32_t MenUch::tworz(uint32_t const& nr) {
	if(_il == _il_rezerw) rezerwuj(_il_rezerw*2);
	++_il;
	_tab[_wolne.wez()] = nr;
	return _wolne.usun();
}



