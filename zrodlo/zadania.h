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
	WYKR_KOL,
	RYSUJ,
};
// -------------------------------------------------------
struct ZadUstawOb {
	KodZad			kod_zad;
	uint32_t		uch_ob;
	XMFLOAT3		poz;
};
struct ZadWybOb {
	KodZad			kod_zad;
	uint32_t		x;
	uint32_t		y;
};
struct WynWybOb {
	KodZad			kod_zad;
	uint32_t		uch_ob;
};
// -------------------------------------------------------
struct Zadania {
	static Wek2<uint8_t>		zad;
	static Wek2<uint8_t>		wyn;
};
// -------------------------------------------------------




