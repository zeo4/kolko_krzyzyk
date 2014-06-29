#pragma once

#include <utility>
#include <list>

using std::forward;
using std::list;

template<class T>
class PamiecPula {
private:
	#define null 0
	void**				_pierwszy_wolny;
	size_t const		_rozm_ob;
	list<void*>			_segmenty;
	void				dodaj_segment(size_t const);
	inline uint8_t		rownaj_przod(void**const) const;
public:
						PamiecPula();
						~PamiecPula();
	inline void			niszcz(T*);
	T*					przydziel();
						template<class...A>
	void				tworz(T*, A&&...);
	inline void			zwolnij(void*);
};
template<class T>
PamiecPula<T>::PamiecPula() : _rozm_ob(sizeof(T)) {
	assert(_rozm_ob >= sizeof(void*));
	dodaj_segment(100000);
}
template<class T>
PamiecPula<T>::~PamiecPula() {
	list<void*>::const_iterator it;
	for(it = _segmenty.begin(); it != _segmenty.end(); ++it) {
		free(*it);
	}
	_pierwszy_wolny = null;
}
template<class T>
void PamiecPula<T>::dodaj_segment(size_t rozm_segment) {
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
void PamiecPula<T>::niszcz(T* wsk) {
	wsk->~T();
}
template<class T>
T* PamiecPula<T>::przydziel() {
	if(_pierwszy_wolny == null) {
		dodaj_segment(100000);
	}

	T* wsk = (T*)_pierwszy_wolny;
	_pierwszy_wolny = (void**)*_pierwszy_wolny;
	return wsk;
}
template<class T>
uint8_t PamiecPula<T>::rownaj_przod(void**const adres) const {
	uint8_t przes = ( ((uintptr_t)(*adres) + 3) & ~3 ) - (uintptr_t)(*adres);
	*adres = (void*)((uintptr_t)(*adres) + przes);
	return przes;
}
template<class T> template<class...A>
void PamiecPula<T>::tworz(T* wsk, A&&...argumenty) {
	new(wsk) T(forward<A>(argumenty)...);
}
template<class T>
void PamiecPula<T>::zwolnij(void* wsk) {
	*((void**)wsk) = _pierwszy_wolny;
	_pierwszy_wolny = (void**)wsk;
}


