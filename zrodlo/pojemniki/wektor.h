# pragma once

#include <stdint.h>
#include <operatory.h>

struct Segment {
	uint32_t		pocz;
	uint32_t		il;
};
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektorPodstawa {
public:
						WektorPodstawa();
						~WektorPodstawa();
	void				rezerwuj(uint32_t const&) const;
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
void WektorPodstawa<T,H>::rezerwuj(uint32_t const& il) const {
	if(il <= _il_rezerw) return;

	T* pam = (T*)malloc(il*_rozm_el);
	memmove(pam, _tab, _il_rezerw*_rozm_el);
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
	inline uint32_t		wstaw_kon();
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
uint32_t WektorWpis<T,H>::wstaw_kon() {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = 0;
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
				Wektor();
				~Wektor();
	void		zamien(uint32_t const&, uint32_t const&);
	void		rezerwuj(uint32_t const&);
	void		usun(uint32_t const&);
protected:
	T*			_tab_rob;
};
template<class T, class H>
Wektor<T,H>::Wektor()
	: _tab_rob((T*)malloc(_il_rezerw*_rozm_el)) {
}
template<class T, class H>
Wektor<T,H>::~Wektor() {
	free(_tab_rob);
}
template<class T, class H>
void Wektor<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	WektorWpis::rezerwuj(il);

	free(_tab_rob);
	_tab_rob = (T*)malloc(_il_rezerw*_rozm_el);
}
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
	inline T		wez();
};
template<class T, class H>
void WektorStos<T,H>::wstaw(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = el;
}
template<class T, class H>
T WektorStos<T,H>::wez() {
	return _tab[--_il];
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class Wektor : public WektorWpis<T,H> {
public:
	void					upakuj();
protected:
	WektorWpis<Segment>		_usuniete;
};
template<class T, class H>
void Wektor<T,H>::upakuj() {
	_usuniete.uloz_unikat();
	T* wsk1 = _tab_rob;
	T* wsk2 = _tab;
	uint32_t il;
	for(uint32_t i = 0; i < _usuniete.wez_il(); ++i) {
		il = (&_tab[_usuniete[i].pocz]) - wsk2;
		memove(wsk1, wsk2, il*_rozm_el);
		wsk1 += il;
		wsk2 += il + _usuniete[i].il;
	}
	_usuniete.czysc();
	_il = wsk1 - _tab_rob;

	wsk1 = _tab_rob;
	_tab_rob = _tab;
	_tab = wsk1;
}
// -------------------------------------------------------
template<class T, class H = FunHasz<T>>
class WektorSeg : public Wektor<T,H> {
#define pusty 4294967295
public:
	T&						operator[](uint32_t const&) const;
	uint32_t				wstaw_kon(T const*const&, uint32_t const&);
	//void					upakuj();
protected:
	Segment*				_seg;
	Wektor<uint32_t>		_mapa_seg_wolne;
	//uint32_t*				_mapa; // zamienia numer elementu na indeks w tablicy
};
template<class T, class H>
T& WektorSeg<T,H>::operator[](uint32_t const& nr) const {
	return _tab[_seg[nr].pocz];
}
template<class T, class H>
void WektorSeg<T,H>::upakuj() {
	_usuniete.uloz_unikat();
	uint32_t ind1 = 0, ind2 = 0, il, j, przes_akt = 0;
	uint32_t* przes = (uint32_t*)malloc(_il*4);
	bool* usuniete = (bool*)malloc(_il);
	ZeroMemory(usuniete, _il);
	for(uint32_t i = 0; i < _usuniete.wez_il(); ++i) {
		il = _usuniete[i].pocz - ind2;

		// kopiuj upakowane
		memove(&_tab_rob[ind1], &_tab[ind2], il*_rozm_el);
		
		// licz przesunięcia
		for(j = ind2; j < ind2+il+_usuniete[i].il; ++j) {
			przes[j] = przes_akt;
		}
		przes_akt += _usuniete[i].il;

		// oznacz usunięte
		memset(&usuniete[_usuniete[i].pocz], 1, _usuniete[i].il);
		
		ind1 += il;
		ind2 += il + _usuniete[i].il;
	}
	_usuniete.czysc();
	_il = ind1;

	// aktualizuj mapę
	for(j = 0; j < _il_rezerw; ++j) {
		_mapa[j] -= przes[_mapa[j]];
	}

	wsk1 = _tab_rob;
	_tab_rob = _tab;
	_tab = wsk1;
}
template<class T, class H>
uint32_t WektorSeg<T,H>::wstaw_kon(T const*const& t, uint32_t const& il = 1) {
	while(_il+il > _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_seg[_mapa_seg_wolne[0]] = {_il, il};
	_mapa_seg_wolne.zamien(0, _mapa_seg_wolne.wez_il()-1);
	for(uint32_t i = 0; i < il; ++i) {
		_tab[_il++] = t[i];
	}
	return 
}



