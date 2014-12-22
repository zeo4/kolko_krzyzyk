#pragma once

#include <stdint.h>
#include <operatory.h>
#include <lista_lin.h>
#include <pamiec_lin.h>

template<class T, class FH = FunHasz<T>>
class WielozbiorHaszLin {
public:
						WielozbiorHaszLin();
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
WielozbiorHaszLin<T,FH>::WielozbiorHaszLin() : _tab_hasz(0), _rozm_ob(sizeof(T)), _il(0) {
}
template<class T, class FH>
void WielozbiorHaszLin<T,FH>::czysc() {
	_lista.czysc();
	delete[] _tab_hasz;
	_tab_hasz = 0;
	_il = 0;
}
template<class T, class FH>
void WielozbiorHaszLin<T,FH>::haszuj() {
	if(_tab_hasz != 0) {
		delete[] _tab_hasz;
	}
	uint32_t indeks, rozm_zakres = _maks - _min + 1;
	T*const tab = new T[rozm_zakres];
	uint32_t*const maska = new uint32_t[rozm_zakres];
	for(uint32_t i = 0; i < rozm_zakres; ++i) {
		maska[i] = 0;
	}

	// przepisz sortując wpisy
	ListaLin<T>::Iter_ it;
	_il = 0;
	for(it = _lista.wez_pocz(); it != _lista.wez_kon(); ++it) {
		indeks = _f_hasz(it->wart)-_min;
		tab[indeks] = it->wart;
		++(maska[indeks]);
		++_il;
	}

	// przepisz powielając powtórzone
	_tab_hasz = new T[_il];
	indeks = 0;
	int j;
	for(uint32_t i = 0; i < rozm_zakres; ++i) {
		for(j = 0; j < maska[i]; ++j) {
			_tab_hasz[indeks++] = tab[i];
		}
	}

	delete[] tab;
	delete[] maska;
}
template<class T, class FH>
uint32_t WielozbiorHaszLin<T,FH>::wez_il() const {
	return _il;
}
template<class T, class FH>
void WielozbiorHaszLin<T,FH>::wstaw_kon(T const& wart) {
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


