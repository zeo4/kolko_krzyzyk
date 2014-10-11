#pragma once
#include <DirectXMath.h>
#include <stdint.h>
#include <wek.h>
using namespace DirectX;
// -------------------------------------------------------
enum KodZad {
	USTAW_OB,
	TWORZ_OB,
	WYB_OB,
	WYKR_KOL,
	RYSUJ,
};
// -------------------------------------------------------
struct ZadUstawOb {
	KodZad			kod_zad;
	uint32_t		uch_ob;
	XMFLOAT3		w;
};
// -------------------------------------------------------
struct Zadania {
	static Wek2<uint8_t>		zad;
};
// -------------------------------------------------------



