#pragma once

#include <stdint.h>
#include <wektor.h>

class Obiekty3w {
	Wektor<uint32_t>								_nr_obiekty;
	WektorZachSeg<XMFLOAT3>							_wierz;
	ID3D11Buffer*									_wierz_buf;
	WektorZachSeg<XMFLOAT2>							_tekstury_wsp;
	ID3D11Buffer*									_tekstury_wsp_buf;
	WektorZachSeg<ID3D11ShaderResourceView*>		_tekstury_wid;
	WektorZachSeg<DWORD>							_ind;
	ID3D11Buffer*									_ind_buf;
public:
													Obiekty3w();
	void											tworz_ob(
														XMFLOAT3*, XMFLOAT2*,
														uint32_t const&,
														DWORD*, uint32_t const&,
														string const&
													);
	void											niszcz_ob(uint32_t const&);
};


