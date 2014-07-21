#pragma once

#include <stdint.h>
#include <wektor.h>

class Obiekty3w {
	struct Segment {
		uint32_t			pocz;
		uint32_t			il;
	};
	Wektor<XMFLOAT3>						_wierz;
	ID3D11Buffer*							_buf_wierz;
	Wektor<XMFLOAT2>						_wsp_tekstury;
	ID3D11Buffer*							_buf_wsp_tekstury;
	Wektor<Segment>							_mapa_wierz;
	Wektor<DWORD>							_indeksy;
	ID3D11Buffer*							_buf_indeksy;
	Wektor<Segment>							_mapa_indeksy;
	Wektor<ID3D11ShaderResourceView*>		_wid_tekstury;
public:
							Obiekty3w();
	void					tworz_ob(
								XMFLOAT3*, XMFLOAT2*, uint32_t,
								DWORD*, uint32_t const&,
								string
							);
	void					niszcz_ob(uint32_t const&);
};


