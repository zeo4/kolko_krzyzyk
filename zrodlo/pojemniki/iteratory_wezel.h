#pragma once

#include <operatory.h>

template<class T>
class Wezel {
public:
				Wezel();
	T			wart;
	Wezel*		przed;
	Wezel*		po;
};
template<class T>
Wezel<T>::Wezel() : przed(0), po(0) {
}

template<class T, class M = Mniej<K>>
class IterWezel {
public:
					IterWezel();
					IterWezel(Wezel<T>&);
	IterWezel&		operator++();
	IterWezel		operator++(int);
	IterWezel&		operator--();
	IterWezel		operator--(int);
	Wezel<T>*&		operator->() const;
	Wezel<T>&		operator*() const;
	bool			operator<(IterWezel const&) const;
	bool			operator==(IterWezel const&) const;
	bool			operator!=(IterWezel const&) const;
private:
	M				_mniej;
	Wezel<T>*		_wezel;
};
template<class T, class M>
IterWezel<T,M>::IterWezel() : _wezel(0) {
}
template<class T, class M>
IterWezel<T,M>::IterWezel(Wezel<T>& w) : _wezel(&w) {
}
template<class T, class M>
IterWezel<T,M>& IterWezel<T,M>::operator++() {
	_wezel = _wezel->po;
	return *this;
}
template<class T, class M>
IterWezel<T,M> IterWezel<T,M>::operator++(int) {
	IterWezel it = _wezel;
	_wezel = _wezel->po;
	return it;
}
template<class T, class M>
IterWezel<T,M>& IterWezel<T,M>::operator--() {
	_wezel = _wezel->przed;
	return *this;
}
template<class T, class M>
IterWezel<T,M> IterWezel<T,M>::operator--(int) {
	IterWezel it = _wezel;
	_wezel = _wezel->przed;
	return it;
}
template<class T, class M>
Wezel<T>*& IterWezel<T,M>::operator->() const {
	return _wezel;
}
template<class T, class M>
Wezel<T>& IterWezel<T,M>::operator*() const {
	return *_wezel;
}
template<class T, class M>
bool IterWezel<T,M>::operator<(IterWezel const& it) const {
	return _mniej(_wezel->wart, it._wezel->wart);
}
template<class T, class M>
bool IterWezel<T,M>::operator==(IterWezel const& it) const {
	return _wezel == it._wezel;
}
template<class T, class M>
bool IterWezel<T,M>::operator!=(IterWezel const& it) const {
	return _wezel != it._wezel;
}


