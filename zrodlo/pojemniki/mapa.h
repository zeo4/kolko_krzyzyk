#pragma once

#include <pamiec_pula.h>
#include <operatory.h>

template<class K, class W>
class Dwojka {
public:
				Dwojka();
	K			klucz;
	W			wart;
	Dwojka*		przed;
	Dwojka*		po;
};
template<class K, class W>
Dwojka<K,W>::Dwojka() : przed(null), po(null) {
}

template<class K, class W, class M = Mniej<K>>
class IteratorDwojka {
	typedef Dwojka<K,W>		Dwojka_;
public:
						IteratorDwojka();
						IteratorDwojka(Dwojka_&);
						IteratorDwojka(Dwojka_*const);
	IteratorDwojka&		operator++();
	IteratorDwojka		operator++(int);
	IteratorDwojka&		operator--();
	IteratorDwojka		operator--(int);
	Dwojka_*&			operator->() const;
	Dwojka_&			operator*() const;
	bool				operator<(IteratorDwojka const&) const;
	bool				operator==(IteratorDwojka const&) const;
	bool				operator!=(IteratorDwojka const&) const;
	void				skocz_o(uint32_t const&);
private:
	M					_mniej;
	Dwojka_*			_wsk;
};
template<class K, class W, class M>
IteratorDwojka<K,W,M>::IteratorDwojka() : _wsk(null) {
}
template<class K, class W, class M>
IteratorDwojka<K,W,M>::IteratorDwojka(Dwojka_& d) : _wsk(&d) {
}
template<class K, class W, class M>
IteratorDwojka<K,W,M>::IteratorDwojka(Dwojka_*const d) : _wsk(d) {
}
template<class K, class W, class M>
IteratorDwojka<K,W,M>& IteratorDwojka<K,W,M>::operator++() {
	_wsk = _wsk->po;
	return *this;
}
template<class K, class W, class M>
IteratorDwojka<K,W,M> IteratorDwojka<K,W,M>::operator++(int) {
	IteratorDwojka it = _wsk;
	++*this;
	return it;
}
template<class K, class W, class M>
IteratorDwojka<K,W,M>& IteratorDwojka<K,W,M>::operator--() {
	_wsk = _wsk->przed;
	return *this;
}
template<class K, class W, class M>
IteratorDwojka<K,W,M> IteratorDwojka<K,W,M>::operator--(int) {
	IteratorDwojka it = _wsk;
	--*this;
	return it;
}
template<class K, class W, class M>
Dwojka<K,W>*& IteratorDwojka<K,W,M>::operator->() const {
	return _wsk;
}
template<class K, class W, class M>
Dwojka<K,W>& IteratorDwojka<K,W,M>::operator*() const {
	return *_wsk;
}
template<class K, class W, class M>
bool IteratorDwojka<K,W,M>::operator<(IteratorDwojka const& it) const {
	return _mniej(_wsk->klucz, it->klucz);
}
template<class K, class W, class M>
bool IteratorDwojka<K,W,M>::operator==(IteratorDwojka const& it) const {
	return _wsk == &*it;
}
template<class K, class W, class M>
bool IteratorDwojka<K,W,M>::operator!=(IteratorDwojka const& it) const {
	return _wsk != &*it;
}
template<class K, class W, class M>
void IteratorDwojka<K,W,M>::skocz_o(uint32_t const& il) {
	for(uint32_t i = 0; i < il; ++i) {
		_wsk = _wsk->po;
	}
}

template<class K, class W, class M = Mniej<K>, class MP = PamiecPula<Dwojka<K,W>>>
class Mapa {
	#define null 0
	typedef Dwojka<K,W>					Dwojka_;
public:
	typedef IteratorDwojka<K,W,M>		Iter_;
					Mapa();
	Iter_			wez_pocz() const;
	Iter_			wez_kon() const;
	uint32_t		wez_il() const;
	void			wstaw(K const&, W const&);
	void			usun(K const&);
	Iter_			znajdz(K const&) const;
private:
	Iter_			znajdz_za(K const&) const;
	M				_mniej;
	MP				_men_pam;
	Iter_			_pierwszy;
	Iter_ const		_ostatni;
	uint32_t		_il_el;
};
template<class K, class W, class M, class MP>
Mapa<K,W,M,MP>::Mapa() : _pierwszy(_men_pam.przydziel()), _ostatni(_pierwszy), _il_el(0) {
}
template<class K, class W, class M, class MP>
void Mapa<K,W,M,MP>::usun(K const& klucz) {

}
template<class K, class W, class M, class MP>
uint32_t Mapa<K,W,M,MP>::wez_il() const {
	return _il_el;
}
template<class K, class W, class M, class MP>
IteratorDwojka<K,W,M> Mapa<K,W,M,MP>::wez_kon() const {
	return _ostatni;
}
template<class K, class W, class M, class MP>
IteratorDwojka<K,W,M> Mapa<K,W,M,MP>::wez_pocz() const {
	return _pierwszy;
}
template<class K, class W, class M, class MP>
void Mapa<K,W,M,MP>::wstaw(K const& klucz, W const& wart) {
	Dwojka_* d = _men_pam.przydziel();
	d->klucz = klucz;
	d->wart = wart;

	if(_il_el == 0) {
		d->po = &*_pierwszy;
		_pierwszy->przed = d;
		--_pierwszy;
	} else if(_mniej(klucz, _pierwszy->klucz)) {
		d->po = &*_pierwszy;
		_pierwszy->przed = d;
		--_pierwszy;
	} else {
		Iter_ it = znajdz_za(klucz);
		d->przed = it->przed;
		d->po = &*it;
		d->przed->po = d;
		it->przed = d;
	}
	++_il_el;
}
template<class K, class W, class M, class MP>
IteratorDwojka<K,W,M> Mapa<K,W,M,MP>::znajdz(K const& klucz) const {
	if(_il_el == 0) {
		return _ostatni;
	}

	Iter_ pierw = _pierwszy, sr, ost = _ostatni;
	uint32_t il = _il_el, i;
	while(1) {
		if(il == 1) {
			if(_mniej(pierw->klucz, klucz)) {
				return ost;
			} else {
				return pierw;
			}
		}

		// ustaw środkowy
		sr = pierw;
		sr.skocz_o(il/2);

		// ustaw zakres
		if(_mniej(klucz, sr->klucz)) {
			ost = sr;
			il = il/2;
		} else {
			pierw = sr;
			il = il-il/2;
		}
	}
}
template<class K, class W, class M, class MP>
IteratorDwojka<K,W,M> Mapa<K,W,M,MP>::znajdz_za(K const& klucz) const {
	if(_il_el == 0) {
		return _ostatni;
	}

	Iter_ pierw = _pierwszy, sr, ost = _ostatni;
	uint32_t il = _il_el, i;
	while(1) {
		if(il == 1) {
			return ost;
		}

		// ustaw środkowy
		sr = pierw;
		sr.skocz_o(il/2);

		// ustaw zakres
		if(_mniej(klucz, sr->klucz)) {
			ost = sr;
			il = il/2;
		} else {
			pierw = sr;
			il = il-il/2;
		}
	}
}




