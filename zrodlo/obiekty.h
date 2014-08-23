#pragma once

#include <stdint.h>
#include <wekt.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <zasobyGraf.h>

using namespace DirectX;

class Obiekty3w {
	friend class Grafika;
	typedef ID3D11ShaderResourceView* Teks;
public:
	//							Obiekty3w();
	void						tworz_ob(
									XMFLOAT3*, XMFLOAT2*,
									uint32_t const&,
									DWORD*, uint32_t const&,
									uint32_t const&);
	//void						usun(uint32_t const&);
	//void						pakuj();
private:
	//char const*					wez_sciez_teks(uint32_t const&) const;
	Wek<uint32_t>				nr;
	WekSegPula<XMFLOAT3>		wierz;
	WekSegPula<XMFLOAT2>		wierz_teks;
	WekSegLuz<Teks>				teks;
	WekSegPula<DWORD>			ind;
	WekSegPula<XMFLOAT3>		przes;
};


