#pragma once

#include <stdint.h>
#include <assert.h>
#include <list>
#include <utility>

using std::list;
using std::forward;

template<class T>
class MenadzerPula {
public:
	#define null 0
private:
	void**				_pierwszy_wolny;
	size_t const		_rozm_ob;
	list<void*>			_segmenty;
	void*				dodaj_segment(size_t const);
	inline void*		rownaj_przod(void* const, uint32_t const) const;
public:
						MenadzerPula();
						~MenadzerPula();
	void				niszcz(T*);
	T*					przydziel(size_t);
						template<class...A>
	void				tworz(T*, A&&...);
	void				zwolnij(void*, size_t);
};
template<class T>
MenadzerPula<T>::MenadzerPula() : _rozm_ob(sizeof(T)) {
	assert(_rozm_ob >= sizeof(void*));
	dodaj_segment(10000);
}
template<class T>
MenadzerPula<T>::~MenadzerPula() {
	list<void*>::const_iterator it;
	for(it = _segmenty.begin(); it != _segmenty.end(); ++it) {
		free(*it);
	}

	_pierwszy_wolny = null;
}
template<class T>
void* MenadzerPula<T>::dodaj_segment(size_t rozm_segment) {
	_pierwszy_wolny = (void**)malloc(rozm_segment);
	_segmenty.push_back(_pierwszy_wolny);

	// twórz listę bloków pamięci segmentu
	void** wsk = _pierwszy_wolny;
	for(size_t i = 0; i < (rozm_segment/_rozm_ob)-1; ++i) {
		*wsk = reinterpret_cast<char*>(wsk) + _rozm_ob;
		wsk = (void**)*wsk;
	}
	*wsk = null;

	return _pierwszy_wolny;
}
template<class T>
void MenadzerPula<T>::niszcz(T* wsk) {
	wsk->~T();
}
template<class T>
T* MenadzerPula<T>::przydziel(size_t rozm_pamiec) {
	assert(_rozm_ob == rozm_pamiec);
	
	if(_pierwszy_wolny == null) {
		dodaj_segment(10000);
	}

	T* wsk = (T*)_pierwszy_wolny;
	_pierwszy_wolny = (void**)*_pierwszy_wolny;
	return wsk;
}
template<class T>
void* MenadzerPula<T>::rownaj_przod(void* const adres, uint32_t const rozmZmienna) const {
	return (void*)(((uint32_t)adres + rozmZmienna) & ~(rozmZmienna-1));
}
template<class T> template<class...A>
void MenadzerPula<T>::tworz(T* wsk, A&&...argumenty) {
	new(wsk) T(forward<A>(argumenty)...);
}
template<class T>
void MenadzerPula<T>::zwolnij(void* wsk, size_t rozm_pamiec) {
	assert(_rozm_ob == rozm_pamiec);

	*((void**)wsk) = _pierwszy_wolny;
	_pierwszy_wolny = (void**)wsk;
}


