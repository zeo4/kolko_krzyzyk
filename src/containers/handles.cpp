#pragma once

#include <windows.h>
#include <operators.h>
#include <handles.h>
// -------------------------------------------------------
UchPula::UchPula()
	: el((uint32_t*)((uint8_t*)malloc(256 * 4 + 12) + 12)),
	poj(256),
	wolny(0) {
	for(uint32_t _i = 0; _i < poj-1; ++_i) {
		el[_i] = 0x80000000 | _i+1;
	}
	el[poj-1] = 0xffffffff;
}
UchPula::UchPula(uint32_t*const _pam) {
	memcpy(this, _pam, 4);
	memcpy((uint8_t*)this + 4, (uint8_t*)_pam + 8, 4);
	el = (uint32_t*)((uint8_t*)_pam + 12);
}
UchPula::UchPula(uint32_t*const _el, uint32_t const _poj, uint32_t const _wolny)
	: el(_el),
	poj(_poj),
	wolny(_wolny) {}
void UchPula::aktual(uint32_t const*const _mapa) {
	for(uint32_t _i = 0; _i < poj; ++_i) {
		if(el[_i] & 0x80000000) continue;
		if(_mapa[el[_i]] == 0x80000000) continue;
		el[_i] = _mapa[el[_i]];
	}
}
void UchPula::rezerw_tyl(uint32_t const _poj) {
	if(_poj <= poj) return;

	uint32_t*const _el = (uint32_t*)((uint8_t*)malloc(_poj * 4 + 12) + 12);
	memcpy(_el, el, poj*4);
	free((uint8_t*)el - 12);
	el = _el;
	wolny = poj;
	for(poj; poj < _poj-1; ++poj) {
		el[poj] = 0x80000000 | poj+1;
	}
	el[poj++] = 0xffffffff;
}
uint32_t UchPula::wstaw(uint32_t const _nr) {
	if((el[wolny] & 0x7fffffff) >= poj) rezerw_tyl(poj*2);
	uint32_t _nr_wstaw = wolny;
	wolny = el[wolny] & 0x7fffffff;
	el[_nr_wstaw] = _nr;
	return _nr_wstaw;
}
// -------------------------------------------------------



