#pragma once

#include <stdint.h>
#include <operatory.h>
#include <windows.h>
#include <bitset>

using std::bitset;

// -------------------------------------------------------
template<class T>
class Wek {
public:
									Wek();
									~Wek();
	inline T&						operator[](uint32_t const&) const;
	void							rezerw_tyl(uint32_t const&);
	void							rezerw_przod(uint32_t const&);
	inline void						wstaw_kon(T const&, uint32_t const& = 1);
	void							usun(uint32_t const&, uint32_t const& = 1);
	inline T&						usun_kon();
	inline void						czysc();
	inline uint32_t const&			wez_il() const;
	inline uint32_t const&			wez_poj() const;
	inline void						zamien(uint32_t const&, uint32_t const&);
	void							pakuj();
	void							licz_uloz(uint32_t*&) const;
	void							licz_uloz_unikat(uint32_t*&) const;
	void							uloz(uint32_t const*const&);
protected:
	inline uint32_t					wez_hasz(T const&) const;
	void							licz_zakres(uint32_t&, uint32_t&) const;
	bool							czy_pusty(T const&) const;
	T*								el;
	uint32_t						el_il;
	uint32_t						el_poj;
};
template<class T>
Wek<T>::Wek()
	: el((T*)malloc(256*sizeof(T))),
	el_il(0),
	el_poj(256) {}
template<class T>
Wek<T>::~Wek() {
	free(el);
}
template<class T>
T& Wek<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
bool Wek<T>::czy_pusty(T const& _el) const {
	return !memcmp(&_el, &pusty, sizeof(T));
}
template<class T>
void Wek<T>::czysc() {
	el_il = 0;
}
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
		if(el[_i] == _pusty) continue;
		el[_ind++] = el[_i];
	}
	el_il = _ind;
}
template<class T>
void Wek<T>::rezerw_przod(uint32_t const& _poj) {
	if(_poj <= el_poj) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el+_poj-el_poj, el, el_poj*sizeof(T));
	free(el);
	el = _el;
	el_il += _poj - el_poj;
	el_poj = _poj;
}
template<class T>
void Wek<T>::rezerw_tyl(uint32_t const& _poj) {
	if(_poj <= el_poj) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el, el, el_poj*sizeof(T));
	free(el);
	el = _el;
	el_poj = _poj;
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
void Wek<T>::usun(uint32_t const& _nr, uint32_t const& _il) {
	wyp_pam<T>(&el[_nr], gen_min<T>(), _il);
}
template<class T>
T& Wek<T>::usun_kon() {
	return el[--el_il];
}
template<class T>
uint32_t Wek<T>::wez_hasz(T const& _el) const {
	return *(uint32_t*)&_el;
}
template<class T>
uint32_t const& Wek<T>::wez_il() const {
	return el_il;
}
template<class T>
uint32_t const& Wek<T>::wez_poj() const {
	return el_poj;
}
template<class T>
void Wek<T>::wstaw_kon(T const& _el, uint32_t const& _il) {
	while(el_il+_il > el_poj) rezerw_tyl(el_poj*2);
	memmove(&el[el_il], &_el, _il*sizeof(T));
	el_il += _il;
}
template<class T>
void Wek<T>::zamien(uint32_t const& _nr1, uint32_t const& _nr2) {
	T el_rob = el[_nr1];
	el[_nr1] = el[_nr2];
	el[_nr2] = el_rob;
}
// -------------------------------------------------------
template<class T>
class WekStos : protected Wek<T> {
public:
	inline void		rezerw(uint32_t const&);
	inline void		wstaw(T const&, uint32_t const& = 0);
	inline T&		usun();
	inline T&		wez() const;
};
template<class T>
void WekStos<T>::rezerw(uint32_t const& _poj) {
	Wek::rezerw_przod(_poj);
}
template<class T>
T& WekStos<T>::usun() {
	return Wek::usun_kon();
}
template<class T>
T& WekStos<T>::wez() const {
	return Wek::operator[](el_il-1);
}
template<class T>
void WekStos<T>::wstaw(T const& _el, uint32_t const& _il) {
	Wek::wstaw_kon(_el, _il);
}
// -------------------------------------------------------
class ListaWolne : public WekStos<uint32_t> {
public:
								ListaWolne();
	void						rezerw(uint32_t const&);
	inline uint32_t const&		wez_il() const;
	inline uint32_t const&		wez_poj() const;
};
uint32_t const& ListaWolne::wez_il() const {
	return Wek::wez_il();
}
uint32_t const& ListaWolne::wez_poj() const {
	return Wek::wez_poj();
}
// -------------------------------------------------------
template<class T>
class WekPula {
public:
								WekPula();
								~WekPula();
	inline T&					operator[](uint32_t const&) const;
	void						rezerw(uint32_t const&);
	inline uint32_t				wstaw(T const&);
	inline void					usun(uint32_t const&);
	inline uint32_t const&		wez_poj() const;
protected:
	T*							el;
	ListaWolne					el_wolne;
};
template<class T>
WekPula<T>::WekPula() 
	: el((T*)malloc(256*sizeof(T))) {}
template<class T>
WekPula<T>::~WekPula() {
	free(el);
}
template<class T>
T& WekPula<T>::operator[](uint32_t const& _nr) const {
	return el[_nr];
}
template<class T>
void WekPula<T>::rezerw(uint32_t const& _poj) {
	if(_poj <= el_wolne.wez_poj()) return;

	T*const _el = (T*const)malloc(_poj*sizeof(T));
	memmove(_el, el, el_wolne.wez_poj()*sizeof(T));
	free(el);
	el = _el;

	el_wolne.rezerw(_poj);
}
template<class T>
void WekPula<T>::usun(uint32_t const& _nr) {
	if(_nr >= el_poj) return;
	if(el[_nr] == gen_min<T>()) return;
	el[_nr] = gen_min<T>();
	el_wolne.wstaw(_nr);
}
template<class T>
uint32_t const& WekPula<T>::wez_poj() const {
	return el_wolne.wez_poj();
}
template<class T>
uint32_t WekPula<T>::wstaw(T const& _el) {
	if(el_wolne.wez_il() == 0) rezerw(el_wolne.wez_poj()*2);
	el[el_wolne.wez()] = _el;
	return el_wolne.usun();
}
// -------------------------------------------------------
template<class A, class B = A>
struct Para {
	A		pierw;
	B		drug;
};
// -------------------------------------------------------
template<class T>
class WekUch {
public:
	inline T&					operator[](uint32_t const&) const;
	inline void					rezerw(uint32_t const&);
	inline uint32_t				wstaw_kon(T const&, uint32_t const& = 1);
	inline void					usun(uint32_t const&);
	void						pakuj();
protected:
	WekPula<Para<uint32_t>>		seg;
	Wek<T>						el;
};
template<class T>
T& WekUch<T>::operator[](uint32_t const& _nr) const {
	return el[seg[_nr].pierw];
}
template<class T>
void WekUch<T>::pakuj() {
	int32_t _i;

	// licz przesunięcia elementów
	int32_t*const _przes = (int32_t*const)malloc(el_il*4);
	int32_t _przes_akt = 0;
	T const _el_pusty = gen_min<T>();
	for(_i = 0; _i < el_il; ++_i) {
		if(el[_i] == _el_pusty) {
			++_przes_akt;
		} else {
			_przes[_i] = _przes_akt;
		}
	}

	// pakuj elementy
	el.pakuj();

	// aktualizuj segmenty
	Para<uint32_t>const _seg_pusty = gen_min<Para<uint32_t>>();
	for(_i = 0; _i < seg.wez_poj(); ++_i) {
		if(seg[_i] == _seg_pusty) continue;
		seg[_i].pierw -= _przes[seg[_i].pierw];
	}
}
template<class T>
void WekUch<T>::rezerw(uint32_t const& _poj) {
	seg.rezerw(_il);
	el.rezerw_tyl(_il);
}
template<class T>
void WekUch<T>::usun(uint32_t const& _nr) {
	el.usun(seg[_nr].pierw, seg[_nr].drug);
	seg[_nr] = gen_min<Para<uint32_t>>();
}
template<class T>
uint32_t WekUch<T>::wstaw_kon(T const& _el, uint32_t const& _il) {
	el.wstaw_kon(_el, _il);
	return seg.wstaw({el.wez_il()-1, _il});
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektPodst {
public:
						WektPodst();
						~WektPodst();
	void				rezerwuj(uint32_t const&);
	inline uint32_t		wez_rozm_el() const;
	inline uint32_t		wez_il() const;
	inline uint32_t		wez_il_rezerw() const;
	inline void			czysc();
protected:
	void				licz_zakres(uint32_t&, uint32_t&) const;
	uint32_t			_rozm_el;
	H					_f_hasz;
	uint32_t			_il_rezerw;
	uint32_t			_il;
	T*					_tab;
};
template<class T, class H>
WektPodst<T,H>::WektPodst()
	: _rozm_el(sizeof(T)),
	_il_rezerw(4096),
	_il(0),
	_tab((T*)malloc(_il_rezerw*_rozm_el)) {
}
template<class T, class H>
WektPodst<T,H>::~WektPodst() {
	free(_tab);
}
template<class T, class H>
void WektPodst<T,H>::czysc() {
	_il = 0;
}
template<class T, class H>
void WektPodst<T,H>::licz_zakres(uint32_t& hasz_min, uint32_t& hasz_maks) const {
	hasz_min = _f_hasz(_tab[0]);
	hasz_maks = hasz_min;
	uint32_t hasz;
	for(uint32_t i = 1; i < _il; ++i) {
		hasz = _f_hasz(_tab[i]);
		if(hasz < hasz_min) {
			hasz_min = hasz;
		} else if(hasz > hasz_maks) {
			hasz_maks = hasz;
		}
	}
}
template<class T, class H>
void WektPodst<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	T* pam = (T*)malloc(il*_rozm_el);
	memmove(pam, _tab, _il*_rozm_el);
	free(_tab);
	_tab = pam;
	_il_rezerw = il;
}
template<class T, class H>
uint32_t WektPodst<T,H>::wez_rozm_el() const {
	return _rozm_el;
}
template<class T, class H>
uint32_t WektPodst<T,H>::wez_il() const {
	return _il;
}
template<class T, class H>
uint32_t WektPodst<T,H>::wez_il_rezerw() const {
	return _il_rezerw;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektWpis : public WektPodst<T,H> {
public:
	inline T&			operator[](uint32_t const&) const;
	inline void			wstaw_kon(T const&);
	void				uloz();
	void				uloz_unikat();
};
template<class T, class H>
T& WektWpis<T,H>::operator[](uint32_t const& nr) const {
	return _tab[nr];
}
template<class T, class H>
void WektWpis<T,H>::uloz() {
	if(_il < 2) return;

	uint32_t hasz_min, hasz_maks;
	licz_zakres(hasz_min, hasz_maks);
	uint32_t i, il_zakres = hasz_maks-hasz_min+1;
	T*const tab = (T*)malloc(_il*_rozm_el);
	uint32_t*const powtorzenia = (uint32_t*)malloc(il_zakres*sizeof(uint32_t));
	uint32_t*const indeksy = (uint32_t*)malloc(il_zakres*sizeof(uint32_t));
	ZeroMemory(powtorzenia, il_zakres*sizeof(uint32_t));

	for(i = 0; i < _il; ++i) {
		tab[i] = _tab[i];
		++(powtorzenia[_f_hasz(_tab[i])-hasz_min]);
	}

	indeksy[0] = 0;
	for(i = 1; i < il_zakres; ++i) {
		indeksy[i] = indeksy[i-1] + powtorzenia[i-1];
	}

	ZeroMemory(powtorzenia, il_zakres*sizeof(uint32_t));
	uint32_t hasz;
	for(i = 0; i < _il; ++i) {
		hasz = _f_hasz(tab[i])-hasz_min;
		_tab[indeksy[hasz] + powtorzenia[hasz]] = tab[i];
		++(powtorzenia[hasz]);
	}

	free(tab);
	free(powtorzenia);
	free(indeksy);
}
template<class T, class H>
void WektWpis<T,H>::uloz_unikat() {
	if(_il < 2) return;

	uint32_t hasz_min, hasz_maks;
	licz_zakres(hasz_min, hasz_maks);
	uint32_t i, il, indeks, il_zakres = hasz_maks-hasz_min+1;
	T*const tab = (T*)malloc(il_zakres*_rozm_el);
	bool*const maska = (bool*)malloc(il_zakres);
	ZeroMemory(maska, il_zakres);

	// przepisz sortując / usuwając podwójne elementy
	il = 0;
	for(i = 0; i < _il; ++i) {
		indeks = _f_hasz(_tab[i])-hasz_min;
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
void WektWpis<T,H>::wstaw_kon(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = el;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class Wekt : public WektWpis<T,H> {
public:
	void		zamien(uint32_t const&, uint32_t const&);
	void		usun(uint32_t const&);
};
template<class T, class H>
void Wekt<T,H>::usun(uint32_t const& nr) {
	zamien(nr, --_il);
}
template<class T, class H>
void Wekt<T,H>::zamien(uint32_t const& nr1, uint32_t const& nr2) {
	T rob = _tab[nr1];
	_tab[nr1] = _tab[nr2];
	_tab[nr2] = rob;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektStos : public WektPodst<T,H> {
public:
	inline void		wstaw(T const&);
	inline T&		wez();
	inline T		usun();
};
template<class T, class H>
T WektStos<T,H>::usun() {
	return _tab[--_il];
}
template<class T, class H>
T& WektStos<T,H>::wez() {
	return _tab[_il-1];
}
template<class T, class H>
void WektStos<T,H>::wstaw(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = el;
}
// -------------------------------------------------------
class WektWolne
	: public WektStos<uint32_t> {
public:
				WektWolne();
	void		rezerwuj(uint32_t const&);
};
class MenUch
	: public WektPodst<uint32_t,FunHasz<uint32_t>> {
public:
	uint32_t		tworz(uint32_t const&);
	void			niszcz(uint32_t const&);
protected:

	WektWolne		_wolne;
};
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektZachSegPodst : public WektPodst<T,H> {
protected:
	typedef Para<uint32_t>		Segment_;
public:
											WektZachSegPodst();
											~WektZachSegPodst();
	inline T*								operator[](uint32_t const&) const;
	inline Segment_ const&					wez_seg(uint32_t const&) const;
	inline void								usun_zbierz(uint32_t const&);
	inline WektWpis<uint32_t> const&		wez_do_usun() const;
protected:
	T*										_tab_rob;
	Segment_*								_seg;
	WektWolne								_seg_wolne;
	WektWpis<uint32_t>						_seg_do_usun;
};
template<class T, class H>
WektZachSegPodst<T,H>::WektZachSegPodst()
	: _tab_rob((T*)malloc(_il_rezerw*_rozm_el)),
	_seg((Segment_*)malloc(_il_rezerw*sizeof(Segment_))) {
}
template<class T, class H>
WektZachSegPodst<T,H>::~WektZachSegPodst() {
	free(_tab_rob);
	free(_seg);
}
template<class T, class H>
T* WektZachSegPodst<T,H>::operator[](uint32_t const& nr) const {
	if(_seg[nr].drug == 0) return 0;
	return &_tab[_seg[nr].pierw];
}
template<class T, class H>
void WektZachSegPodst<T,H>::usun_zbierz(uint32_t const& nr) {
	if(_seg[nr].drug == 0) return;
	_seg_do_usun.wstaw_kon(nr);
}
template<class T, class H>
WektWpis<uint32_t> const& WektZachSegPodst<T,H>::wez_do_usun() const {
	return _seg_do_usun;
}
template<class T, class H>
Para<uint32_t> const& WektZachSegPodst<T,H>::wez_seg(uint32_t const& nr) const {
	return _seg[nr];
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektZachSeg
	: public WektZachSegPodst<T,H> {
public:
	void									rezerwuj(uint32_t const&);
	uint32_t								wstaw_kon(
												T const*const&, uint32_t const&);
	void									usun_wyk();
};
template<class T, class H>
void WektZachSeg<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	_seg_wolne.rezerwuj(il);

	free(_tab_rob);
	_tab_rob = (T*)malloc(il*_rozm_el);

	Segment_* pam2 = (Segment_*)malloc(il*sizeof(Segment_));
	memmove(pam2, _seg, _il_rezerw*sizeof(Segment_));
	free(_seg);
	_seg = pam2;

	WektPodst::rezerwuj(il);
	_seg_do_usun.rezerwuj(il);
}
// =======================================================
template<class T, class H>
void WektZachSeg<T,H>::usun_wyk() {
	if(_seg_do_usun.wez_il() == 0) return;

	_seg_do_usun.uloz_unikat();
	uint32_t ind1 = 0, ind2 = 0, il, j, przes_akt = 0;
	uint32_t* przes = (uint32_t*)malloc(_il*4);
	for(uint32_t i = 0; i < _seg_do_usun.wez_il(); ++i) {
		il = _seg[_seg_do_usun[i]].pierw - ind1;

		// kopiuj upakowane
		memmove(&_tab_rob[ind2], &_tab[ind1], il*_rozm_el);

		// licz przesunięcia
		for(j = ind1; j < ind1+il+_seg[_seg_do_usun[i]].drug; ++j) {
			przes[j] = przes_akt;
		}
		przes_akt += _seg[_seg_do_usun[i]].drug;

		// ustaw na kolejny obszar pamięci do upakowania
		ind1 += il + _seg[_seg_do_usun[i]].drug;
		ind2 += il;

		// usuń segment
		_seg[_seg_do_usun[i]].drug = 0;
		_seg_wolne.wstaw(_seg_do_usun[i]);
	}
	_seg_do_usun.czysc();
	_il = ind2;

	// uwzględnij w segmentach przesunięcie elementów tablicy
	for(j = 0; j < _il_rezerw; ++j) {
		_seg[j].pierw -= przes[_seg[j].pierw];
	}

	// przeskocz na upakowaną tablicę
	T* wsk = _tab;
	_tab = _tab_rob;
	_tab_rob = wsk;
}
template<class T, class H>
uint32_t WektZachSeg<T,H>::wstaw_kon(T const*const& t, uint32_t const& il = 1) {
	while(_il+il > _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_seg[_seg_wolne.wez()] = {_il, il};
	for(uint32_t i = 0; i < il; ++i) {
		_tab[_il++] = t[i];
	}
	return _seg_wolne.usun();
}
// -------------------------------------------------------







