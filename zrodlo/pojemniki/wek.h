#pragma once

#include <stdint.h>
#include <operatory.h>
// -------------------------------------------------------
template<class T>
class Wek {
public:
								Wek();
								Wek(T*const);
								Wek(T*const,
									uint32_t const&, uint32_t const&, uint32_t const&
								);
	inline T&					operator[](uint32_t const&) const;
	void						rezerw_tyl(uint32_t const&);
	void						rezerw_przod(uint32_t const&);
	inline void					niszcz();
	inline void					wstaw_kon(T const&, uint32_t const& = 1);
	inline void					usun(uint32_t const&, uint32_t const& = 1);
	template<class H = FunHasz<T>>
	void						usun_dupl_licz(uint32_t*&, H = H()) const;
	inline void					usun_kon(uint32_t const& = 1);
	inline void					zamien(uint32_t const&, uint32_t const&);
	inline uint32_t const&		wez_il() const;
	inline uint32_t const&		wez_pierw_pusty() const;
	void						defrag_licz(uint32_t*&, uint32_t const&);
	void						defrag_wyk(uint32_t const*const);
	template<class H = FunHasz<T>>
	void						uloz_licz(uint32_t*&, H = H()) const;
	template<class H = FunHasz<T>>
	void						uloz_uni_licz(uint32_t*&, H = H()) const;
	void						uloz_wyk(uint32_t const*const);
	void						aktual(uint32_t const*const);
	inline void					aktual_nagl();
	static T const				pusty;
protected:
	template<class H>
	void						licz_zakres(uint32_t&, uint32_t&, H) const;
	uint32_t					poj;
	uint32_t					il;
	uint32_t					pierw_pusty;
	T*							el;
};
template<class T>
Wek<T>::Wek()
	: el((T*)((uint8_t*)malloc(256 * sizeof(T) + 12) + 12)),
	poj(256),
	il(0),
	pierw_pusty(0x80000000) {}
template<class T>
Wek<T>::Wek(T*const _pam) {
	memcpy(this, _pam, 12);
	el = (T*)((uint8_t*)_pam + 12);
}
template<class T>
Wek<T>::Wek(T*const _el, uint32_t const& _poj, uint32_t const& _il, uint32_t const& _pierw_pusty)
	: el(_el),
	poj(_poj),
	il(_il),
	pierw_pusty(_pierw_pusty) {}
template<class T>
T& Wek<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
void Wek<T>::aktual(uint32_t const*const _mapa) {
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) continue;
		if(_mapa[el[_i]] == 0x80000000) continue;
		el[_i] = _mapa[el[_i]];
	}
}
template<class T>
void Wek<T>::aktual_nagl() {
	memcpy((uint8_t*)el - 12, this, 12);
}
template<class T>
void Wek<T>::defrag_licz(uint32_t*& _mapa, uint32_t const& _il) {
	free(_mapa);
	_mapa = (uint32_t*)malloc(il*4);
	wyp_pam(_mapa, 0x80000000, il);

	if(il < 2 || pierw_pusty == 0x80000000) return;

	uint32_t _n = 0, _pierw_pusty = pierw_pusty, _ind_pelny = _pierw_pusty;
	while(_n < _il && _ind_pelny < il-1) {
		if(memcmp(&el[++_ind_pelny], &pusty, sizeof(T)) == 0) continue;
		_mapa[_ind_pelny] = _pierw_pusty++;
		++_n;
	}
}
template<class T>
void Wek<T>::defrag_wyk(uint32_t const*const _mapa) {
	if(pierw_pusty == 0x80000000) return;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		zamien(_i, _mapa[_i]);
		pierw_pusty++;
	}
	for(int32_t _i = il-1; _i >= 0; --_i) {
		if(memcmp(&el[_i], &pusty, sizeof(T)) != 0) break;
		--il;
	}
	if(pierw_pusty >= il) pierw_pusty = 0x80000000;
}
template<class T> template<class H>
void Wek<T>::licz_zakres(uint32_t& _hasz_min, uint32_t& _hasz_maks, H _licz_hasz) const {
	_hasz_min = 0x80000000;
	_hasz_maks = 0;
	uint32_t _hasz;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) continue;
		_hasz = _licz_hasz(el[_i]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		if(_hasz > _hasz_maks) _hasz_maks = _hasz;
	}
	if(_hasz_min == 0x80000000) _hasz_min = 1;
}
template<class T>
void Wek<T>::niszcz() {
	free((uint8_t*)el - 12);
}
template<class T>
void Wek<T>::rezerw_przod(uint32_t const& _poj) {
	if(_poj <= poj) return;

	T*const _el = (T*const)((uint8_t*)malloc(_poj * sizeof(T) + 12) + 12);
	memcpy(_el+_poj-poj, el, poj * sizeof(T));
	free((uint8_t*)el - 12);
	el = _el;
	il += _poj - poj;
	poj = _poj;
}
template<class T>
void Wek<T>::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= poj) return;

	T*const _el = (T*const)((uint8_t*)malloc(_poj * sizeof(T) + 12) + 12);
	memcpy(_el, el, poj * sizeof(T));
	free((uint8_t*)el - 12);
	el = _el;
	poj = _poj;
}
template<class T> template<class H>
void Wek<T>::uloz_licz(uint32_t*& _mapa, H _licz_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks, _licz_hasz);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze elementów i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(il*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _licz_hasz(el[_i]) - _hasz_min; // tablica hasze elementów
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
template<class T> template<class H>
void Wek<T>::uloz_uni_licz(uint32_t*& _mapa, H _licz_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks, _licz_hasz);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze elementów i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(il*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _licz_hasz(el[_i]) - _hasz_min; // tablica hasze elementów
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
void Wek<T>::uloz_wyk(uint32_t const*const _mapa) {
	T*const _el = (T*const)malloc(il * sizeof(T));
	uint32_t _il = 0;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		_el[_mapa[_i]] = el[_i];
		++_il;
	}
	memcpy(el, _el, _il * sizeof(T));
	il = _il;
	pierw_pusty = 0x80000000;
	free(_el);
}
template<class T>
void Wek<T>::usun(uint32_t const& _nr, uint32_t const& _il) {
	wyp_pam(&el[_nr], pusty, _il);
	if(_nr < pierw_pusty) pierw_pusty = _nr;
}
template<class T> template<class H>
void Wek<T>::usun_dupl_licz(uint32_t*& _mapa, H _licz_hasz) const {
	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _il_hasz;
	licz_zakres(_hasz_min, _hasz_maks, _licz_hasz);
	_il_hasz = _hasz_maks - _hasz_min + 1;

	// licz mapę usunięcia duplikatów
	bool* _t1 = (bool*)malloc(_il_hasz * sizeof(bool)); // wystąpienia haszy
	memset(_t1, 0, _il_hasz * sizeof(bool));
	free(_mapa);
	_mapa = (uint32_t*)malloc(il * 4); // mapa usunięcia duplikatów
	uint32_t _ind = 0, _hasz;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(el[_i] == pusty) {
			_mapa[_i] = 0x80000000;
			continue;
		}
		_hasz = _licz_hasz(el[_i]) - _hasz_min;
		if(_t1[_hasz]) {
			_mapa[_i] = 0x80000000;
			continue;
		}
		_t1[_hasz] = true;
		_mapa[_i] = _ind++;
	}

	free(_t1);
}
template<class T>
void Wek<T>::usun_kon(uint32_t const& _il) {
	if(il <= _il) il = 0;
	else il -= _il;
}
template<class T>
uint32_t const& Wek<T>::wez_il() const {
	return il;
}
template<class T>
uint32_t const& Wek<T>::wez_pierw_pusty() const {
	return pierw_pusty;
}
template<class T>
void Wek<T>::wstaw_kon(T const& _el, uint32_t const& _il) {
	while(il+_il > poj) rezerw_tyl(poj*2);
	memmove(&el[il], &_el, _il*sizeof(T));
	il += _il;
}
template<class T>
void Wek<T>::zamien(uint32_t const& _nr1, uint32_t const& _nr2) {
	T _el = el[_nr1];
	el[_nr1] = el[_nr2];
	el[_nr2] = _el;
}
template<class T>
T const Wek<T>::pusty = gen_pusty<T>();
// -------------------------------------------------------
template<class T>
class WekLuz {
public:
						WekLuz();
						WekLuz(T*const);
						WekLuz(T*const, uint32_t const&);
	inline T&			operator[](uint32_t const&);
	void				rezerw_tyl(uint32_t const&);
	inline void			niszcz();
	void				aktual(uint32_t const*const&);
	inline void			aktual_nagl();
	void				czysc();
	static T const		pusty;
protected:
	uint32_t			poj;
	T*					el;
};
template<class T>
WekLuz<T>::WekLuz()
	: el((T*)((uint8_t*)malloc(256 * sizeof(T) + 12) + 12)),
	poj(256) {
	czysc();
}
template<class T>
WekLuz<T>::WekLuz(T*const _pam) {
	memcpy(this, _pam, 4);
	el = (T*)((uint8_t*)_pam + 12);
}
template<class T>
WekLuz<T>::WekLuz(T*const _el, uint32_t const& _poj)
	: el(_el),
	poj(_poj) {}
template<class T>
T& WekLuz<T>::operator[](uint32_t const& _nr) {
	while(_nr >= poj) rezerw_tyl(2 * poj);
	return el[_nr];
}
template<class T>
void WekLuz<T>::aktual(uint32_t const*const& _mapa) {
	for(uint32_t _i = 0; _i < poj; ++_i) {
		if(el[_i] == pusty) continue;
		if(_mapa[el[_i]] == 0x80000000) continue;
		el[_i] = _mapa[el[_i]];
	}
}
template<class T>
void WekLuz<T>::aktual_nagl() {
	memcpy((uint8_t*)el - 4, this, 4);
}
template<class T>
void WekLuz<T>::czysc() {
	wyp_pam(el, pusty, poj);
}
template<class T>
void WekLuz<T>::niszcz() {
	free((uint8_t*)el - 12);
}
template<class T>
void WekLuz<T>::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= poj) return;

	T*const _el = (T*)((uint8_t*)malloc(_poj * sizeof(T) + 12) + 12);
	memcpy(_el, el, poj * sizeof(T));
	wyp_pam(_el + poj, pusty, _poj - poj);
	free((uint8_t*)el - 12);
	el = _el;
	poj = _poj;
}
template<class T>
T const WekLuz<T>::pusty = gen_pusty<T>();
// -------------------------------------------------------
template<>
void WekLuz<bool>::aktual(uint32_t const*const&);
template<>
void WekLuz<bool>::czysc();
template<>
void WekLuz<bool>::rezerw_tyl(uint32_t const&);
// -------------------------------------------------------
template<class A, class B = A>
struct Para {
	inline bool		operator==(Para const&) const;
	inline bool		operator!=(Para const&) const;
	A				pierw;
	B				drug;
};
template<class A, class B>
bool Para<A,B>::operator==(Para const& _para) const {
	return (pierw == _para.pierw && drug == _para.drug);
}
template<class A, class B>
bool Para<A,B>::operator!=(Para const& _para) const {
	return !operator==(_para);
}
// -------------------------------------------------------
template<class A, class B>
std::ostream& operator<<(std::ostream& _strumien, Para<A,B> const& _para) {
	_strumien << "{" << _para.pierw << ", " << _para.drug << "}";
	return _strumien;
}
// -------------------------------------------------------
template<class T>
class Wek2 {
public:
	typedef Para<uint32_t> Wiersz_;
	inline T*const&				operator[](uint32_t const&) const;
	inline void					niszcz();
	inline void					wstaw_kon(T const*const, uint32_t const);
	inline void					wstaw_kon(Wek2<T>const&);
	inline void					usun(uint32_t const&, uint32_t = 1);
	template<class H = FunHasz<T>>
	void						usun_dupl_licz(uint32_t*&, H = H()) const;
	inline void					usun_kon(uint32_t = 1);
	inline Wiersz_ const&		wez_wier(uint32_t const&) const;
	inline uint32_t const&		wez_il_wier() const;
	inline uint32_t const&		wez_il_el() const;
	void						defrag_licz(uint32_t*&, uint32_t*&, uint32_t const);
	void						defrag_wyk(uint32_t const*const,
									uint32_t const*const);
	template<class H = FunHasz<T>>
	void						uloz_licz(uint32_t*&, H = H()) const;
	template<class H = FunHasz<T>>
	void						uloz_uni_licz(uint32_t*&, H = H()) const;
	void						uloz_wyk(uint32_t const*const);
	void						aktual(uint32_t const*const);
	inline void					aktual_nagl();
	static Wiersz_ const		pusty;
protected:
	template<class H>
	void						licz_zakres(uint32_t&, uint32_t&, H) const;
	Wek<Wiersz_>				wier;
	Wek<T>						el;
};
template<class T>
T*const& Wek2<T>::operator[](uint32_t const& _nr) const {
	return &el[wier[_nr].pierw];
}
template<class T>
void Wek2<T>::aktual(uint32_t const*const _mapa) {
	uint32_t _j;
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == wier.pusty) continue;
		if(_mapa[el[wier[_i].pierw]] == 0x80000000) continue;
		wyp_pam(&el[wier[_i].pierw], _mapa[el[wier[_i].pierw]], wier[_i].drug);
	}
}
template<class T>
void Wek2<T>::aktual_nagl() {
	wier.aktual_nagl();
	el.aktual_nagl();
}
template<class T>
void Wek2<T>::defrag_licz(uint32_t*& _mapa_wier, uint32_t*& _mapa_el, uint32_t const _il_wier) {
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
void Wek2<T>::defrag_wyk(uint32_t const*const _mapa_wier, uint32_t const*const _mapa_el) {
	wier.defrag_wyk(_mapa_wier);
	el.defrag_wyk(_mapa_el);
}
template<class T> template<class H>
void Wek2<T>::licz_zakres(uint32_t& _hasz_min, uint32_t& _hasz_maks, H _licz_hasz) const {
	_hasz_min = 0x80000000;
	_hasz_maks = 0;
	uint32_t _hasz;
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) continue;
		_hasz = _licz_hasz(el[wier[_i].pierw]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		if(_hasz > _hasz_maks) _hasz_maks = _hasz;
	}
	if(_hasz_min == 0x80000000) _hasz_min = 1;
}
template<class T>
void Wek2<T>::niszcz() {
	wier.niszcz();
	el.niszcz();
}
template<class T> template<class H>
void Wek2<T>::uloz_licz(uint32_t*& _mapa_wier, H _licz_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks, _licz_hasz);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze wierszy i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(wier.wez_il()*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _licz_hasz(el[wier[_i].pierw]) - _hasz_min; // tablica hasze wierszy
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
template<class T> template<class H>
void Wek2<T>::uloz_uni_licz(uint32_t*& _mapa_wier, H _licz_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _hasz_il;
	licz_zakres(_hasz_min, _hasz_maks, _licz_hasz);
	_hasz_il = _hasz_maks - _hasz_min + 1;

	// licz hasze wierszy i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(wier.wez_il()*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _licz_hasz(el[wier[_i].pierw]) - _hasz_min; // tablica hasze wierszy
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
void Wek2<T>::uloz_wyk(uint32_t const*const _mapa_wier) {
	wier.uloz_wyk(_mapa_wier);

	// twórz mapę elementów, aktualizuj wiersze
	uint32_t*const _mapa_el = (uint32_t*const)malloc(el.wez_il() * 4);
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
void Wek2<T>::usun(uint32_t const& _nr, uint32_t _il) {
	for(uint32_t _i = 0; _i < _il; ++_i) {
		el.usun(wier[_nr+_i].pierw, wier[_nr+_i].drug);
		wier.usun(_nr+_i);
	}
}
template<class T> template<class H>
void Wek2<T>::usun_dupl_licz(uint32_t*& _mapa, H _licz_hasz) const {
	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _il_hasz;
	licz_zakres(_hasz_min, _hasz_maks, _licz_hasz);
	_il_hasz = _hasz_maks - _hasz_min + 1;

	// licz mapę usunięcia duplikatów
	bool* _t1 = (bool*)malloc(_il_hasz * sizeof(bool)); // wystąpienia haszy
	memset(_t1, 0, _il_hasz * sizeof(bool));
	free(_mapa);
	_mapa = (uint32_t*)malloc(wier.wez_il() * 4); // mapa usunięcia duplikatów
	uint32_t _ind = 0, _hasz;
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		if(wier[_i] == pusty) {
			_mapa[_i] = 0x80000000;
			continue;
		}
		_hasz = _licz_hasz(el[wier[_i].pierw]) - _hasz_min;
		if(_t1[_hasz]) {
			_mapa[_i] = 0x80000000;
			continue;
		}
		_t1[_hasz] = true;
		_mapa[_i] = _ind++;
	}

	free(_t1);
}
template<class T>
void Wek2<T>::usun_kon(uint32_t _il) {
	if(_il > wier.wez_il()) _il = wier.wez_il();
	for(uint32_t _i = 0; _i < _il; ++_i) {
		el.usun_kon(wier[wier.wez_il()-1].drug);
		wier.usun_kon();
	}
}
template<class T>
uint32_t const& Wek2<T>::wez_il_el() const {
	return el.wez_il();
}
template<class T>
uint32_t const& Wek2<T>::wez_il_wier() const {
	return wier.wez_il();
}
template<class T>
typename Wek2<T>::Wiersz_ const& Wek2<T>::wez_wier(uint32_t const& _nr) const {
	return wier[_nr];
}
template<class T>
void Wek2<T>::wstaw_kon(T const*const _el, uint32_t const _il) {
	if(_il == 0) return;
	wier.wstaw_kon({el.wez_il(), _il});
	el.wstaw_kon(*_el, _il);
}
template<class T>
void Wek2<T>::wstaw_kon(Wek2<T>const& _wek2) {
	wier.wstaw_kon(_wek2.wez_wier(0), _wek2.wez_il_wier());
	el.wstaw_kon(*_wek2[0], _wek2.wez_il_el());
}
template<class T>
typename Wek2<T>::Wiersz_ const Wek2<T>::pusty = gen_pusty<Wiersz_>();
// -------------------------------------------------------



