# pragma once

#include <stdint.h>
#include <operatory.h>

template<class T, class H = FunHasz<T>>
class Wektor {
	struct Segment {
		uint32_t		pocz;
		uint32_t		il;
	};
public:
						Wektor();
						~Wektor();
	inline T&			operator[](uint32_t const&);
	void				rezerwuj(uint32_t const&);
	inline uint32_t		wstaw_kon();
	inline void			wstaw_kon(T const&);
	inline void			wstaw_zakres_kon(T const*, uint32_t);
	inline void			licz_zakres(uint32_t&, uint32_t&) const;
	void				uloz();
	void				uloz_unikat();
	inline uint32_t		wez_il_rezerw() const;
	inline uint32_t		wez_il() const;
	void				usun(uint32_t, uint32_t);
	void				upakuj();
	inline void			czysc();
protected:
	uint8_t				_rozm_el;
	H					_f_hasz;
	uint32_t			_il_rezerw;
	uint32_t			_il;
	T*					_tab;
	T*					_tab_rob;
	Wektor<Segment>		_usuniete;
};
template<class T, class H>
Wektor<T,H>::Wektor()
	: _rozm_el(sizeof(T)),
	_il_rezerw(4096),
	_il(0),
	_tab((T*)malloc(_il_rezerw*_rozm_el)) {
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
void Wektor<T,H>::licz_zakres(uint32_t& hasz_min, uint32_t& hasz_maks) const {
	hasz_min = _f_hasz(tab[0]);
	hasz_maks = hasz_min;
	uint32_t hasz;
	for(uint32_t i = 1; i < _il; ++i) {
		hasz = _f_hasz(tab[i]);
		if(hasz < hasz_min) {
			hasz_min = hasz;
		} else if(hasz > hasz_maks) {
			hasz_maks = hasz;
		}
	}
}
template<class T, class H>
void Wektor<T,H>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;

	free(_tab_rob);
	_tab_rob = (T*)malloc(il*_rozm_el);
	memmove(_tab_rob, _tab, _il_rezerw*_rozm_el);
	free(_tab);
	_tab = _tab_rob;
	_tab_rob = (T*)malloc(il*_rozm_el);
	_il_rezerw = il;
}
template<class T, class H>
void Wektor<T,H>::uloz() {
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
void Wektor<T,H>::uloz_unikat() {
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
void Wektor<T,H>::upakuj() {
	_usuniete.uloz_unikat();
	T* wsk1 = _tab_rob;
	T* wsk2 = _tab;
	uint32_t il;
	for(uint32_t i = 0; i < _usuniete.wez_il(); ++i) {
		il = (&_tab[_usuniete[i].pocz])-wsk2;
		memmove(wsk1, wsk2, il*_rozm_el);
		wsk1 += il;
		wsk2 += il + _usuniete[i].il;
	}
	wsk1 = _tab_rob;
	_tab_rob = _tab;
	_tab = wsk1;
}
template<class T, class H>
void Wektor<T,H>::usun(uint32_t pocz, uint32_t il = 1) {
	_usuniete.wstaw_kon({pocz, il});
}
template<class T, class H>
uint32_t Wektor<T,H>::wez_il() const {
	return _il;
}
template<class T, class H>
uint32_t Wektor<T,H>::wez_il_rezerw() const {
	return _il_rezerw;
}
template<class T, class H>
uint32_t Wektor<T,H>::wstaw_kon() {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = 0;
}
template<class T, class H>
void Wektor<T,H>::wstaw_kon(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*2);
	}
	_tab[_il++] = el;
}
template<class T, class H>
void Wektor<T,H>::wstaw_zakres_kon(T const* tab, uint32_t il) {
	for(uint32_t i = 0; i < il; ++i) {
		wstaw_kon(tab[i]);
	}
}






