#pragma once

#include <stdint.h>
#include <bitset>

using std::bitset;

// -------------------------------------------------------
template<class T>
T rzut_bity(bitset<sizeof(T)*8>& _bity) {
	T _wart;
	memmove(&_wart, &_bity, sizeof(T));
	return _wart;
}
template<class T>
T gen_maks() {
	bitset<sizeof(T)*8> _bity;
	_bity.set();
	_bity.reset(sizeof(T)*8-1);
	return rzut_bity<T>(_bity);
}
template<class T>
T gen_min() {
	bitset<sizeof(T)*8> _bity;
	_bity.reset();
	_bity.set(sizeof(T)*8-1);
	return rzut_bity<T>(_bity);
}
// -------------------------------------------------------
template<class T>
void wyp_pam(T*const& pam, T const& _wart, uint32_t const& _il) {
	for(int32_t _i = 0; _i < _il; ++_i) {
		memmove(_pam+_i, &_wart, sizeof(T));
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


