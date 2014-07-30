# pragma once

#include <stdint.h>
#include <operatory.h>

// -------------------------------------------------------
template<class A, class B>
class Para {
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
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektZachSeg : WektPodst<T,H> {
	typedef Para<uint32_t, uint32_t>		Segment_;
public:
											WektZachSeg();
											~WektZachSeg();
	inline T*								operator[](uint32_t const&) const;
	void									rezerwuj(uint32_t const&);
	uint32_t								wstaw_kon(
												T const*const&, uint32_t const&);
	inline Segment_ const&					wez_seg(uint32_t const&) const;
	inline void								usun_zbierz(uint32_t const&);
	inline WektWpis<uint32_t> const&		wez_do_usun() const;
	void									usun_wykonaj();
protected:
	T*										_tab_rob;
	Segment_*								_seg;
	uint32_t*								_seg_wolne;
	uint32_t								_il_seg_wolne;
	WektWpis<uint32_t>						_seg_do_usun;
};
template<class T, class H>
WektZachSeg<T,H>::WektZachSeg()
	: _tab_rob((T*)malloc(_il_rezerw*_rozm_el)),
	_seg_wolne((uint32_t*)malloc(_il_rezerw*4)),
	_il_seg_wolne(0) {
	for(uint32_t i = 0; i < _il_rezerw; ++i) {
		_seg_wolne[i] = _il_rezerw-1-i;
	}
}
template<class T, class H>
WektZachSeg<T,H>::~WektZachSeg() {
	free(_tab_rob);
	free(_seg);
	free(_seg_wolne);
}
template<class T, class H>
T* WektZachSeg<T,H>::operator[](uint32_t const& nr) const {
	if(_seg[nr].drug == 0) return 0;

	return &_tab[_seg[nr].pierw];
}
template<class T, class H>
void WektZachSeg<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	uint32_t* pam1 = (uint32_t*)malloc(il*4);
	uint32_t il_nowych = il-_il_rezerw;
	memmove(pam1+il_nowych, _seg_wolne, _il_seg_wolne*4);
	free(_seg_wolne);
	_seg_wolne = pam1;
	_il_seg_wolne += il_nowych;
	for(uint32_t i = 0; i < il_nowych; ++i) {
		_seg_wolne[i] = il-1-i;
	}

	WektorPodstawa::rezerwuj(il);
	_seg_do_usun.rezerwuj(il);

	free(_tab_rob);
	_tab_rob = (T*)malloc(il*_rozm_el);

	Segment* pam2 = (Segment*)malloc(il*sizeof(Segment));
	memmove(pam2, _seg, (il-_il_seg_wolne)*sizeof(Segment));
	free(_seg);
	_seg = pam2;
}
template<class T, class H>
void WektZachSeg<T,H>::usun_wykonaj() {
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
		_seg_wolne[_il_seg_wolne++] = _seg_do_usun[i];
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
void WektZachSeg<T,H>::usun_zbierz(uint32_t const& nr) {
	if(_seg[nr].drug == 0) return;

	_seg_do_usun.wstaw_kon(nr);
}
template<class T, class H>
WektWpis<uint32_t> const& WektZachSeg<T,H>::wez_do_usun() const {
	return _seg_do_usun;
}
template<class T, class H>
Para<uint32_t,uint32_t> const& WektZachSeg<T,H>::wez_seg(uint32_t const& nr) const {
	return _seg[nr];
}
template<class T, class H>
uint32_t WektZachSeg<T,H>::wstaw_kon(T const*const& t, uint32_t const& il = 1) {
	while(_il+il > _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_seg[_seg_wolne[_il_seg_wolne-1]] = {_il, il};
	for(uint32_t i = 0; i < il; ++i) {
		_tab[_il++] = t[i];
	}
	return _seg_wolne[--_il_seg_wolne];
}
// -------------------------------------------------------





