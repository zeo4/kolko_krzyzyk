#ifndef _GLOBALNE_C_
#define _GLOBALNE_C_

#include "globalne.h"

template<class TYP>
Wsk<TYP>::Wsk() : wskaznik(NULL)
	{}
template<class TYP>
Wsk<TYP>::Wsk(
	TYP*		w
	) : wskaznik(w)
	{}
template<class TYP>
Wsk<TYP>& Wsk<TYP>::operator=(
	TYP*		wsk
	) {
	wskaznik = wsk;
	return *this;
}
template<class TYP>
TYP& Wsk<TYP>::operator*() {
	return *wskaznik;
}
template<class TYP>
TYP* Wsk<TYP>::operator->() {
	return wskaznik;
}
template<class TYP>
TYP const* Wsk<TYP>::operator->() const {
	return wskaznik;
}
template<class TYP>
Wsk<TYP>& Wsk<TYP>::operator++() {
	++wskaznik;
	return *this;
}
template<class TYP>
Wsk<TYP> Wsk<TYP>::operator++(
	int
	) {
	Wsk<TYP> w = *this;
	++wskaznik;
	return w;
}
template<class TYP>
Wsk<TYP>& Wsk<TYP>::operator--() {
	--wskaznik;
	return *this;
}
template<class TYP>
Wsk<TYP> Wsk<TYP>::operator--(
	int
	) {
	Wsk<TYP> w = *this;
	--wskaznik;
	return w;
}

#endif