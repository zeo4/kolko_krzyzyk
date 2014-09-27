#pragma once

#include <windows.h>
#include <uch.h>

Uch::Uch()
	: el((uint32_t*)malloc(256*4)),
	poj(256),
	wolny(0) {
	for(uint32_t _i = 0; _i < poj-1; ++_i) {
		el[_i] = 0x80000000 | _i+1;
	}
	el[poj-1] = 0xffffffff;
}
Uch::~Uch() {
	free(el);
}
void Uch::aktual(uint32_t const*const& _mapa) {
	for(uint32_t _i = 0; _i < poj; ++_i) {
		if(el[_i] & 0x80000000) continue;
		if(_mapa[el[_i]] == 0x80000000) continue;
		el[_i] = _mapa[el[_i]];
	}
}
void Uch::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= poj) return;

	uint32_t*const _el = (uint32_t*)malloc(_poj*4);
	memcpy(_el, el, poj*4);
	free(el);
	el = _el;
	wolny = poj;
	for(poj; poj < _poj-1; ++poj) {
		el[poj] = 0x80000000 | poj+1;
	}
	el[poj++] = 0xffffffff;
}
uint32_t Uch::wstaw(uint32_t const& _nr) {
	if((el[wolny] & 0x7fffffff) >= poj) rezerw_tyl(poj*2);
	uint32_t _nr_wstaw = wolny;
	wolny = el[wolny] & 0x7fffffff;
	el[_nr_wstaw] = _nr;
	return _nr_wstaw;
}



