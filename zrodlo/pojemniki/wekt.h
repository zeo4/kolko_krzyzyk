#pragma once

#include <stdint.h>
#include <operatory.h>
#include <windows.h>

// -------------------------------------------------------
class MenPam {
public:
	char**				_p;
	uint32_t			_typ_rozm;
	uint32_t			_il;
	uint32_t			_il_rezerw;
	inline char*		operator[](uint32_t const&) const;
	void				rezerw_kon(uint32_t const&);
	void				rezerw_pocz(uint32_t const&);
	inline void			wstaw_kon(void*const&);
	inline char*		usun_kon();
	inline void			czysc();
	void				zamien(uint32_t const&, uint32_t const&);
	void				licz_uloz_unikat(uint32_t*&);
	void				licz_uloz(uint32_t*&);
	void				uloz(uint32_t const*const&);
private:
	inline uint32_t		wez_hasz(char const*const&) const;
	void				licz_zakres(uint32_t&, uint32_t&) const;
};
char* MenPam::operator[](uint32_t const& nr) const {
	if(_il == 0) return 0;
	return (*_p) + nr * _typ_rozm;
}
void MenPam::czysc() {
	_il = 0;
}
char* MenPam::usun_kon() {
	return (*_p)+(--_il)*_typ_rozm;
}
void MenPam::wstaw_kon(void*const& pam) {
	if(_il == _il_rezerw) rezerw_kon(_il_rezerw*2);
	memmove((*_p)+(_il++)*_typ_rozm, pam, _typ_rozm);
}
// -------------------------------------------------------
template<class A, class B>
struct Para {
	A		pierw;
	B		drug;
};
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
// =======================================================
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
	typedef Para<uint32_t, uint32_t>		Segment_;
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
Para<uint32_t, uint32_t> const& WektZachSegPodst<T,H>::wez_seg(uint32_t const& nr) const {
	return _seg[nr];
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektZachSeg : public WektZachSegPodst<T,H> {
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







