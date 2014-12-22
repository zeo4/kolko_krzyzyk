#pragma once
#include <vec.h>
// -------------------------------------------------------
void VecSparse<bool>::update(uint32_t const*const& _map) {}
void VecSparse<bool>::clear() {
	memset(el, 0, cap * sizeof(bool));
}
void VecSparse<bool>::reserve_back(uint32_t const& _cap) {
	if(_cap <= cap) return;

	bool*const _el = (bool*)((uint8_t*)malloc(_cap * sizeof(bool) + 12) + 12);
	memcpy(_el, el, cap * sizeof(bool));
	memset(_el + cap, 0, (_cap - cap) * sizeof(bool));
	free((uint8_t*)el - 12);
	el = _el;
	cap = _cap;
}
// -------------------------------------------------------



