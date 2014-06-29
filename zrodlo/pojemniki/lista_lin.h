#pragma once

#include <pamiec_lin.h>
#include <operatory.h>
#include <iteratory_wezel.h>

template<class T, class M = Mniej<T>, class P = PamiecLin<Wezel<T>>>
class ListaLin {
public:
	typedef IterWezel<T,M>		Iter_;
						ListaLin();
	Iter_				wez_pocz() const;
	Iter_				wez_kon() const;
	void				wstaw_kon(T const&);
private:
	P					_pamiec;
	Wezel<T>*			_pierwszy;
	Wezel<T>* const		_ostatni;
};
template<class T, class M, class P>
ListaLin<T,M,P>::ListaLin() : _pierwszy(_pamiec.przydziel()), _ostatni(_pierwszy) {
}
template<class T, class M, class P>
IterWezel<T,M> ListaLin<T,M,P>::wez_pocz() const {
	return *_pierwszy;
}
template<class T, class M, class P>
IterWezel<T,M> ListaLin<T,M,P>::wez_kon() const {
	return *_ostatni;
}
template<class T, class M, class P>
void ListaLin<T,M,P>::wstaw_kon(T const& wart) {
	Wezel<T>* w = _pamiec.przydziel();
	w->wart = wart;
	w->przed = _ostatni->przed;
	w->po = _ostatni;
	_ostatni->przed = w;

	if(_pierwszy == _ostatni) {
		_pierwszy = w;
	} else {
		w->przed->po = w;
	}
}





