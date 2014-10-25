#pragma once

#include <stdint.h>
#include <bitset>

using std::bitset;
// -------------------------------------------------------
template<class T>
T gen_maks() {
	bitset<sizeof(T)*8> _bity;
	_bity.set();
	_bity.reset(sizeof(T)*8-1);
	return rzut_bity<T>(_bity);
}
template<class T>
T gen_min() {
	T wart;
	memset(&wart, 0, sizeof(T));
	*((char*)&wart + sizeof(T) - 1) = 1<<7;
	return wart;
}
template<class T>
T gen_pusty() {
	T wart;
	memset(&wart, 0, sizeof(T));
	*((uint8_t*)&wart + sizeof(T) - 1) = 1 << 7;
	return wart;
}
template<class T>
uint32_t wez_hasz(T const& _el) {
	return *(uint32_t*)&_el;
}
// -------------------------------------------------------
template<class T>
void wyp_pam(T*const& _pam, T const& _wart, uint32_t const& _il) {
	for(int32_t _i = 0; _i < _il; ++_i) {
		memcpy(_pam+_i, &_wart, sizeof(T));
	}
}
// -------------------------------------------------------
template<class T>
class FunHasz {
public:
	inline uint32_t		operator()(T const&) const;
};
template<class T>
uint32_t FunHasz<T>::operator()(T const& wart) const {
	return *((uint32_t*)(&wart));
}

template<class T>
class FunHasz<T*> {
public:
	inline uint32_t		operator()(T const*const) const;
};
template<class T>
uint32_t FunHasz<T*>::operator()(T const*const wart) const {
	return (uint32_t)wart;
}

template<class T>
class FunMniej {
public:
	inline bool		operator()(T const, T const) const;
};
template<class T>
bool FunMniej<T>::operator()(T const ob1, T const ob2) const {
	return ob1 < ob2;
}


