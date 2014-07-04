#pragma once

#include <stdint.h>

template<class T>
class FunHasz {
public:
	inline uint32_t		operator()(T const) const;
};
template<class T>
uint32_t FunHasz<T>::operator()(T const wart) const {
	return wart;
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


