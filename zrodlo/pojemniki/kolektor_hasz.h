# pragma once

#include <stdint.h>
#include <windows.h>
#include <operatory.h>
#include <kolektor.h>

template<class T, class H = FunHasz<T>>
class KolektorHasz : public Kolektor<T> {
public:
					KolektorHasz();
					~KolektorHasz();
	void			wstaw_kon(T const&);
	void			haszuj();
private:
	H				_f_hasz;
	uint32_t		_hasz_min;
	uint32_t		_hasz_maks;
	T*				_tab_hasz;
	uint32_t		_il_hasz;
};
template<class T, class H>
KolektorHasz<T,H>::KolektorHasz() : _tab_hasz(0), _il_hasz(0) {
}
template<class T, class H>
KolektorHasz<T,H>::~KolektorHasz() {
	free(_tab_hasz);
}
template<class T, class H>
void KolektorHasz<T,H>::haszuj() {
	free(_tab_hasz);
	uint32_t i, indeks, il_zakres = _hasz_maks-_hasz_min+1;
	T*const tab = (T*)malloc(il_zakres*_rozm_el);
	bool*const maska = (bool*)malloc(il_zakres);
	ZeroMemory(maska, il_zakres);

	// przepisz sortując elementy
	_il_hasz = 0;
	for(i = 0; i < _il; ++i) {
		indeks = _f_hasz(_tab[i])-_hasz_min;
		if(maska[indeks] == false) {
			tab[indeks] = _tab[i];
			maska[indeks] = true;
			++_il_hasz;
		}
	}

	// przepisz usuwając puste komórki
	_tab_hasz = (T*)malloc(_il_hasz*_rozm_el);
	indeks = 0;
	for(i = 0; i < il_zakres; ++i) {
		if(maska[i]) {
			_tab_hasz[indeks++] = tab[i];
		}
	}

	free(tab);
	free(maska);
}
template<class T, class H>
void KolektorHasz<T,H>::wstaw_kon(T const& el) {
	uint32_t hasz_el = _f_hasz(el);
	if(_il == 0) {
		_hasz_min = hasz_el;
		_hasz_maks = hasz_el;
	} else if(hasz_el < _hasz_min) {
		_hasz_min = hasz_el;
	} else if(hasz_el > _hasz_maks) {
		_hasz_maks = hasz_el;
	}
	Kolektor::wstaw_kon(el);
}



