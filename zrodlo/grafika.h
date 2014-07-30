#pragma once

#include "obiekty.h"
#include "zasobyGraf.h"
#include <stdint.h>

class Grafika {
public:
	void							przypisz(Obiekty3w const*);
	void							tworz_co_ob();
	void							aktual_co_ob(uint32_t const&);
	inline void						wiaz_co_ob() const;
	inline void						wiaz_ind() const;
	void							rysuj();
private:
	Obiekty3w const*				_ob;
	struct CoObDane {
								CoObDane();
		XMMATRIX				mac_swp;
	}		_co_ob_dane;
	ID3D11Buffer*					_co_ob_buf;
};



