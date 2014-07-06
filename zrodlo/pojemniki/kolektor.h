# pragma once

#include <stdint.h>

template<class T>
class Kolektor {
public:
						Kolektor();
						~Kolektor();
	void				rezerwuj(uint32_t const&);
	inline void			wstaw_kon(T const&);
	inline T*			wez_pocz() const;
	inline uint32_t		wez_il() const;
	inline void			czysc();
protected:
	uint8_t				_rozm_el;
	uint32_t			_il_rezerw;
	uint32_t			_il;
	T*					_tab;
};
template<class T>
Kolektor<T>::Kolektor() : _rozm_el(sizeof(T)), _il_rezerw(4096), _il(0), _tab((T*)malloc(_il_rezerw*_rozm_el)) {
}
template<class T>
Kolektor<T>::~Kolektor() {
	free(_tab);
}
template<class T>
void Kolektor<T>::czysc() {
	_il = 0;
}
template<class T>
void Kolektor<T>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;
	T*const pam = (T*)malloc(il*_rozm_el);
	memmove(pam, _tab, _il_rezerw*_rozm_el);
	free(_tab);
	_tab = pam;
	_il_rezerw = il;
}
template<class T>
uint32_t Kolektor<T>::wez_il() const {
	return _il;
}
template<class T>
T* Kolektor<T>::wez_pocz() const {
	return _tab;
}
template<class T>
void Kolektor<T>::wstaw_kon(T const& el) {
	if(_il == _il_rezerw) {
		rezerwuj(_il_rezerw*4);
	}
	_tab[_il++] = el;
}



