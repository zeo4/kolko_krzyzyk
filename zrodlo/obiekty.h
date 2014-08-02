#pragma once

#include <stdint.h>
#include <wekt.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <zasobyGraf.h>

using namespace DirectX;

class Obiekty3w {
	friend class Grafika;
	typedef Para<ID3D11ShaderResourceView*, uint32_t>		TeksturaDane_;
public:
										Obiekty3w();
	void								tworz_ob(
											XMFLOAT3*, XMFLOAT2*,
											uint32_t const&,
											DWORD*, uint32_t const&,
											uint32_t const&);
	void								usun_zbierz(uint32_t const&);
	void								usun_wyk();
private:
	char const*							wez_sciez_teks(
											uint32_t const&) const;
	Wekt<uint32_t>						_nr;
	WektZachSeg<XMFLOAT3>				_wierz;
	WektZachSeg<XMFLOAT2>				_wierz_teks;
	WektZachSeg<uint32_t>				_teks_mapa;
	TeksturaDane_*const					_teks_dane;
	WektZachSeg<DWORD>					_ind;
	WektZachSeg<XMFLOAT3>				_przes;
};


