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
	void				dodaj_segment(size_t const);
	inline uint8_t		rownaj_przod(void**const) const;
public:
						MenadzerPula();
						~MenadzerPula();
	inline void			niszcz(T*);
	T*					przydziel(size_t);
						template<class...A>
	void				tworz(T*, A&&...);
	inline void			zwolnij(void*);
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
void MenadzerPula<T>::dodaj_segment(size_t rozm_segment) {
	_pierwszy_wolny = (void**)malloc(rozm_segment);
	_segmenty.push_back(_pierwszy_wolny);
	uint8_t przes = rownaj_przod(_pierwszy_wolny);

	// twórz listę bloków pamięci segmentu
	void** wsk = _pierwszy_wolny;
	for(uint32_t i = 0; i < (rozm_segment-przes)/_rozm_ob; ++i) {
		*wsk = reinterpret_cast<char*>(wsk) + _rozm_ob;
		wsk = (void**)*wsk;
	}
	*wsk = null;
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
uint8_t MenadzerPula<T>::rownaj_przod(void**const adres) const {
	uint8_t przes = ( ((uintptr_t)(*adres) + 3) & ~3 ) - (uintptr_t)(*adres);
	*adres = (void*)((uintptr_t)(*adres) + przes);
	return przes;
}
template<class T> template<class...A>
void MenadzerPula<T>::tworz(T* wsk, A&&...argumenty) {
	new(wsk) T(forward<A>(argumenty)...);
}
template<class T>
void MenadzerPula<T>::zwolnij(void* wsk) {
	*((void**)wsk) = _pierwszy_wolny;
	_pierwszy_wolny = (void**)wsk;
}


