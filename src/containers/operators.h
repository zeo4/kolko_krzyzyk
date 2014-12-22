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
T gen_empty() {
	T wart;
	memset(&wart, 0, sizeof(T));
	*((uint8_t*)&wart + sizeof(T) - 1) = 1 << 7;
	return wart;
}
// -------------------------------------------------------
template<class T>
void wyp_pam(T*const _pam, T const& _wart, uint32_t const& _il) {
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
uint32_t FunHasz<T>::operator()(T const& _wart) const {
	return *(uint32_t*)&_wart;
}
// -------------------------------------------------------
template<class T>
class FunHasz2 {
public:
	inline uint32_t		operator()(T const&) const;
};
template<class T>
uint32_t FunHasz2<T>::operator()(T const& _wart) const {
	return *((uint32_t*)&_wart + 1);
}



