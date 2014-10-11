#pragma once
#include <stdint.h>
// -------------------------------------------------------
class UchLuz {
public:
							UchLuz();
							~UchLuz();
	inline uint32_t&		operator[](uint32_t const&);
	void					rezerw_tyl(uint32_t const&);
	void					aktual(uint32_t const*const&);
	static uint32_t			pusty;
protected:
	uint32_t*				el;
	uint32_t				poj;
};
uint32_t& UchLuz::operator[](uint32_t const& _nr) {
	while(_nr >= poj) rezerw_tyl(2*poj);
	return el[_nr];
}
// -------------------------------------------------------
class UchPula {
public:
								UchPula();
								~UchPula();
	inline uint32_t&			operator[](uint32_t const&) const;
	void						rezerw_tyl(uint32_t const&);
	uint32_t					wstaw(uint32_t const&);
	inline void					usun(uint32_t const&);
	inline uint32_t const&		wez_poj() const;
	void						aktual(uint32_t const*const&);
	inline bool					sprawdz_pusty(uint32_t const&) const;
protected:
	uint32_t*					el;
	uint32_t					poj;
	uint32_t					wolny;
};
uint32_t& UchPula::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
bool UchPula::sprawdz_pusty(uint32_t const& _nr) const {
	return el[_nr] & 0x80000000;
}
void UchPula::usun(uint32_t const& _nr) {
	if(el[_nr] & 0x80000000) return;
	el[_nr] = 0x80000000 | wolny;
	wolny = _nr;
}
uint32_t const& UchPula::wez_poj() const {
	return poj;
}
// -------------------------------------------------------



