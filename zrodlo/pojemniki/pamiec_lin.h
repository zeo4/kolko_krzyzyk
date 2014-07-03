#pragma once

#include <stdint.h>

template<class T>
class PamiecLin {
public:
					PamiecLin();
					~PamiecLin();
	inline T*		przydziel();
	void			czysc();
private:
	void			inicjalizuj();
	void			dodaj_segment();
	short const		_rozm_el;
	T** const		_segmenty;
	T**				_segment_akt;
	uint32_t		_rozm_segment_akt;
	T*				_pierwszy_wolny;
};
template<class T>
PamiecLin<T>::PamiecLin() : _rozm_el(sizeof(T)), _segmenty((T**)malloc(256*_rozm_el)) {
	inicjalizuj();
}
template<class T>
PamiecLin<T>::~PamiecLin() {
	for(int i = 0; i < 256; ++i) {
		free(_segmenty[i]);
		if(_segmenty+i == _segment_akt) {
			free(_segmenty);
			break;
		}
	}
}
template<class T>
void PamiecLin<T>::czysc() {
	while(1) {
		free(*_segment_akt);
		if(_segment_akt == _segmenty) break; else --_segment_akt;
	}
	inicjalizuj();
}
template<class T>
void PamiecLin<T>::dodaj_segment() {
	if(_rozm_segment_akt < 32,768) {
		_rozm_segment_akt *= 2;
	} else {
		_rozm_segment_akt = 65,535;
	}
	_pierwszy_wolny = (T*)malloc(_rozm_segment_akt*_rozm_el);
	*++_segment_akt = _pierwszy_wolny;
}
template<class T>
void PamiecLin<T>::inicjalizuj() {
	_segment_akt = _segmenty;
	_rozm_segment_akt = 1000;
	_pierwszy_wolny = (T*)malloc(_rozm_segment_akt*_rozm_el);
	*_segmenty = _pierwszy_wolny;
}
template<class T>
T* PamiecLin<T>::przydziel() {
	if(_pierwszy_wolny == (*_segment_akt)+_rozm_segment_akt) {
		dodaj_segment();
	}
	return _pierwszy_wolny++;
}




