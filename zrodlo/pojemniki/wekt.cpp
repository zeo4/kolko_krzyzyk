#pragma once

#include <wekt.h>

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
void MenPam::uloz() {
	if(_il < 2) return;

	uint32_t hasz_min, hasz_maks, il_zakres, i;
	licz_zakres(hasz_min, hasz_maks);
	il_zakres = hasz_maks - hasz_min +1;
	char*const tab_nowa = (char*)malloc(_il_rezerw*_typ_rozm);
	uint32_t*const powt = (uint32_t*)malloc(il_zakres*4);
	uint32_t*const il_przed = (uint32_t*)malloc(il_zakres*4);

	// licz powtórzone wpisy
	ZeroMemory(powt, il_zakres*4);
	for(i = 0; i < _il; ++i) {
		++powt[wez_hasz(operator[](i))-hasz_min];
	}

	// licz ilość elementów przed pierwszym elementem z danego hasza
	il_przed[0] = 0;
	for(i = 1; i < il_zakres; ++i) {
		il_przed[i] = il_przed[i-1] + powt[i-1];
	}

	// ustaw / ułóż poszczególne elementy
	ZeroMemory(powt, il_zakres*4);
	uint32_t hasz;
	for(i = 0; i < _il; ++i) {
		hasz = wez_hasz(operator[](i)) - hasz_min;
		memmove(tab_nowa+(il_przed[hasz]+powt[hasz]++)*_typ_rozm, operator[](i), _typ_rozm);
	}

	free(*_p); free(powt); free(il_przed);
	*_p = tab_nowa;
}
void MenPam::uloz_unikat() {
	if(_il < 2) return;

	uint32_t hasz_min, hasz_maks, il_zakres, il, i, ind;
	licz_zakres(hasz_min, hasz_maks);
	il_zakres = hasz_maks - hasz_min + 1;
	char*const unikaty = (char*)malloc(il_zakres*_typ_rozm);
	bool*const wpisane = (bool*)malloc(il_zakres);
	ZeroMemory(wpisane, il_zakres);

	// przepisz sortując / usuwając podwójne elementy
	il = 0;
	for(i = 0; i < _il; ++i) {
		ind = wez_hasz(operator[](i)) - hasz_min;
		if(wpisane[ind] == false) {
			memmove(unikaty+ind*_typ_rozm, operator[](i), _typ_rozm);
			wpisane[ind] = true;
			++il;
		}
	}

	// przepisz usuwając puste komórki
	_il = 0;
	for(i = 0; i < il_zakres; ++i) {
		if(wpisane[i]) {
			memmove(operator[](_il++), unikaty+i*_typ_rozm, _typ_rozm);
		}
	}

	free(unikaty); free(wpisane);
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



