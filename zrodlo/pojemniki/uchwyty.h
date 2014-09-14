#pragma once

#include <stdint.h>

// -------------------------------------------------------
class Uch {
public:
						Uch();
						~Uch();
	void				rezerw_tyl(uint32_t const&);
	inline uint32_t		wstaw(uint32_t const&);
	inline void			usun(uint32_t const&);
	inline void			aktual(uint32_t const*const&);
protected:
	uint32_t*			uch;
	uint32_t			poj;
	uint32_t			wolny;
};
void Uch::aktual(uint32_t const*const& _mapa) {
	for(uint32_t _i = 0; _i < poj; ++_i) {
		if(!(uch[_i] & 0x80000000)) uch[_i] = _mapa[uch[_i]];
	}
}
void Uch::usun(uint32_t const& _nr) {
	if(uch[_nr] & 0x80000000) return;
	uch[_nr] = 0x80000000 | wolny;
	wolny = _nr;
}
uint32_t Uch::wstaw(uint32_t const& _nr) {
	if(wolny == 0x7fffffff) rezerw_tyl(poj*2);
	uint32_t _wolny = wolny;
	wolny = uch[wolny] & 0x7fffffff;
	uch[_wolny] = _nr;
	return _wolny;
}
// -------------------------------------------------------


