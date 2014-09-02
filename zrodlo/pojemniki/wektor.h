#pragma once

#include <stdint.h>
#include <operatory.h>

// -------------------------------------------------------
template<class T>
class WekBaza {
public:
								WekBaza();
								~WekBaza();
	inline T&					operator[](uint32_t const&) const;
	void						rezerw_tyl(uint32_t const&);
	void						maz(uint32_t const&, uint32_t const& = 1);
	inline uint32_t const&		wez_rozm() const;
	inline uint32_t const&		wez_poj() const;
	inline void					zamien(uint32_t const&, uint32_t const&);
	inline bool					sprawdz_pusty(uint32_t const&) const;
protected:
	T*							el;
	uint32_t					el_poj;
	static T const				pusty;
};
template<class T>
WekBaza<T>::WekBaza()
	: el((T*)malloc(256*sizeof(T))),
	el_poj(256) {
	wyp_pam(el, pusty, 256);
}
template<class T>
WekBaza<T>::~WekBaza() {
	free(el);
}
template<class T>
T& WekBaza<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
void WekBaza<T>::maz(uint32_t const& _nr, uint32_t const& _il) {
	wyp_pam<T>(&el[_nr], gen_min<T>(), _il);
}
template<class T>
void WekBaza<T>::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= el_poj) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el, el, el_poj*sizeof(T));
	wyp_pam(_el+el_poj, pusty, _poj-el_poj);
	free(el);
	el = _el;
	el_poj = _poj;
}
template<class T>
bool WekBaza<T>::sprawdz_pusty(uint32_t const& _nr) const {
	return !memcmp(&el[_nr], &pusty, sizeof(T));
}
template<class T>
uint32_t const& WekBaza<T>::wez_poj() const {
	return el_poj;
}
template<class T>
uint32_t const& WekBaza<T>::wez_rozm() const {
	return sizeof(T);
}
template<class T>
void WekBaza<T>::zamien(uint32_t const& _nr1, uint32_t const& _nr2) {
	T el_rob = el[_nr1];
	el[_nr1] = el[_nr2];
	el[_nr2] = el_rob;
}
template<class T>
T const WekBaza<T>::pusty = gen_min<T>();
// -------------------------------------------------------
template<class T>
class Wek : public WekBaza<T> {
public:
								Wek();
	void						rezerw_przod(uint32_t const&);
	inline void					wstaw_kon(T const&, uint32_t const& = 1);
	inline T const&				usun_kon();
	inline uint32_t const&		wez_il() const;
	void						pakuj();
	void						licz_uloz(uint32_t*&) const;
	void						licz_uloz_unikat(uint32_t*&) const;
	void						uloz(uint32_t const*const&);
	uint32_t					szuk(T const&) const;
protected:
	void						licz_zakres(uint32_t&, uint32_t&) const;
	uint32_t					el_il;
};
template<class T>
Wek<T>::Wek()
	: el_il(0) {}
template<class T>
void Wek<T>::licz_uloz(uint32_t*& _el_ind) const {
	if(el_il < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _il_zakres;
	licz_zakres(_hasz_min, _hasz_maks);
	_il_zakres = _hasz_maks - _hasz_min + 1;

	// licz hasze elementów i ich powtórzenia
	free(_el_ind);
	_el_ind = (uint32_t*)malloc(el_il*4);
	uint32_t* _hasz_powt = (uint32_t*)malloc(_il_zakres*4);
	memset(_hasz_powt, 0, _il_zakres*4);
	for(_i = 0; _i < el_il; ++_i) {
		_el_ind[_i] = wez_hasz(el[_i]) - _hasz_min;
		++_hasz_powt[_el_ind[_i]];
	}

	// licz indeksy haszy
	uint32_t* _hasz_ind = (uint32_t*)malloc(_il_zakres*4);
	uint32_t _ind = 0;
	for(_i = 0; _i < _il_zakres; ++_i) {
		_hasz_ind[_i] = _ind;
		_ind += _hasz_powt[_i];
	}

	// licz indeksy elementów
	memset(_hasz_powt, 0, _il_zakres*4);
	uint32_t _hasz;
	for(_i = 0; _i < el_il; ++_i) {
		_hasz = _el_ind[_i];
		_el_ind[_i] = _hasz_ind[_hasz] + _hasz_powt[_hasz];
		++_hasz_powt[_hasz];
	}
}
template<class T>
void Wek<T>::licz_uloz_unikat(uint32_t*& _el_ind) const {
	if(el_il < 2) return;
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_maks, _il_zakres;
	licz_zakres(_hasz_min, _hasz_maks);
	_il_zakres = _hasz_maks - _hasz_min + 1;

	// licz / oznacz hasze elementów, nie-hasze to -1
	free(_el_ind);
	_el_ind = (uint32_t*)malloc(el_il*4);
	uint32_t* _hasz_ind = (uint32_t*)malloc(_il_zakres*4);
	memset(_hasz_ind, -1, _il_zakres*4);
	for(_i = 0; _i < el_il; ++_i) {
		_el_ind[_i] = wez_hasz(el[_i]) - _hasz_min;
		_hasz_ind[_el_ind[_i]] = 1;
	}

	// licz indeksy haszy
	uint32_t _ind = 0;
	for(_i = 0; _i < _il_zakres; ++_i) {
		if(_hasz_ind[_i] != -1) {
			_hasz_ind[_i] = _ind++;
		}
	}

	// licz indeksy elementów
	for(_i = 0; _i < el_il; ++_i) {
		_el_ind[_i] = _hasz_ind[_el_ind[_i]];
	}
}
template<class T>
void Wek<T>::licz_zakres(uint32_t& _hasz_min, uint32_t& _hasz_maks) const {
	_hasz_min = wez_hasz(el[0]);
	_hasz_maks = _hasz_min;
	uint32_t _hasz;
	for(uint32_t _i = 1; _i < el_il; ++_i) {
		_hasz = wez_hasz(el[_i]);
		if(_hasz < _hasz_min) {
			_hasz_min = _hasz;
		} else if(_hasz > _hasz_maks) {
			_hasz_maks = _hasz;
		}
	}
}
template<class T>
void Wek<T>::pakuj() {
	uint32_t _ind = 0;
	T const _pusty = gen_min<T>();

	for(int32_t _i = 0; _i < el_il; ++_i) {
		if(memcmp(&el[_i], &pusty, sizeof(T)) == 0) continue;
		el[_ind++] = el[_i];
	}
	el_il = _ind;
}
template<class T>
void Wek<T>::rezerw_przod(uint32_t const& _poj) {
	if(_poj <= el_poj) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el+_poj-el_poj, el, el_poj*sizeof(T));
	wyp_pam(_el, pusty, _poj-el_poj);
	free(el);
	el = _el;
	el_il += _poj - el_poj;
	el_poj = _poj;
}
template<class T>
uint32_t Wek<T>::szuk(T const& _el) const {
	int32_t _min = 0, _maks = el_il-1, _sr;
	uint32_t _el_hasz = wez_hasz<T>(_el), _sr_hasz;
	while(_min <= _maks) {
		_sr = (_min+_maks)/2;
		_sr_hasz = wez_hasz<T>(el[_sr]);
		if(_sr_hasz == _el_hasz) return _sr;
		if(_sr_hasz < _el_hasz) {
			_maks = _sr-1;
		} else {
			_min = _sr+1;
		}
	}
	return gen_min<uint32_t>();
}
template<class T>
void Wek<T>::uloz(uint32_t const*const& _el_ind) {
	T*const _el = (T*const)malloc(el_poj*sizeof(T));
	T const _pusty = gen_min<T>();
	wyp_pam<T>(_el, _pusty, el_poj);
	uint32_t _el_il = 0;
	for(int32_t _i = el_il-1; _i > -1; --_i) {
		if(_el[_el_ind[_i]] == _pusty){
			++_el_il;
		}
		_el[_el_ind[_i]] = el[_i];
	}
	free(el);
	el = _el;
	el_il = _el_il;
}
template<class T>
T const& Wek<T>::usun_kon() {
	return el[--el_il];
}
template<class T>
uint32_t const& Wek<T>::wez_il() const {
	return el_il;
}
template<class T>
void Wek<T>::wstaw_kon(T const& _el, uint32_t const& _il) {
	while(el_il+_il > el_poj) rezerw_tyl(el_poj*2);
	memmove(&el[el_il], &_el, _il*sizeof(T));
	el_il += _il;
}
// -------------------------------------------------------
template<class T>
class WekStos : protected Wek<T> {
public:
	inline void					rezerw(uint32_t const&);
	inline void					wstaw(T const&, uint32_t const& = 0);
	inline T const&				usun();
	inline T&					wez() const;
	inline uint32_t const&		wez_il() const;
	inline uint32_t const&		wez_poj() const;
};
template<class T>
void WekStos<T>::rezerw(uint32_t const& _poj) {
	Wek::rezerw_przod(_poj);
}
template<class T>
T const& WekStos<T>::usun() {
	return Wek::usun_kon();
}
template<class T>
T& WekStos<T>::wez() const {
	return Wek::operator[](el_il-1);
}
template<class T>
uint32_t const& WekStos<T>::wez_il() const {
	return Wek::wez_il();
}
template<class T>
uint32_t const& WekStos<T>::wez_poj() const {
	return Wek::wez_poj();
}
template<class T>
void WekStos<T>::wstaw(T const& _el, uint32_t const& _il) {
	Wek::wstaw_kon(_el, _il);
}
// -------------------------------------------------------
class ListaWolne : public WekStos<uint32_t> {
public:
				ListaWolne();
	void		rezerw(uint32_t const&);
};
// -------------------------------------------------------
template<class T>
class WekPula : protected WekBaza<T> {
public:
	inline T&					operator[](uint32_t const&) const;
	inline void					rezerw(uint32_t const&);
	inline uint32_t const&		wpisz(T const&);
	inline void					maz(uint32_t const&);
	inline uint32_t const&		wez_poj() const;
	WekBaza::sprawdz_pusty;
protected:
	ListaWolne					el_wolne;
};
template<class T>
T& WekPula<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
void WekPula<T>::maz(uint32_t const& _nr) {
	if(_nr >= el_poj) return;
	if(memcmp(&el[_nr], &pusty, sizeof(T)) == 0) return;
	WekBaza::maz(_nr);
	el_wolne.wstaw(_nr);
}
template<class T>
void WekPula<T>::rezerw(uint32_t const& _poj) {
	WekBaza::rezerw_tyl(_poj);
	el_wolne.rezerw(_poj);
}
template<class T>
uint32_t const& WekPula<T>::wez_poj() const {
	return el_poj;
}
template<class T>
uint32_t const& WekPula<T>::wpisz(T const& _el) {
	if(el_wolne.wez_il() == 0) rezerw(el_wolne.wez_poj()*2);
	el[el_wolne.wez()] = _el;
	return el_wolne.usun();
}
// -------------------------------------------------------
template<class T>
class WekLuz : protected WekBaza<T> {
public:
	inline T&					operator[](uint32_t const&) const;
	inline void					rezerw(uint32_t const&);
	inline void					wpisz(T const&, uint32_t const&);
	inline void					maz(uint32_t const&);
	inline uint32_t const&		wez_poj() const;
	WekBaza::sprawdz_pusty;
};
template<class T>
T& WekLuz<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
void WekLuz<T>::maz(uint32_t const& _nr) {
	el[_nr] = pusty;
}
template<class T>
void WekLuz<T>::rezerw(uint32_t const& _poj) {
	WekBaza::rezerw_tyl(_poj);
}
template<class T>
uint32_t const& WekLuz<T>::wez_poj() const {
	return el_poj;
}
template<class T>
void WekLuz<T>::wpisz(T const& _el, uint32_t const& _nr) {
	while(_nr >= el_poj) rezerw(el_poj*2);
	el[_nr] = _el;
}
// -------------------------------------------------------
template<class A, class B = A>
struct Para {
	A		pierw;
	B		drug;
};
// -------------------------------------------------------
template<class T, template<class>class S>
class WekSegBaza {
public:
	inline T&							operator[](uint32_t const&) const;
	inline void							maz(uint32_t const&);
	inline S<Para<uint32_t>> const&		wez_seg() const;
	inline Wek<T> const&				wez_el() const;
	void								pakuj();
protected:
	S<Para<uint32_t>>					seg;
	Wek<T>								el;
};
template<class T, template<class>class S>
T& WekSegBaza<T,S>::operator[](uint32_t const& _nr) const {
	return el[seg[_nr].pierw];
}
template<class T, template<class>class S>
void WekSegBaza<T,S>::maz(uint32_t const& _nr) {
	el.maz(seg[_nr].pierw, seg[_nr].drug);
	seg.maz(_nr);
}
template<class T, template<class>class S>
void WekSegBaza<T,S>::pakuj() {
	uint32_t _i;

	// licz przesunięcia
	uint32_t*const _przes = (uint32_t*const)malloc(el.wez_il()*4);
	uint32_t _przes_akt = 0;
	for(_i = 0; _i < el.wez_il(); ++_i) {
		if(el.sprawdz_pusty(_i)) {
			++_przes_akt;
		} else {
			_przes[_i] = _przes_akt;
		}
	}

	// pakuj elementy
	el.pakuj();

	// aktualizuj segmenty
	for(_i = 0; _i < seg.wez_poj(); ++_i) {
		if(seg.sprawdz_pusty(_i)) continue;
		seg[_i].pierw -= _przes[seg[_i].pierw];
	}
}
template<class T, template<class>class S>
S<Para<uint32_t>> const& WekSegBaza<T,S>::wez_seg() const {
	return seg;
}
template<class T, template<class>class S>
Wek<T> const& WekSegBaza<T,S>::wez_el() const {
	return el;
}
// -------------------------------------------------------
template<class T>
class WekSegPula : public WekSegBaza<T,WekPula> {
public:
	inline uint32_t const&		wstaw_kon(T const&, uint32_t const& = 1);
};
template<class T>
uint32_t const& WekSegPula<T>::wstaw_kon(T const& _el, uint32_t const& _il) {
	el.wstaw_kon(_el, _il);
	return seg.wpisz({el.wez_il()-_il, _il});
}
// -------------------------------------------------------
template<class T>
class WekSegLuz : public WekSegBaza<T,WekLuz> {
public:
	inline void		wstaw_kon(T const&, uint32_t const&, uint32_t const& = 1);
};
template<class T>
void WekSegLuz<T>::wstaw_kon(T const& _el, uint32_t const& _nr, uint32_t const& _il) {
	if(seg[_nr] != seg.pusty) return;
	seg.wpisz({el.wez_il(), _il}, _nr);
	el.wstaw_kon(_el, _il);
}
// -------------------------------------------------------







