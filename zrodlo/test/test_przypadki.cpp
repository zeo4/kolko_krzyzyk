#pragma once

#include <test_przypadki.h>
// -------------------------------------------------------
void TestWektor::inic() {
	uint32_t _il = 800;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		wstaw_kon((_il-1-_i)/10);
	}
}
void TestWektor::niszcz() {
	usun_kon(il);
}
void TestWektor::testUlozLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 1; _i < il; ++_i) {
		if(el[_i] != el[_i-1])
			UPEWNIJ_W(_mapa[_i-1], _mapa[_i]) << "_i=" << _i << "\n";
		else
			UPEWNIJ_M(_mapa[_i-1], _mapa[_i]) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWektor::testUlozLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(il-1);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	UPEWNIJ_R(_mapa[0], 0x80000000) << "\n";
	UPEWNIJ_R(_mapa[il-1], 0x80000000) << "\n";
	for(uint32_t _i = 1; _i < il-1; ++_i) {
		UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWektor::testUlozLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/5;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(_i < _od1) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWektor::testUlozLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWektor::testUlozUnikatLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind = 0x80000000;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		_ind = _i;
		break;
	}
	UPEWNIJ_NR(_ind, 0x80000000) << "\n";
	for(uint32_t _i = _ind+1; _i < il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		UPEWNIJ_NR(el[_ind], el[_i]) << "_ind=" << _ind << " _i=" << _i << "\n";
		UPEWNIJ_W(_mapa[_ind], _mapa[_i]) << "_ind=" << _ind << " _i=" << _i << "\n";
		_ind = _i;
	}
	niszcz();
}
void TestWektor::testUlozUnikatLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(il-1);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	UPEWNIJ_R(_mapa[0], 0x80000000) << "\n";
	UPEWNIJ_R(_mapa[il-1], 0x80000000) << "\n";
	uint32_t _ind = 1;
	for(uint32_t _i = 1; _i < il-1; ++_i) {
		if(el[_i] == el[_ind]) continue;
		UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	niszcz();
}
void TestWektor::testUlozUnikatLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/5;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind = 0;
	for(uint32_t _i = 0; _i < il; ++_i) {
		if(el[_ind] == el[_i]) continue;
		if(_i < _od1) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	niszcz();
}
void TestWektor::testUlozUnikatLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWektor::wykonaj() {
	testUlozLiczNieusunieteZadne();
	testUlozLiczUsunietePoczKon();
	testUlozLiczUsunieteSrodek();
	testUlozLiczUsunieteWszystkie();
	testUlozUnikatLiczNieusunieteZadne();
	testUlozUnikatLiczUsunietePoczKon();
	testUlozUnikatLiczUsunieteSrodek();
	testUlozUnikatLiczUsunieteWszystkie();
}
// -------------------------------------------------------
void main() {
	TestWektor().wykonaj();
}
// -------------------------------------------------------

