# pragma once

#include <stdint.h>

template<class T, class H = FunHasz<T>>
class Wektor {
public:
						Wektor();
						~Wektor();
	inline T&			operator[](uint32_t const&);
	void				rezerwuj(uint32_t const&);
	inline void			wstaw_kon(T const&);
	void				uloz();
	void				uloz_unikat();
	inline uint32_t		wez_il() const;
	inline void			czysc();
protected:
	uint8_t				_rozm_el;
	H					_f_hasz;
	uint32_t			_hasz_min;
	uint32_t			_hasz_maks;
	uint32_t			_il_rezerw;
	uint32_t			_il;
	T*					_tab;
};
template<class T, class H>
Wektor<T,H>::Wektor() : _rozm_el(sizeof(T)), _il_rezerw(4096), _il(0), _tab((T*)malloc(_il_rezerw*_rozm_el)) {
}
template<class T, class H>
Wektor<T,H>::~Wektor() {
	free(_tab);
}
template<class T, class H>
T& Wektor<T,H>::operator[](uint32_t const& indeks) {
	return _tab[indeks];
}
template<class T, class H>
void Wektor<T,H>::czysc() {
	_il = 0;
}
template<class T, class H>
void Wektor<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;
	T*const pam = (T*)malloc(il*_rozm_el);
	memmove(pam, _tab, _il_rezerw*_rozm_el);
	free(_tab);
	_tab = pam;
	_il_rezerw = il;
}
template<class T, class H>
void Wektor<T,H>::uloz() {
	uint32_t il, ind1_pocz, ind1_kon, ind2_pocz, ind2_kon, il_gr = 1;
	T* t = (T*)malloc(_il*_rozm_el);
	while(il_gr < _il) {
		il = 0; ind1_pocz = 0; ind1_kon = 0; ind2_pocz = 0, ind2_kon = 0;
		while(il < _il) {
			ind1_pocz = ind2_kon;
			ind1_kon = ind1_pocz + il_gr;
			ind2_pocz = ind1_kon;
			ind2_kon = ind2_pocz + il_gr;

			// gdy grup-a/y przekracz-a/ają ilość wszystkich elementów
			if(ind1_kon > _il) {
				ind1_kon = _il;
				ind2_kon = _il;
			} else if(ind2_kon > _il) {
				ind2_kon = _il;
			}

			// układaj elementy grup
			while(ind1_pocz < ind1_kon && ind2_pocz < ind2_kon) {
				if(_f_hasz(_tab[ind1_pocz]) <= _f_hasz(_tab[ind2_pocz])) {
					t[il++] = _tab[ind1_pocz];
					++ind1_pocz;
				} else {
					t[il++] = _tab[ind2_pocz];
					++ind2_pocz;
				}
			}
			while(ind1_pocz < ind1_kon) {
				t[il++] = _tab[ind1_pocz];
				++ind1_pocz;
			}
			while(ind2_pocz < ind2_kon) {
				t[il++] = _tab[ind2_pocz];
				++ind2_pocz;
			}
		}
		memmove(_tab, t, il*_rozm_el);
		il_gr *= 2;
	}
}
template<class T, class H>
void Wektor<T,H>::uloz_unikat() {
	uint32_t i, il, indeks, il_zakres = _hasz_maks-_hasz_min+1;
	T*const tab = (T*)malloc(il_zakres*_rozm_el);
	bool*const maska = (bool*)malloc(il_zakres);
	ZeroMemory(maska, il_zakres);

	// przepisz sortując / usuwając podwójne elementy
	il = 0;
	for(i = 0; i < _il; ++i) {
		indeks = _f_hasz(_tab[i])-_hasz_min;
		if(maska[indeks] == false) {
			tab[indeks] = _tab[i];
			maska[indeks] = true;
			++il;
		}
	}

	// przepisz usuwając puste komórki
	_il = 0;
	for(i = 0; i < il_zakres; ++i) {
		if(maska[i]) {
			_tab[_il++] = tab[i];
		}
	}

	free(tab);
	free(maska);
}
template<class T, class H>
uint32_t Wektor<T,H>::wez_il() const {
	return _il;
}
template<class T, class H>
void Wektor<T,H>::wstaw_kon(T const& el) {
	uint32_t hasz_el = _f_hasz(el);
	if(_il == 0) {
		_hasz_min = hasz_el;
		_hasz_maks = hasz_el;
	} else if(hasz_el < _hasz_min) {
		_hasz_min = hasz_el;
	} else if(hasz_el > _hasz_maks) {
		_hasz_maks = hasz_el;
	}
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*4);
	}
	_tab[_il++] = el;
}



