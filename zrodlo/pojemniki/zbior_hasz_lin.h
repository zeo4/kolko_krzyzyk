﻿#pragma once

#include <stdint.h>
#include <operatory.h>
#include <pamiec_lin.h>

template<class T, class FH = FunHasz<T>>
class ZbiorHaszLin {
public:
						ZbiorHaszLin();
	uint32_t			wez_il() const;
	inline void			wstaw_kon(T const&);
	void				haszuj();
	void				czysc();
private:
	ListaLin<T>			_lista;
	T*					_tab_hasz;
	uint32_t			_min;
	uint32_t			_maks;
	uint32_t			_il;
	short const			_rozm_ob;
	FH					_f_hasz;
};
template<class T, class FH>
ZbiorHaszLin<T,FH>::ZbiorHaszLin() : _tab_hasz(0), _rozm_ob(sizeof(T)), _il(0) {
}
template<class T, class FH>
void ZbiorHaszLin<T,FH>::czysc() {
	_lista.czysc();
	delete[] _tab_hasz;
	_il = 0;
}
template<class T, class FH>
void ZbiorHaszLin<T,FH>::haszuj() {
	if(_tab_hasz != 0) {
		delete[] _tab_hasz;
	}
	uint32_t indeks;
	T*const tab = new T[_maks - _min + 1];
	bool*const maska = new bool[_maks - _min + 1];
	for(uint32_t i = 0; i < _maks-_min+1; ++i) {
		maska[i] = false;
	}

	// przepisz sortując wpisy
	ListaLin<T>::Iter_ it;
	_il = 0;
	for(it = _lista.wez_pocz(); it != _lista.wez_kon(); ++it) {
		indeks = _f_hasz(it->wart)-_min;
		if(maska[indeks] == false) {
			tab[indeks] = it->wart;
			maska[indeks] = true;
			++_il;
		}
	}

	// przepisz usuwając puste komórki
	_tab_hasz = new T[_il];
	indeks = 0;
	for(uint32_t i = 0; i < _maks-_min+1; ++i) {
		if(maska[i]) {
			_tab_hasz[indeks++] = tab[i];
		}
	}

	delete[] tab;
	delete[] maska;
}
template<class T, class FH>
uint32_t ZbiorHaszLin<T,FH>::wez_il() const {
	return _il;
}
template<class T, class FH>
void ZbiorHaszLin<T,FH>::wstaw_kon(T const& wart) {
	if(_il == 0) {
		_min = _f_hasz(wart);
		_maks = _f_hasz(wart);
	} else if(_f_hasz(wart) < _min) {
		_min = _f_hasz(wart);
	} else if(_f_hasz(wart) > _maks) {
		_maks = _f_hasz(wart);
	}
	_lista.wstaw_kon(wart);
	++_il;
}


