#pragma once

template<class T>
class Mniej {
public:
	inline bool		operator()(T const, T const) const;
};
template<class T>
bool Mniej<T>::operator()(T const ob1, T const ob2) const {
	return ob1 < ob2;
}


