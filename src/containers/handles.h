#pragma once
#include <stdint.h>
// -------------------------------------------------------
class UchPula {
public:
								UchPula();
								UchPula(uint32_t*const);
								UchPula(uint32_t*const,
									uint32_t const, uint32_t const);
	inline uint32_t&			operator[](uint32_t const) const;
	void						rezerw_tyl(uint32_t const);
	inline void					destroy();
	uint32_t					wstaw(uint32_t const);
	inline void					usun(uint32_t const);
	inline uint32_t const&		wez_poj() const;
	void						aktual(uint32_t const*const);
	inline void					aktual_nagl();
	inline bool					sprawdz_pusty(uint32_t const) const;
protected:
	uint32_t					poj;
	uint32_t					wolny;
	uint32_t*					el;
};
uint32_t& UchPula::operator[](uint32_t const _nr) const {
	return el[_nr];
}
void UchPula::aktual_nagl() {
	memcpy((uint8_t*)el - 12, this, 4);
	memcpy((uint8_t*)el - 4, (uint8_t*)this + 4, 4);
}
void UchPula::destroy() {
	free((uint8_t*)el - 12);
}
bool UchPula::sprawdz_pusty(uint32_t const _nr) const {
	return el[_nr] & 0x80000000;
}
void UchPula::usun(uint32_t const _nr) {
	if(el[_nr] & 0x80000000) return;
	el[_nr] = 0x80000000 | wolny;
	wolny = _nr;
}
uint32_t const& UchPula::wez_poj() const {
	return poj;
}
// -------------------------------------------------------



