#pragma once

#include <stdint.h>
#include <assert.h>
#include <vector>

using std::vector;

template<class T>
class MenadzerPula {
	#define null 0
private:
	size_t				_il_wolne;
	void**				_pierwszy_wolny;
	size_t const		_rozm_ob;
	vector<void*>		_segmenty;
	void*				dodaj_segment(size_t const);
	inline void*		rownajPrzod(void* const, uint32_t const) const;
public:
						MenadzerPula();
	void*				allocate(size_t);
};
template<class T>
MenadzerPula<T>::MenadzerPula() : _il_wolne(0), _rozm_ob(sizeof(T)) {
	assert(_rozm_ob >= sizeof(void*));
	dodaj_segment(10000);
}
template<class T>
void* MenadzerPula<T>::allocate(size_t rozm_pamiec) {

}
template<class T>
void* MenadzerPula<T>::dodaj_segment(size_t rozm_segment) {
	_pierwszy_wolny = (void**)malloc(rozm_segment);
	_segmenty.push_back(_pierwszy_wolny);
	_il_wolne += rozm_segment;

	// twórz listę bloków pamięci segmentu
	void** wsk = _pierwszy_wolny;
	for(size_t i = 0; i < rozm_segment/_rozm_ob; ++i) {
		*wsk = reinterpret_cast<char*>(wsk) + _rozm_ob;
		wsk = (void**)*wsk;
	}
	*wsk = null;

	return _pierwszy_wolny;
}
template<class T>
void* MenadzerPula<T>::rownajPrzod(void* const adres, uint32_t const rozmZmienna) const {
	return (void*)(((uint32_t)adres + rozmZmienna) & ~(rozmZmienna-1));
}


