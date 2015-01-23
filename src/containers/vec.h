#pragma once

#include <stdint.h>
#include <operators.h>
// -------------------------------------------------------
template<class T>
class Vec {
public:
								Vec();
								Vec(T*const);
								Vec(T*const,
									uint32_t const&, uint32_t const&, uint32_t const&
								);
	inline T&					operator[](uint32_t const) const;
	void						reserve_back(uint32_t const&);
	void						reserve_front(uint32_t const&);
	inline void					destroy();
	inline void					push_back(T const&, uint32_t const& = 1);
	inline void					erase(uint32_t const&, uint32_t const& = 1);
	template<class H = FunHasz<T>>
	void						erase_dupl_comp(uint32_t*&, H = H()) const;
	template<class H1, class H2>
	void						erase_dupl_comp(uint32_t*&, H1, H2) const;
	inline void					pop_back(uint32_t const& = 1);
	inline void					swap(uint32_t const&, uint32_t const&);
	inline uint32_t const&		get_size() const;
	inline uint32_t const&		get_first_empty() const;
	void						defrag_comp(uint32_t*&, uint32_t const&);
	void						defrag_exe(uint32_t const*const);
	template<class H = FunHasz<T>>
	void						sort_comp(uint32_t*&, H = H()) const;
	template<class H = FunHasz<T>>
	void						sort_uni_comp(uint32_t*&, H = H()) const;
	void						sort_exe(uint32_t const*const);
	void						update(uint32_t const*const);
	inline void					update_header();
	static T const				empty;
protected:
	template<class H>
	void						comp_range(uint32_t&, uint32_t&, H) const;
	uint32_t					cap;
	uint32_t					size;
	uint32_t					first_empty;
	T*							el;
};
template<class T>
Vec<T>::Vec()
	: el((T*)((uint8_t*)malloc(256 * sizeof(T) + 12) + 12)),
	cap(256),
	size(0),
	first_empty(0x80000000) {}
template<class T>
Vec<T>::Vec(T*const _mem) {
	memcpy(this, _mem, 12);
	el = (T*)((uint8_t*)_mem + 12);
}
template<class T>
Vec<T>::Vec(T*const _el, uint32_t const& _cap, uint32_t const& _size, uint32_t const& _pierw_pusty)
	: el(_el),
	cap(_cap),
	size(_size),
	first_empty(_pierw_pusty) {}
template<class T>
T& Vec<T>::operator[](uint32_t const _no) const {
	return el[_no];
}
template<class T>
void Vec<T>::update(uint32_t const*const _map) {
	for(uint32_t _i = 0; _i < size; ++_i) {
		if(el[_i] == empty) continue;
		if(_map[el[_i]] == 0x80000000) continue;
		el[_i] = _map[el[_i]];
	}
}
template<class T>
void Vec<T>::update_header() {
	memcpy((uint8_t*)el - 12, this, 12);
}
template<class T>
void Vec<T>::defrag_comp(uint32_t*& _map, uint32_t const& _size) {
	free(_map);
	_map = (uint32_t*)malloc(size*4);
	wyp_pam(_map, 0x80000000, size);

	if(size < 2 || first_empty == 0x80000000) return;

	uint32_t _n = 0, _pierw_pusty = first_empty, _ind_pelny = _pierw_pusty;
	while(_n < _size && _ind_pelny < size-1) {
		if(memcmp(&el[++_ind_pelny], &empty, sizeof(T)) == 0) continue;
		_map[_ind_pelny] = _pierw_pusty++;
		++_n;
	}
}
template<class T>
void Vec<T>::defrag_exe(uint32_t const*const _map) {
	if(first_empty == 0x80000000) return;
	for(uint32_t _i = 0; _i < size; ++_i) {
		if(_map[_i] == 0x80000000) continue;
		swap(_i, _map[_i]);
		first_empty++;
	}
	for(int32_t _i = size-1; _i >= 0; --_i) {
		if(memcmp(&el[_i], &empty, sizeof(T)) != 0) break;
		--size;
	}
	if(first_empty >= size) first_empty = 0x80000000;
}
template<class T> template<class H>
void Vec<T>::comp_range(uint32_t& _hasz_min, uint32_t& _hasz_max, H _comp_hasz) const {
	_hasz_min = 0x80000000;
	_hasz_max = 0;
	uint32_t _hasz;
	for(uint32_t _i = 0; _i < size; ++_i) {
		if(el[_i] == empty) continue;
		_hasz = _comp_hasz(el[_i]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		if(_hasz > _hasz_max) _hasz_max = _hasz;
	}
	if(_hasz_min == 0x80000000) _hasz_min = 1;
}
template<class T>
void Vec<T>::destroy() {
	free((uint8_t*)el - 12);
}
template<class T>
void Vec<T>::reserve_front(uint32_t const& _cap) {
	if(_cap <= cap) return;

	T*const _el = (T*const)((uint8_t*)malloc(_cap * sizeof(T) + 12) + 12);
	memcpy(_el+_cap-cap, el, cap * sizeof(T));
	free((uint8_t*)el - 12);
	el = _el;
	size += _cap - cap;
	cap = _cap;
}
template<class T>
void Vec<T>::reserve_back(uint32_t const& _cap) {
	if(_cap <= cap) return;

	T*const _el = (T*const)((uint8_t*)malloc(_cap * sizeof(T) + 12) + 12);
	memcpy(_el, el, cap * sizeof(T));
	free((uint8_t*)el - 12);
	el = _el;
	cap = _cap;
}
template<class T> template<class H>
void Vec<T>::sort_comp(uint32_t*& _map, H _comp_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_max, _hasz_il;
	comp_range(_hasz_min, _hasz_max, _comp_hasz);
	_hasz_il = _hasz_max - _hasz_min + 1;

	// licz hasze elementów i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(size*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < size; ++_i) {
		if(el[_i] == empty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _comp_hasz(el[_i]) - _hasz_min; // tablica hasze elementów
			++_t2[_t1[_i]]; // tablica powtórzenia haszy
		}
	}

	// licz indeksy haszy
	uint32_t*const _t3 = (uint32_t*const)malloc(_hasz_il*4);
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		_t3[_i] = _ind; // tablica indeksy haszy
		_ind += _t2[_i];
	}

	// licz indeksy elementów
	memset(_t2, 0, _hasz_il*4); // tablica ilość wpisanych elementów haszy
	uint32_t _hasz;
	for(_i = 0; _i < size; ++_i) {
		_hasz = _t1[_i];
		if(_hasz == 0x80000000) continue;
		_t1[_i] = _t3[_hasz] + _t2[_hasz]; // tablica indeksy elementów
		++_t2[_hasz];
	}

	free(_map);
	free(_t2);
	free(_t3);
	_map = _t1;
}
template<class T> template<class H>
void Vec<T>::sort_uni_comp(uint32_t*& _map, H _comp_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_max, _hasz_il;
	comp_range(_hasz_min, _hasz_max, _comp_hasz);
	_hasz_il = _hasz_max - _hasz_min + 1;

	// licz hasze elementów i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(size*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < size; ++_i) {
		if(el[_i] == empty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _comp_hasz(el[_i]) - _hasz_min; // tablica hasze elementów
			if(_t2[_t1[_i]] == 0) ++_t2[_t1[_i]]; // tablica powtórzenia haszy
			else _t1[_i] = 0x80000000;
		}
	}

	// licz indeksy haszy
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		if(_t2[_i] == 0) continue;
		_t2[_i] = _ind; // tablica indeksy haszy
		++_ind;
	}

	// licz indeksy elementów
	for(_i = 0; _i < size; ++_i) {
		if(_t1[_i] == 0x80000000) continue;
		_t1[_i] = _t2[_t1[_i]]; // tablica indeksy elementów
	}

	free(_map);
	free(_t2);
	_map = _t1;
}
template<class T>
void Vec<T>::sort_exe(uint32_t const*const _map) {
	T*const _el = (T*const)malloc(size * sizeof(T));
	uint32_t _size = 0;
	for(uint32_t _i = 0; _i < size; ++_i) {
		if(_map[_i] == 0x80000000) continue;
		_el[_map[_i]] = el[_i];
		++_size;
	}
	memcpy(el, _el, _size * sizeof(T));
	size = _size;
	first_empty = 0x80000000;
	free(_el);
}
template<class T>
void Vec<T>::erase(uint32_t const& _no, uint32_t const& _size) {
	wyp_pam(&el[_no], empty, _size);
	if(_no < first_empty) first_empty = _no;
}
template<class T> template<class H>
void Vec<T>::erase_dupl_comp(uint32_t*& _map, H _comp_hasz) const {
	// licz zakres haszy
	uint32_t _hasz_min, _hasz_max, _il_hasz;
	comp_range(_hasz_min, _hasz_max, _comp_hasz);
	_il_hasz = _hasz_max - _hasz_min + 1;

	// licz mapę usunięcia duplikatów
	bool* _t1 = (bool*)malloc(_il_hasz * sizeof(bool)); // wystąpienia haszy
	memset(_t1, 0, _il_hasz * sizeof(bool));
	free(_map);
	_map = (uint32_t*)malloc(size * 4); // mapa usunięcia duplikatów
	uint32_t _ind = 0, _hasz;
	for(uint32_t _i = 0; _i < size; ++_i) {
		if(el[_i] == empty) {
			_map[_i] = 0x80000000;
			continue;
		}
		_hasz = _comp_hasz(el[_i]) - _hasz_min;
		if(_t1[_hasz]) {
			_map[_i] = 0x80000000;
			continue;
		}
		_t1[_hasz] = true;
		_map[_i] = _ind++;
	}

	free(_t1);
}
template<class T> template<class H1, class H2>
void Vec<T>::erase_dupl_comp(uint32_t*& _map, H1 _comp_hasz1, H2 _comp_hasz2) const {
	free(_map);
	_map = (uint32_t*)malloc(size * 4);
	uint32_t _hasz_poprz1, _hasz_nast1, _hasz_poprz2, _hasz_nast2, _i = 0, _ind = 0;
	while(el[_i] == empty) {
		_map[_i] = 0x80000000;
		 ++_i;
	}
	_hasz_poprz1 = _comp_hasz1(el[_i]);
	_hasz_poprz2 = _comp_hasz2(el[_i]);
	_map[_i] = _ind++;
	for(++_i; _i < size; ++_i) {
		if(el[_i] == empty) {
			_map[_i] = 0x80000000;
			continue;
		}
		_hasz_nast1 = _comp_hasz1(el[_i]);
		_hasz_nast2 = _comp_hasz2(el[_i]);
		if(_hasz_poprz1 == _hasz_nast1) {
			if(_hasz_poprz2 == _hasz_nast2) {
				_map[_i] = 0x80000000;
			} else {
				_hasz_poprz2 = _hasz_nast2;
				_map[_i] = _ind++;
			}
		} else {
			_hasz_poprz1 = _hasz_nast1;
			_hasz_poprz2 = _hasz_nast2;
			_map[_i] = _ind++;
		}
	}
}
template<class T>
void Vec<T>::pop_back(uint32_t const& _size) {
	if(size <= _size) size = 0;
	else size -= _size;
}
template<class T>
uint32_t const& Vec<T>::get_size() const {
	return size;
}
template<class T>
uint32_t const& Vec<T>::get_first_empty() const {
	return first_empty;
}
template<class T>
void Vec<T>::push_back(T const& _el, uint32_t const& _size) {
	while(size+_size > cap) reserve_back(cap*2);
	memmove(&el[size], &_el, _size*sizeof(T));
	size += _size;
}
template<class T>
void Vec<T>::swap(uint32_t const& _nr1, uint32_t const& _nr2) {
	T _el = el[_nr1];
	el[_nr1] = el[_nr2];
	el[_nr2] = _el;
}
template<class T>
T const Vec<T>::empty = gen_empty<T>();
// -------------------------------------------------------
template<class T>
class VecSparse {
public:
						VecSparse();
						VecSparse(T*const);
						VecSparse(T*const, uint32_t const&);
	inline T&			operator[](uint32_t const&);
	inline uint32_t		get_cap() const;
	void				reserve_back(uint32_t const&);
	inline void			destroy();
	void				update(uint32_t const*const&);
	inline void			update_header();
	void				clear();
	static T const		empty;
protected:
	uint32_t			cap;
	T*					el;
};
template<class T>
VecSparse<T>::VecSparse()
	: el((T*)((uint8_t*)malloc(256 * sizeof(T) + 12) + 12)),
	cap(256) {
	clear();
}
template<class T>
VecSparse<T>::VecSparse(T*const _mem) {
	memcpy(this, _mem, 4);
	el = (T*)((uint8_t*)_mem + 12);
}
template<class T>
VecSparse<T>::VecSparse(T*const _el, uint32_t const& _cap)
	: el(_el),
	cap(_cap) {}
template<class T>
T& VecSparse<T>::operator[](uint32_t const& _no) {
	while(_no >= cap) reserve_back(2 * cap);
	return el[_no];
}
template<class T>
void VecSparse<T>::update(uint32_t const*const& _map) {
	for(uint32_t _i = 0; _i < cap; ++_i) {
		if(el[_i] == empty) continue;
		if(_map[el[_i]] == 0x80000000) continue;
		el[_i] = _map[el[_i]];
	}
}
template<class T>
void VecSparse<T>::update_header() {
	memcpy((uint8_t*)el - 4, this, 4);
}
template<class T>
void VecSparse<T>::clear() {
	wyp_pam(el, empty, cap);
}
template<class T>
void VecSparse<T>::destroy() {
	free((uint8_t*)el - 12);
}
template<class T>
void VecSparse<T>::reserve_back(uint32_t const& _cap) {
	if(_cap <= cap) return;

	T*const _el = (T*)((uint8_t*)malloc(_cap * sizeof(T) + 12) + 12);
	memcpy(_el, el, cap * sizeof(T));
	wyp_pam(_el + cap, empty, _cap - cap);
	free((uint8_t*)el - 12);
	el = _el;
	cap = _cap;
}
template<class T>
uint32_t VecSparse<T>::get_cap() const {
	return cap;
}
template<class T>
T const VecSparse<T>::empty = gen_empty<T>();
// -------------------------------------------------------
template<>
void VecSparse<bool>::update(uint32_t const*const&);
template<>
void VecSparse<bool>::clear();
template<>
void VecSparse<bool>::reserve_back(uint32_t const&);
// -------------------------------------------------------
template<class A, class B = A>
struct Pair {
	inline bool		operator==(Pair const&) const;
	inline bool		operator!=(Pair const&) const;
	A				first;
	B				second;
};
template<class A, class B>
bool Pair<A,B>::operator==(Pair const& _pair) const {
	return (first == _pair.first && second == _pair.second);
}
template<class A, class B>
bool Pair<A,B>::operator!=(Pair const& _pair) const {
	return !operator==(_pair);
}
// -------------------------------------------------------
template<class A, class B>
std::ostream& operator<<(std::ostream& _stream, Pair<A,B> const& _pair) {
	_stream << "{" << _pair.first << ", " << _pair.second << "}";
	return _stream;
}
// -------------------------------------------------------
template<class T>
class Vec2 {
public:
	typedef Pair<uint32_t> Row_;
	inline T*const&				operator[](uint32_t const&) const;
	inline void					destroy();
	inline void					push_back(T const*const, uint32_t const);
	inline void					push_back(Vec2<T>const&);
	inline void					erase(uint32_t const&, uint32_t = 1);
	template<class H = FunHasz<T>>
	void						erase_dupl_comp(uint32_t*&, H = H()) const;
	template<class H1, class H2>
	void						erase_dupl_comp(uint32_t*&, H1, H2) const;
	inline void					pop_back(uint32_t = 1);
	inline Row_ const&			get_row(uint32_t const&) const;
	inline uint32_t const&		get_col_size() const;
	inline uint32_t const&		get_size() const;
	void						defrag_comp(uint32_t*&, uint32_t*&, uint32_t const);
	void						defrag_exe(uint32_t const*const,
									uint32_t const*const);
	template<class H = FunHasz<T>>
	void						sort_comp(uint32_t*&, H = H()) const;
	template<class H = FunHasz<T>>
	void						sort_uni_comp(uint32_t*&, H = H()) const;
	void						sort_exe(uint32_t const*const);
	void						update(uint32_t const*const);
	inline void					update_header();
	static Row_ const			empty;
protected:
	template<class H>
	void						comp_range(uint32_t&, uint32_t&, H) const;
	Vec<Row_>					rows;
	Vec<T>						el;
};
template<class T>
T*const& Vec2<T>::operator[](uint32_t const& _no) const {
	return &el[rows[_no].first];
}
template<class T>
void Vec2<T>::update(uint32_t const*const _map) {
	uint32_t _j;
	for(uint32_t _i = 0; _i < rows.get_size(); ++_i) {
		if(rows[_i] == rows.empty) continue;
		if(_map[el[rows[_i].pierw]] == 0x80000000) continue;
		wyp_pam(&el[rows[_i].pierw], _map[el[rows[_i].pierw]], rows[_i].drug);
	}
}
template<class T>
void Vec2<T>::update_header() {
	rows.update_header();
	el.update_header();
}
template<class T>
void Vec2<T>::defrag_comp(uint32_t*& _rows_map, uint32_t*& _el_map, uint32_t const _il_wier) {
	// licz ile elementów odpowiada podanej ilości wierszy
	uint32_t _il_el = 0, _licz = _il_wier;
	for(uint32_t _ind = rows.get_first_empty();
		_ind < rows.get_size() && _licz > 0;
		++_ind) {
		if(rows[_ind] == rows.empty) continue;
		_il_el += rows[_ind].second;
		--_licz;
	}

	rows.defrag_comp(_rows_map, _il_wier);
	el.defrag_comp(_el_map, _il_el);
}
template<class T>
void Vec2<T>::defrag_exe(uint32_t const*const _rows_map, uint32_t const*const _el_map) {
	rows.defrag_exe(_rows_map);
	el.defrag_exe(_el_map);
}
template<class T> template<class H>
void Vec2<T>::comp_range(uint32_t& _hasz_min, uint32_t& _hasz_max, H _comp_hasz) const {
	_hasz_min = 0x80000000;
	_hasz_max = 0;
	uint32_t _hasz;
	for(uint32_t _i = 0; _i < rows.get_size(); ++_i) {
		if(rows[_i] == empty) continue;
		_hasz = _comp_hasz(el[rows[_i].first]);
		if(_hasz < _hasz_min) _hasz_min = _hasz;
		if(_hasz > _hasz_max) _hasz_max = _hasz;
	}
	if(_hasz_min == 0x80000000) _hasz_min = 1;
}
template<class T>
void Vec2<T>::destroy() {
	rows.destroy();
	el.destroy();
}
template<class T> template<class H>
void Vec2<T>::sort_comp(uint32_t*& _rows_map, H _comp_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_max, _hasz_il;
	comp_range(_hasz_min, _hasz_max, _comp_hasz);
	_hasz_il = _hasz_max - _hasz_min + 1;

	// licz hasze wierszy i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(rows.get_size()*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < rows.get_size(); ++_i) {
		if(rows[_i] == empty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _comp_hasz(el[rows[_i].first]) - _hasz_min; // tablica hasze wierszy
			++_t2[_t1[_i]]; // tablica powtórzenia haszy
		}
	}

	// licz indeksy haszy
	uint32_t*const _t3 = (uint32_t*const)malloc(_hasz_il*4);
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		_t3[_i] = _ind; // tablica indeksy haszy
		_ind += _t2[_i];
	}

	// licz indeksy wierszy
	memset(_t2, 0, _hasz_il*4); // tablica ilość wpisanych wierszy haszy
	uint32_t _hasz;
	for(_i = 0; _i < rows.get_size(); ++_i) {
		_hasz = _t1[_i];
		if(_hasz == 0x80000000) continue;
		_t1[_i] = _t3[_hasz] + _t2[_hasz]; // tablica indeksy wierszy
		++_t2[_hasz];
	}

	free(_rows_map);
	free(_t2);
	free(_t3);
	_rows_map = _t1;
}
template<class T> template<class H>
void Vec2<T>::sort_uni_comp(uint32_t*& _rows_map, H _comp_hasz) const {
	uint32_t _i;

	// licz zakres haszy
	uint32_t _hasz_min, _hasz_max, _hasz_il;
	comp_range(_hasz_min, _hasz_max, _comp_hasz);
	_hasz_il = _hasz_max - _hasz_min + 1;

	// licz hasze wierszy i powtórzenia każdego hasza
	uint32_t*const _t1 = (uint32_t*const)malloc(rows.get_size()*4);
	uint32_t*const _t2 = (uint32_t*const)malloc(_hasz_il*4);
	memset(_t2, 0, _hasz_il*4);
	for(_i = 0; _i < rows.get_size(); ++_i) {
		if(rows[_i] == empty) _t1[_i] = 0x80000000;
		else {
			_t1[_i] = _comp_hasz(el[rows[_i].pierw]) - _hasz_min; // tablica hasze wierszy
			if(_t2[_t1[_i]] == 0) ++_t2[_t1[_i]]; // tablica powtórzenia haszy
			else _t1[_i] = 0x80000000;
		}
	}

	// licz indeksy haszy
	uint32_t _ind = 0;
	for(_i = 0; _i < _hasz_il; ++_i) {
		if(_t2[_i] == 0) continue;
		_t2[_i] = _ind; // tablica indeksy haszy
		++_ind;
	}

	// licz indeksy wierszy
	for(_i = 0; _i < rows.get_size(); ++_i) {
		if(_t1[_i] == 0x80000000) continue;
		_t1[_i] = _t2[_t1[_i]]; // tablica indeksy wierszy
	}

	free(_rows_map);
	free(_t2);
	_rows_map = _t1;
}
template<class T>
void Vec2<T>::sort_exe(uint32_t const*const _rows_map) {
	rows.sort_exe(_rows_map);

	// twórz mapę elementów, aktualizuj wiersze
	uint32_t*const _el_map = (uint32_t*const)malloc(el.get_size() * 4);
	wyp_pam(_el_map, 0x80000000, el.get_size());
	uint32_t _i, _j, _ind = 0;
	for(_i = 0; _i < rows.get_size(); ++_i) {
		for(_j = rows[_i].first; _j < rows[_i].first + rows[_i].second; ++_j) {
			_el_map[_j] = _ind++;
		}
		rows[_i].first = _ind - rows[_i].second;
	}

	el.sort_exe(_el_map);
}
template<class T>
void Vec2<T>::erase(uint32_t const& _no, uint32_t _size) {
	for(uint32_t _i = 0; _i < _size; ++_i) {
		el.erase(rows[_no+_i].first, rows[_no+_i].second);
		rows.erase(_no+_i);
	}
}
template<class T> template<class H>
void Vec2<T>::erase_dupl_comp(uint32_t*& _map, H _comp_hasz) const {
	// licz zakres haszy
	uint32_t _hasz_min, _hasz_max, _il_hasz;
	comp_range(_hasz_min, _hasz_max, _comp_hasz);
	_il_hasz = _hasz_max - _hasz_min + 1;

	// licz mapę usunięcia duplikatów
	bool* _t1 = (bool*)malloc(_il_hasz * sizeof(bool)); // wystąpienia haszy
	memset(_t1, 0, _il_hasz * sizeof(bool));
	free(_map);
	_map = (uint32_t*)malloc(rows.get_size() * 4); // mapa usunięcia duplikatów
	uint32_t _ind = 0, _hasz;
	for(uint32_t _i = 0; _i < rows.get_size(); ++_i) {
		if(rows[_i] == empty) {
			_map[_i] = 0x80000000;
			continue;
		}
		_hasz = _comp_hasz(el[rows[_i].pierw]) - _hasz_min;
		if(_t1[_hasz]) {
			_map[_i] = 0x80000000;
			continue;
		}
		_t1[_hasz] = true;
		_map[_i] = _ind++;
	}

	free(_t1);
}
template<class T> template<class H1, class H2>
void Vec2<T>::erase_dupl_comp(uint32_t*& _map, H1 _comp_hasz1, H2 _comp_hasz2) const {
	free(_map);
	_map = (uint32_t*)malloc(rows.get_size() *4);
	uint32_t _hasz_poprz1, _hasz_nast1, _hasz_poprz2, _hasz_nast2, _i = 0, _ind = 0;
	while(rows[_i] == empty) {
		_map[_i] = 0x80000000;
		++_i;
	}
	_hasz_poprz1 = _comp_hasz1(el[rows[_i].first]);
	_hasz_poprz2 = _comp_hasz2(el[rows[_i].first]);
	_map[_i] = _ind++;
	for(++_i; _i < rows.get_size(); ++_i) {
		if(rows[_i] == empty) {
			_map[_i] = 0x80000000;
			continue;
		}
		_hasz_nast1 = _comp_hasz1(el[rows[_i].first]);
		_hasz_nast2 = _comp_hasz2(el[rows[_i].first]);
		if(_hasz_poprz1 == _hasz_nast1) {
			if(_hasz_poprz2 == _hasz_nast2) {
				_map[_i] = 0x80000000;
			} else {
				_hasz_poprz2 = _hasz_nast2;
				_map[_i] = _ind++;
			}
		} else {
			_hasz_poprz1 = _hasz_nast1;
			_hasz_poprz2 = _hasz_nast2;
			_map[_i] = _ind++;
		}
	}
}
template<class T>
void Vec2<T>::pop_back(uint32_t _size) {
	if(_size > rows.get_size()) _size = rows.get_size();
	for(uint32_t _i = 0; _i < _size; ++_i) {
		el.pop_back(rows[rows.get_size()-1].second);
		rows.pop_back();
	}
}
template<class T>
uint32_t const& Vec2<T>::get_size() const {
	return el.get_size();
}
template<class T>
uint32_t const& Vec2<T>::get_col_size() const {
	return rows.get_size();
}
template<class T>
typename Vec2<T>::Row_ const& Vec2<T>::get_row(uint32_t const& _no) const {
	return rows[_no];
}
template<class T>
void Vec2<T>::push_back(T const*const _el, uint32_t const _size) {
	if(_size == 0) return;
	rows.push_back({el.get_size(), _size});
	el.push_back(*_el, _size);
}
template<class T>
void Vec2<T>::push_back(Vec2<T>const& _vec2) {
	rows.push_back(_vec2.get_row(0), _vec2.get_col_size());
	el.push_back(*_vec2[0], _vec2.wez_il_el());
}
template<class T>
typename Vec2<T>::Row_ const Vec2<T>::empty = gen_empty<Row_>();
// -------------------------------------------------------



