#pragma once

#include <wektor.h>

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





