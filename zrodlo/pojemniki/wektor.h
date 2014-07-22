# pragma once

#include <stdint.h>
#include <operatory.h>

// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektorPodstawa {
public:
						WektorPodstawa();
						~WektorPodstawa();
	void				rezerwuj(uint32_t const&);
	inline uint32_t		wez_il() const;
	inline uint32_t		wez_il_rezerw() const;
	inline void			czysc();
protected:
	void				licz_zakres(uint32_t&, uint32_t&) const;
	uint8_t				_rozm_el;
	H					_f_hasz;
	uint32_t			_il_rezerw;
	uint32_t			_il;
	T*					_tab;
};
template<class T, class H>
WektorPodstawa<T,H>::WektorPodstawa()
	: _rozm_el(sizeof(T)),
	_il_rezerw(4096),
	_il(0),
	_tab((T*)malloc(_il_rezerw*_rozm_el)) {
}
template<class T, class H>
WektorPodstawa<T,H>::~WektorPodstawa() {
	free(_tab);
}
template<class T, class H>
void WektorPodstawa<T,H>::czysc() {
	_il = 0;
}
template<class T, class H>
void WektorPodstawa<T,H>::licz_zakres(uint32_t& hasz_min, uint32_t& hasz_maks) const {
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
void WektorPodstawa<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	T* pam = (T*)malloc(il*_rozm_el);
	memmove(pam, _tab, _il*_rozm_el);
	free(_tab);
	_tab = pam;
	_il_rezerw = il;
}
template<class T, class H>
uint32_t WektorPodstawa<T,H>::wez_il() const {
	return _il;
}
template<class T, class H>
uint32_t WektorPodstawa<T,H>::wez_il_rezerw() const {
	return _il_rezerw;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektorWpis : public WektorPodstawa<T,H> {
public:
	inline T&			operator[](uint32_t const&) const;
	inline void			wstaw_kon(T const&);
	void				uloz();
	void				uloz_unikat();
};
template<class T, class H>
T& WektorWpis<T,H>::operator[](uint32_t const& nr) const {
	return _tab[nr];
}
template<class T, class H>
void WektorWpis<T,H>::uloz() {
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
void WektorWpis<T,H>::uloz_unikat() {
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
void WektorWpis<T,H>::wstaw_kon(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = el;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class Wektor : public WektorWpis<T,H> {
public:
	void		zamien(uint32_t const&, uint32_t const&);
	void		usun(uint32_t const&);
};
template<class T, class H>
void Wektor<T,H>::usun(uint32_t const& nr) {
	zamien(nr, --_il);
}
template<class T, class H>
void Wektor<T,H>::zamien(uint32_t const& nr1, uint32_t const& nr2) {
	T rob = _tab[nr1];
	_tab[nr1] = _tab[nr2];
	_tab[nr2] = rob;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektorStos : public WektorPodstawa<T,H> {
public:
	inline void		wstaw(T const&);
	inline T&		wez();
	inline T		usun();
};
template<class T, class H>
T WektorStos<T,H>::usun() {
	return _tab[--_il];
}
template<class T, class H>
T& WektorStos<T,H>::wez() {
	return _tab[_il-1];
}
template<class T, class H>
void WektorStos<T,H>::wstaw(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = el;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektorZachSeg : WektorPodstawa<T,H> {
	struct Segment {
		uint32_t				pocz;
		uint32_t				il;
	};
public:
								WektorZachSeg();
								~WektorZachSeg();
	T*							operator[](uint32_t const&) const;
	void						rezerwuj(uint32_t const&);
	uint32_t					wstaw_kon(T const*const&, uint32_t const&);
	void						usun(uint32_t const&);
	void						upakuj();
protected:
	T*							_tab_rob;
	Segment*					_seg;
	uint32_t*					_seg_wolne;
	uint32_t					_il_seg_wolne;
	WektorWpis<uint32_t>		_seg_usuniete;
};
template<class T, class H>
WektorZachSeg<T,H>::WektorZachSeg()
	: _tab_rob((T*)malloc(_il_rezerw*_rozm_el)),
	_seg_wolne((uint32_t*)malloc(_il_rezerw*4)),
	_il_seg_wolne(0) {
	for(uint32_t i = 0; i < _il_rezerw; ++i) {
		_seg_wolne[i] = _il_rezerw-1-i;
	}
}
template<class T, class H>
WektorZachSeg<T,H>::~WektorZachSeg() {
	free(_tab_rob);
	free(_seg);
	free(_seg_wolne);
}
template<class T, class H>
T* WektorZachSeg<T,H>::operator[](uint32_t const& nr) const {
	return &_tab[_seg[nr].pocz];
}
template<class T, class H>
void WektorZachSeg<T,H>::rezerwuj(uint32_t const& il) {
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
	_seg_usuniete.rezerwuj(il);

	free(_tab_rob);
	_tab_rob = (T*)malloc(il*_rozm_el);

	Segment* pam2 = (Segment*)malloc(il*sizeof(Segment));
	memmove(pam2, _seg, (il-_il_seg_wolne)*sizeof(Segment));
	free(_seg);
	_seg = pam2;
}
template<class T, class H>
void WektorZachSeg<T,H>::upakuj() {
	_seg_usuniete.uloz_unikat();
	uint32_t ind1 = 0, ind2 = 0, il, j, przes_akt = 0;
	uint32_t* przes = (uint32_t*)malloc(_il*4);
	for(uint32_t i = 0; i < _seg_usuniete.wez_il(); ++i) {
		il = _seg[_seg_usuniete[i]].pocz - ind1;

		// kopiuj upakowane
		memmove(&_tab_rob[ind2], &_tab[ind1], il*_rozm_el);

		// licz przesunięcia
		for(j = ind1; j < ind1+il+_seg[_seg_usuniete[i]].il; ++j) {
			przes[j] = przes_akt;
		}
		przes_akt += _seg[_seg_usuniete[i]].il;

		// ustaw na kolejny obszar pamięci do upakowania
		ind1 += il + _seg[_seg_usuniete[i]].il;
		ind2 += il;

		// usuń segment
		_seg[_seg_usuniete[i]].il = 0;
		_seg_wolne[_il_seg_wolne++] = _seg_usuniete[i];
	}
	_seg_usuniete.czysc();
	_il = ind2;

	// uwzględnij w segmentach przesunięcie elementów tablicy
	for(j = 0; j < _il_rezerw; ++j) {
		_seg[j].pocz -= przes[_seg[j].pocz];
	}

	// przeskocz na upakowaną tablicę
	T* wsk = _tab;
	_tab = _tab_rob;
	_tab_rob = wsk;
}
template<class T, class H>
void WektorZachSeg<T,H>::usun(uint32_t const& nr) {
	zapobiegać przed usunięciem nieistniejącego segmentu
	_seg_usuniete.wstaw_kon(nr);
}
template<class T, class H>
uint32_t WektorZachSeg<T,H>::wstaw_kon(T const*const& t, uint32_t const& il = 1) {
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





