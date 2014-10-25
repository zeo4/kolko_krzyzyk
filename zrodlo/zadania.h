#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <wek.h>
using namespace DirectX;
// -------------------------------------------------------
enum KodZad {
	USTAW_OB,
	WYB_OB,
	TWORZ_OB,
	USUN_OB,
	WYKR_KOL,
	AKTUAL_KAM,
	AKTUAL_SWIAT,
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
struct ZadUstawOb {
	KodZad			kod;
	uint32_t		el;
	XMFLOAT3		poz;
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




