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
	inline void					wstaw_kon(T const&, uint32_t const& = 1);
	inline void					usun(uint32_t const&, uint32_t const& = 1);
	inline void					usun_kon(uint32_t const& = 1);
	inline void					zamien(uint32_t const&, uint32_t const&);
	inline uint32_t const&		wez_il() const;
	inline uint32_t const&		wez_pierw_pusty() const;
	void						defrag_licz(uint32_t*&, uint32_t const&);
	void						defrag_wyk(uint32_t const*const&);
	void						uloz_licz(uint32_t*&) const;
	void						uloz_unikat_licz(uint32_t*&) const;
	void						uloz_wyk(uint32_t const*const&);
	static T const				pusty;
protected:
	void						licz_zakres(uint32_t&, uint32_t&) const;
	T*							el;
	uint32_t					il;
	uint32_t					poj;
	uint32_t					pierw_pusty;
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
void Wektor<T>::defrag_licz(uint32_t*& _mapa, uint32_t const& _il) {
	if(il < 2 || pierw_pusty == 0x80000000) return;
	free(_mapa);
	_mapa = (uint32_t*)malloc(il*4);
	memset(_mapa, 0xffffffff, il*4);
	uint32_t _n = 0, _ind_pelny = pierw_pusty;

	while(_n < _il && _ind_pelny < il-1) {
		if(el[++_ind_pelny] == pusty) continue;
		_mapa[_ind_pelny] = pierw_pusty++;
		++_n;
	}
}
template<class T>
void Wektor<T>::defrag_wyk(uint32_t const*const& _mapa) {
	if(pierw_pusty > il) return;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(_mapa[_i] == 0xffffffff) continue;
		zamien(_i, _mapa[_i]);
	}
	for(int32_t _i = il-1; _i >= 0; --_i) {
		if(el[_i] != pusty) break;
		--il;
	}
	if(pierw_pusty >= il) pierw_pusty = 0x80000000;
}
template<class T>
void Wektor<T>::licz_zakres(uint32_t& _hasz_min, uint32_t& _hasz_maks) const {
	_hasz_min = 0x80000000;
	_hasz_maks = 0;
	uint32_t _hasz;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) continue;
		_hasz = wez_hasz(el[_i]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		if(_hasz > _hasz_maks) _hasz_maks = _hasz;
	}
	if(_hasz_min == 0x80000000) _hasz_min = 1;
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
void Wektor<T>::uloz_licz(uint32_t*& _mapa) const {
	if(il < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze elementów i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(il*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = wez_hasz(el[_i]) - _hasz_min; // tablica hasze elementów
			++_t2[_t1[_i]]; // tablica powtórzenia haszy
		}
	}

	// licz indeksy haszy
	uint32_t*const _t3 = (uint32_t*const)malloc(_hasz_il*4);
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		_t3[_i] = _ind; // tablica indeksy haszy
		_ind += _t2[_i];
	}

	// licz indeksy elementów
	memset(_t2, 0, _hasz_il*4); // tablica ilość wpisanych elementów haszy
	uint32_t _hasz;
	for(_i = 0; _i < il; ++_i) {
		_hasz = _t1[_i];
		if(_hasz == 0x80000000) continue;
		_t1[_i] = _t3[_hasz] + _t2[_hasz]; // tablica indeksy elementów
		++_t2[_hasz];
	}

	free(_mapa);
	free(_t2);
	free(_t3);
	_mapa = _t1;
}
template<class T>
void Wektor<T>::uloz_unikat_licz(uint32_t*& _mapa) const {
	if(il < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze elementów i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(il*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = wez_hasz(el[_i]) - _hasz_min; // tablica hasze elementów
			if(_t2[_t1[_i]] == 0) ++_t2[_t1[_i]]; // tablica powtórzenia haszy
			else _t1[_i] = 0x80000000;
		}
	}

	// licz indeksy haszy
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		if(_t2[_i] == 0) continue;
		_t2[_i] = _ind; // tablica indeksy haszy
		++_ind;
	}

	// licz indeksy elementów
	for(_i = 0; _i < il; ++_i) {
		if(_t1[_i] == 0x80000000) continue;
		_t1[_i] = _t2[_t1[_i]]; // tablica indeksy elementów
	}

	free(_mapa);
	free(_t2);
	_mapa = _t1;
}
template<class T>
void Wektor<T>::uloz_wyk(uint32_t const*const& _mapa) {
	T*const _el = (T*const)malloc(poj*sizeof(T));
	uint32_t _il = 0;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		_el[_mapa[_i]] = el[_i];
		++_il;
	}
	free(el);
	el = _el;
	il = _il;
}
template<class T>
void Wektor<T>::usun(uint32_t const& _nr, uint32_t const& _il) {
	wyp_pam(&el[_nr], pusty, _il);
	if(_nr < pierw_pusty) pierw_pusty = _nr;
}
template<class T>
void Wektor<T>::usun_kon(uint32_t const& _il) {
	if(il <= _il) il = 0;
	else il -= _il;
}
template<class T>
uint32_t const& Wektor<T>::wez_il() const {
	return il;
}
template<class T>
uint32_t const& Wektor<T>::wez_pierw_pusty() const {
	return pierw_pusty;
}
template<class T>
void Wektor<T>::wstaw_kon(T const& _el, uint32_t const& _il) {
	while(il+_il > poj) rezerw_tyl(poj*2);
	memmove(&el[il], &_el, _il*sizeof(T));
	il += _il;
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
	inline bool		operator==(Paraa const&) const;
	inline bool		operator!=(Paraa const&) const;
	A				pierw;
	B				drug;
};
template<class A, class B>
bool Paraa<A,B>::operator==(Paraa const& _para) const {
	return (pierw == _para.pierw && drug == _para.drug);
}
template<class A, class B>
bool Paraa<A,B>::operator!=(Paraa const& _para) const {
	return !operator==(_para);
}
// -------------------------------------------------------
template<class A, class B>
std::ostream& operator<<(std::ostream& _strumien, Paraa<A,B> const& _para) {
	_strumien << "{" << _para.pierw << ", " << _para.drug << "}";
	return _strumien;
}
// -------------------------------------------------------
template<class T>
class Wektor2 {
public:
	typedef Paraa<uint32_t> Wiersz_;
	typedef Paraa<T const*,uint32_t> WierszEl_;
	inline T*const&				operator[](uint32_t const&) const;
	inline void					wstaw_kon(T const*const&, uint32_t const&);
	inline void					usun(uint32_t const&, uint32_t = 1);
	inline void					usun_kon(uint32_t = 1);
	inline Wiersz_ const&		wez_wier(uint32_t const&) const;
	inline uint32_t const&		wez_il() const;
	void						defrag_licz(uint32_t*&, uint32_t*&, uint32_t const&);
	void						defrag_wyk(uint32_t const*const&,
									uint32_t const*const&);
	void						uloz_licz(uint32_t*&) const;
	void						uloz_unikat_licz(uint32_t*&) const;
	void						uloz_wyk(uint32_t const*const&);
	static Wiersz_ const		pusty;
protected:
	void						licz_zakres(uint32_t&, uint32_t&) const;
	Wektor<Wiersz_>				wier;
	Wektor<T>					el;
};
template<class T>
T*const& Wektor2<T>::operator[](uint32_t const& _nr) const {
	return &el[wier[_nr].pierw];
}
template<class T>
void Wektor2<T>::defrag_licz(uint32_t*& _mapa_wier, uint32_t*& _mapa_el, uint32_t const& _il_wier) {
	// licz ile elementów odpowiada podanej ilości wierszy
	uint32_t _il_el = 0, _licz = _il_wier;
	for(uint32_t _ind = wier.wez_pierw_pusty();
		_ind < wier.wez_il() && _licz > 0;
		++_ind) {
		if(wier[_ind] == wier.pusty) continue;
		_il_el += wier[_ind].drug;
		--_licz;
	}

	wier.defrag_licz(_mapa_wier, _il_wier);
	el.defrag_licz(_mapa_el, _il_el);
}
template<class T>
void Wektor2<T>::defrag_wyk(uint32_t const*const& _mapa_wier, uint32_t const*const& _mapa_el) {
	wier.defrag_wyk(_mapa_wier);
	el.defrag_wyk(_mapa_el);
}
template<class T>
void Wektor2<T>::licz_zakres(uint32_t& _hasz_min, uint32_t& _hasz_maks) const {
	_hasz_min = 0x80000000;
	_hasz_maks = 0;
	uint32_t _hasz;
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) continue;
		_hasz = wez_hasz(el[wier[_i].pierw]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		if(_hasz > _hasz_maks) _hasz_maks = _hasz;
	}
	if(_hasz_min == 0x80000000) _hasz_min = 1;
}
template<class T>
void Wektor2<T>::uloz_licz(uint32_t*& _mapa_wier) const {
	if(wier.wez_il() < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze wierszy i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(wier.wez_il()*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = wez_hasz(el[wier[_i].pierw]) - _hasz_min; // tablica hasze wierszy
			++_t2[_t1[_i]]; // tablica powtórzenia haszy
		}
	}

	// licz indeksy haszy
	uint32_t*const _t3 = (uint32_t*const)malloc(_hasz_il*4);
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		_t3[_i] = _ind; // tablica indeksy haszy
		_ind += _t2[_i];
	}

	// licz indeksy wierszy
	memset(_t2, 0, _hasz_il*4); // tablica ilość wpisanych wierszy haszy
	uint32_t _hasz;
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		_hasz = _t1[_i];
		if(_hasz == 0x80000000) continue;
		_t1[_i] = _t3[_hasz] + _t2[_hasz]; // tablica indeksy wierszy
		++_t2[_hasz];
	}

	free(_mapa_wier);
	free(_t2);
	free(_t3);
	_mapa_wier = _t1;
}
template<class T>
void Wektor2<T>::uloz_unikat_licz(uint32_t*& _mapa_wier) const {
	if(wier.wez_il() < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze wierszy i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(wier.wez_il()*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = wez_hasz(el[wier[_i].pierw]) - _hasz_min; // tablica hasze wierszy
			if(_t2[_t1[_i]] == 0) ++_t2[_t1[_i]]; // tablica powtórzenia haszy
			else _t1[_i] = 0x80000000;
		}
	}

	// licz indeksy haszy
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		if(_t2[_i] == 0) continue;
		_t2[_i] = _ind; // tablica indeksy haszy
		++_ind;
	}

	// licz indeksy wierszy
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		if(_t1[_i] == 0x80000000) continue;
		_t1[_i] = _t2[_t1[_i]]; // tablica indeksy wierszy
	}

	free(_mapa_wier);
	free(_t2);
	_mapa_wier = _t1;
}
template<class T>
void Wektor2<T>::uloz_wyk(uint32_t const*const& _mapa_wier) {
	wier.uloz_wyk(_mapa_wier);

	// twórz mapę elementów, aktualizuj wiersze
	uint32_t*const _mapa_el = (uint32_t*const)malloc(el.wez_il()*4);
	wyp_pam(_mapa_el, 0x80000000, el.wez_il());
	uint32_t _i, _j, _ind = 0;
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		for(_j = wier[_i].pierw; _j < wier[_i].pierw + wier[_i].drug; ++_j) {
			_mapa_el[_j] = _ind++;
		}
		wier[_i].pierw = _ind - wier[_i].drug;
	}

	el.uloz_wyk(_mapa_el);
}
template<class T>
void Wektor2<T>::usun(uint32_t const& _nr, uint32_t _il) {
	for(uint32_t _i = 0; _i < _il; ++_i) {
		el.usun(wier[_nr+_i].pierw, wier[_nr+_i].drug);
		wier.usun(_nr+_i);
	}
}
template<class T>
void Wektor2<T>::usun_kon(uint32_t _il) {
	if(_il > wier.wez_il()) _il = wier.wez_il();
	for(uint32_t _i = 0; _i < _il; ++_i) {
		el.usun_kon(wier[wier.wez_il()-1].drug);
		wier.usun_kon();
	}
}
template<class T>
uint32_t const& Wektor2<T>::wez_il() const {
	return wier.wez_il();
}
template<class T>
typename Wektor2<T>::Wiersz_ const& Wektor2<T>::wez_wier(uint32_t const& _nr) const {
	return wier[_nr];
}
template<class T>
void Wektor2<T>::wstaw_kon(T const*const& _el, uint32_t const& _il) {
	if(_il == 0) return;
	wier.wstaw_kon({el.wez_il(), _il});
	el.wstaw_kon(*_el, _il);
}
template<class T>
typename Wektor2<T>::Wiersz_ const Wektor2<T>::pusty = gen_min<Wiersz_>();
// -------------------------------------------------------



