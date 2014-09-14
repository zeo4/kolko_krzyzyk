#pragma once

#include <test_przypadki.h>
// -------------------------------------------------------
void TestWektor::inic() {
	test_il = 800;
	for(uint32_t _i = 0; _i < test_il; ++_i) {
		wstaw_kon((test_il-1-_i)/10);
	}
}
void TestWektor::niszcz() {
	usun_kon(test_il);
}
void TestWektor::testUlozLiczNieusunieteZadne() {
	inic();
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 1; _i < test_il; ++_i) {
		if(el[_i] != el[_i-1])
			UPEWNIJ_W(_mapa[_i-1], _mapa[_i]) << "\n";
		else
			UPEWNIJ_M(_mapa[_i-1], _mapa[_i]) << "\n";
	}
	niszcz();
}
void TestWektor::testUlozUnikatLiczNieusunieteZadne() {
	inic();
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind = 0x80000000;
	for(uint32_t _i = 0; _i < test_il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		_ind = _i;
		break;
	}
	UPEWNIJ_NR(_ind, 0x80000000) << "\n";
	for(uint32_t _i = _ind+1; _i < test_il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		UPEWNIJ_NR(el[_ind], el[_i]) << "\n";
		UPEWNIJ_W(_mapa[_ind], _mapa[_i]) << "\n";
		_ind = _i;
	}
	niszcz();
}
void TestWektor::wykonaj() {
	testUlozLiczNieusunieteZadne();
	testUlozUnikatLiczNieusunieteZadne();
}
// -------------------------------------------------------
void main() {
	TestWektor().wykonaj();
}
// -------------------------------------------------------

