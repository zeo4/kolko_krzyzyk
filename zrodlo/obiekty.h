#pragma once

#include <stdint.h>
#include <wektor.h>

class Obiekty3w {
	typedef Para<ID3D11ShaderResourceView*, uint32_t>		TeksturaDane_;
public:
										Obiekty3w();
	void								tworz_ob(
											XMFLOAT3*, XMFLOAT2*,
											uint32_t const&,
											DWORD*, uint32_t const&,
											string const&);
	void								usun_zbierz(uint32_t const&);
	void								usun_wykonaj();
	void								aktual_buf() const;
private:
	char const*							wez_sciezka_tekstura(
											uint32_t const&) const;
	Wekt<uint32_t>						_nr;
	WektZachSeg<XMFLOAT3>				_wierz;
	ID3D11Buffer*						_wierz_buf;
	WektZachSeg<XMFLOAT2>				_tekstury_wsp;
	ID3D11Buffer*						_tekstury_wsp_buf;
	WektZachSeg<uint32_t>				_tekstury_mapa;
	TeksturaDane_*const					_tekstury_dane;
	WektZachSeg<DWORD>					_ind;
	ID3D11Buffer*						_ind_buf;
	WektZachSeg<XMFLOAT4X4>				_mac_swiat;
	WektZachSeg<XMFLOAT4X4>				_mac_widok;
	WektZachSeg<XMFLOAT4X4>				_mac_proj;
};


