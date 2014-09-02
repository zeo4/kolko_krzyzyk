#pragma once

#include <stdint.h>
#include <operatory.h>

// -------------------------------------------------------
template<class T>
class Wektor {
public:
								Wektor();
								~Wektor();
	inline T&					operator[](uint32_t const&) const;
	void						rezerw_tyl(uint32_t const&);
	void						rezerw_przod(uint32_t const&);
	inline void					wstaw_kon(T const&);
	inline void					usun(uint32_t const&);
	inline void					usun_kon();
	inline void					zamien(uint32_t const&, uint32_t const&);
	inline uint32_t const&		wez_il() const;
	inline bool					sprawdz_pusty(T const&) const;
	void						defrag(uint32_t const&);
	uint32_t const*				licz_uloz() const;
	uint32_t const*				licz_uloz_unikat() const;
	void						uloz(uint32_t const*const&);
protected:
	void						licz_zakres(uint32_t&, uint32_t&) const;
	T*							el;
	uint32_t					il;
	uint32_t					poj;
	uint32_t					pierw_pusty;
	static T const				pusty;
};
template<class T>
Wektor<T>::Wektor()
	: el((T*)malloc(256*sizeof(T))),
	il(0),
	poj(256),
	pierw_pusty(gen_min<uint32_t>()) {}
template<class T>
Wektor<T>::~Wektor() {
	free(el);
}
template<class T>
T& Wektor<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
void Wektor<T>::defrag(uint32_t const& _il) {
	if(il < 2 || pierw_pusty > il) return;
	uint32_t _n = 0, _ind_pelny = pierw_pusty;
	while(_n < _il && _ind_pelny < il-1) {
		if(sprawdz_pusty(el[++_ind_pelny])) continue;
		zamien(pierw_pusty++, _ind_pelny);
		++_n;
	}
	if(_ind_pelny == il-1) {
		il = pierw_pusty;
		pierw_pusty = gen_min<uint32_t>();
	}
}
template<class T>
uint32_t const* Wektor<T>::licz_uloz() const {
	if(il < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze elementów i powtórzenia haszy
	uint32_t*const _mapa = (uint32_t*const)malloc(il*4);
	uint32_t*const _hasz_powt = (uint32_t*const)malloc(_hasz_il*4);
	memset(_hasz_powt, 0, _hasz_il*4);
	for(_i = 0; _i < il; ++_i) {
		_mapa[_i] = wez_hasz(el[_i]) - _hasz_min;
		++_hasz_powt[_mapa[_i]];
	}

	// licz indeksy haszy
	uint32_t*const _hasz_ind = (uint32_t*const)malloc(_hasz_il*4);
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		_hasz_ind[_i] = _ind;
		_ind += _hasz_powt[_i];
	}

	// licz indeksy elementów
	memset(_hasz_powt, 0, _hasz_il*4);
	uint32_t _hasz;
	for(_i = 0; _i < il; ++_i) {
		_hasz = _mapa[_i];
		_mapa[_i] = _hasz_ind[_hasz] + _hasz_powt[_hasz];
		++_hasz_powt[_hasz];
	}

	return _mapa;
}
template<class T>
uint32_t const* Wektor<T>::licz_uloz_unikat() const {
	if(il < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz / oznacz hasze elementów, nie-hasze to -1
	uint32_t*const _mapa = (uint32_t*const)malloc(il*4);
	uint32_t*const _hasz_ind = (uint32_t*const)malloc(_hasz_il*4);
	memset(_hasz_ind, -1, _hasz_il*4);
	for(_i = 0; _i < il; ++_i) {
		_mapa[_i] = wez_hasz(el[_i]) - _hasz_min;
		_hasz_ind[_mapa[_i]] = 1;
	}

	// licz indeksy haszy
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		if(_hasz_ind[_i] == -1) continue;
		_hasz_ind[_i] = _ind++;
	}

	// licz indeksy elementów
	for(_i = 0; _i < il; ++_i) {
		_mapa[_i] = _hasz_ind[_mapa[_i]];
	}

	return _mapa;
}
template<class T>
void Wektor<T>::licz_zakres(uint32_t& _hasz_min, uint32_t& _hasz_maks) const {
	_hasz_min = wez_hasz(el[0]);
	_hasz_maks = _hasz_min;
	uint32_t _hasz;
	for(uint32_t _i = 1; _i < il; ++_i) {
		_hasz = wez_hasz(el[_i]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		else if(_hasz > _hasz_maks) _hasz_maks = _hasz;
	}
}
template<class T>
void Wektor<T>::rezerw_przod(uint32_t const& _poj) {
	if(_poj <= poj) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el+_poj-poj, el, poj*sizeof(T));
	free(el);
	el = _el;
	il += _poj - poj;
	poj = _poj;
}
template<class T>
void Wektor<T>::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= poj) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el, el, poj*sizeof(T));
	free(el);
	el = _el;
	poj = _poj;
}
template<class T>
bool Wektor<T>::sprawdz_pusty(T const& _el) const {
	return !memcmp(&_el, &pusty, sizeof(T));
}
template<class T>
void Wektor<T>::uloz(uint32_t const*const& _mapa) {
	T*const _el = (T*const)malloc(poj*sizeof(T));
	wyp_pam(_el, pusty, il);
	uint32_t _il = 0;
	for(int32_t _i = il-1; _i > -1; --_i) {
		if(memcmp(&_el[_mapa[_i]], &pusty, sizeof(T)) == 0) ++_il;
		_el[_mapa[_i]] = el[_i];
	}
	free(el);
	el = _el;
	il = _il;
}
template<class T>
void Wektor<T>::usun(uint32_t const& _nr) {
	el[_nr] = pusty;
	if(_nr < pierw_pusty) pierw_pusty = _nr;
}
template<class T>
void Wektor<T>::usun_kon() {
	if(il == 0) return;
	--il;
}
template<class T>
uint32_t const& Wektor<T>::wez_il() const {
	return il;
}
template<class T>
void Wektor<T>::wstaw_kon(T const& _el) {
	if(il == poj) rezerw_tyl(poj*2);
	el[il++] = _el;
}
template<class T>
void Wektor<T>::zamien(uint32_t const& _nr1, uint32_t const& _nr2) {
	T _el = el[_nr1];
	el[_nr1] = el[_nr2];
	el[_nr2] = _el;
}
template<class T>
T const Wektor<T>::pusty = gen_min<T>();
// -------------------------------------------------------
template<class A, class B = A>
struct Paraa {
	A		pierw;
	B		drug;
};
// -------------------------------------------------------
template<class T>
class WektorSeg {
public:
protected:

};


