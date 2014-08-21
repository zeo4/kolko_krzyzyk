#pragma once

#include <wekt.h>

// -------------------------------------------------------
ListaWolne::ListaWolne() {
	el_il = el_poj;
	for(uint32_t _i = 0; _i < el_il; ++_i) {
		el[_i] = el_poj-1-_i;
	}
}
void ListaWolne::rezerw(uint32_t const& _poj) {
	uint32_t _poj_stara = el_poj;
	WekStos::rezerw(_poj);
	for(uint32_t _i = 0; _i < el_poj-_poj_stara; ++_i) {
		el[_i] = el_poj-1-_i;
	}
}
// -------------------------------------------------------
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
// -------------------------------------------------------




