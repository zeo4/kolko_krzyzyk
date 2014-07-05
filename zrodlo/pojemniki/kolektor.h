# pragma once

#include <stdint.h>

template<class T>
class Kolektor {
public:
					Kolektor();
					~Kolektor();
	void			rezerwuj(uint32_t const&);
	inline void		wstaw_kon(T const&);
	inline void		czysc();
private:
	uint8_t			_rozm_el;
	uint32_t		_il_rezerw;
	T*				_pierwszy;
	T*				_pierwszy_wolny;
};
template<class T>
Kolektor<T>::Kolektor() : _rozm_el(sizeof(T)), _il_rezerw(4096), _pierwszy((T*)malloc(_il_rezerw*_rozm_el)), _pierwszy_wolny(_pierwszy) {
}
template<class T>
Kolektor<T>::~Kolektor() {
	free(_pierwszy);
}
template<class T>
void Kolektor<T>::czysc() {
	_pierwszy_wolny = _pierwszy;
}
template<class T>
void Kolektor<T>::rezerwuj(uint32_t const& il) {
	if(il <= _il_rezerw) return;
	T*const pam = (T*)malloc(il*_rozm_el);
	memmove(pam, _pierwszy, _il_rezerw*_rozm_el);
	_pierwszy_wolny = (T*)((char*)_pierwszy_wolny + ((char*)pam-(char*)_pierwszy));
	free(_pierwszy);
	_pierwszy = pam;
	_il_rezerw = il;
}
template<class T>
void Kolektor<T>::wstaw_kon(T const& el) {
	if(_pierwszy_wolny-_pierwszy == _il_rezerw) {
		rezerwuj(_il_rezerw*4);
	}
	*(_pierwszy_wolny++) = el;
}



