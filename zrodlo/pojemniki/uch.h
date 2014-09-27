#pragma once

#include <stdint.h>

// -------------------------------------------------------
class Uch {
public:
						Uch();
						~Uch();
	void				rezerw_tyl(uint32_t const&);
	uint32_t			wstaw(uint32_t const&);
	inline void			usun(uint32_t const&);
	void				aktual(uint32_t const*const&);
protected:
	uint32_t*			el;
	uint32_t			poj;
	uint32_t			wolny;
};
void Uch::usun(uint32_t const& _nr) {
	if(el[_nr] & 0x80000000) return;
	el[_nr] = 0x80000000 | wolny;
	wolny = _nr;
}
// -------------------------------------------------------


