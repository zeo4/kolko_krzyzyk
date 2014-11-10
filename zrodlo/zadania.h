#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <wek.h>
using namespace DirectX;
// -------------------------------------------------------
enum KodZad {
	OB_POZ,
	OB_V,
	KAM_V,
	KAM_OBROT,
	WYB_OB,
	TWORZ_OB,
	USUN_OB,
	KAM_AKT_POZ,
	KAM_AKT,
	ULOZ_OB,
	AKTUAL_SWIAT,
	TWORZ_MAPA_ZASL,
	RYSUJ,
	DEFRAG_FIZ,
	DEFRAG_GRAF,
};
// -------------------------------------------------------
struct WynTworzOb {
	KodZad			kod_zad;
	uint32_t		uch_ob;
};
struct WynWybOb {
	KodZad			kod_zad;
	uint32_t		uch_ob;
};
struct Zad {
	KodZad			kod;
	uint32_t		el;
};
struct ZadTworzOb {
	KodZad			kod;
	uint32_t		el;
	uint32_t		uch_mod;
	uint32_t		uch_teks;
};
struct ZadObPoz {
	KodZad			kod;
	uint32_t		el;
	XMFLOAT3		poz;
};
struct ZadObV {
	KodZad			kod;
	uint32_t		el;
	XMFLOAT3		v;
};
struct ZadKamV {
	KodZad			kod;
	uint32_t		el;
	XMFLOAT3		v;
};
struct ZadKamObrot{
	KodZad			kod;
	uint32_t		el;
	XMFLOAT3		katy;
};
struct ZadWybOb {
	KodZad			kod;
	uint32_t		el;
	uint32_t		x;
	uint32_t		y;
};
// -------------------------------------------------------
struct Zadania {
	template<class T>
	void						wstaw_zad(T);
	static Wek2<uint8_t>		zad;
	static WekLuz<bool>			flg_zad;
	static Wek2<uint8_t>		wyn;
};
template<class T>
void Zadania::wstaw_zad(T _z) {
	zad.wstaw_kon((uint8_t*)&_z, sizeof(T));
	flg_zad[_z.kod] = 1;
}
// -------------------------------------------------------




