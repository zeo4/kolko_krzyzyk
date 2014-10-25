#pragma once
#include <wek.h>
// -------------------------------------------------------
void WekLuz<bool>::aktual(uint32_t const*const& _mapa) {}
void WekLuz<bool>::czysc() {
	memset(el, 0, poj * sizeof(bool));
}
void WekLuz<bool>::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= poj) return;

	bool*const _el = (bool*)malloc(_poj * sizeof(bool));
	memcpy(_el, el, poj * sizeof(bool));
	memset(_el + poj, 0, (_poj - poj) * sizeof(bool));
	free(el);
	el = _el;
	poj = _poj;
}
// -------------------------------------------------------



