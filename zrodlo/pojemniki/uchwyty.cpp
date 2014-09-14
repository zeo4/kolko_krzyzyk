#pragma once

#include <windows.h>
#include <stdint.h>
#include <uchwyty.h>

Uch::Uch()
	: uch((uint32_t*)malloc(256*4)),
	poj(256),
	wolny(0) {
	for(uint32_t _i = 0; _i < poj-1; ++_i) {
		uch[_i] = 0x80000000 | _i+1;
	}
	uch[poj-1] = 0xffffffff;
}
Uch::~Uch() {
	free(uch);
}
void Uch::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= poj) return;

	uint32_t*const _uch = (uint32_t*)malloc(_poj*4);
	memmove(_uch, uch, poj*4);
	free(uch);
	uch = _uch;
	for(uint32_t _i = poj; _i < _poj-1; ++_i) {
		uch[_i] = 0x80000000 | _i+1;
	}
	uch[_poj-1] = 0xffffffff;
	wolny = poj;
	poj = _poj;
}




