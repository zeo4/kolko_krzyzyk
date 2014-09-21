#pragma once

#include <test_przypadki.h>
#include <debug.h>
// -------------------------------------------------------
void TestWektor::inic() {
	uint32_t _il = 800;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		wstaw_kon((_il-1-_i)/5);
	}
}
void TestWektor::inicMalo() {
	uint32_t _il = 10;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		wstaw_kon(_i);
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
	free(_mapa);
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
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/10;
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
	free(_mapa);
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
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < il; ++_i) {
		UPEWNIJ_MR(el[_i-1], el[_i]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[0], _kon = el[il-1];
	uint32_t* _mapa = 0;
	usun(0);
	usun(il-1);
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_MR(el[_i], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[_i], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/10;
	uint32_t* _ciag1 = new uint32_t[_il];
	uint32_t* _ciag2 = new uint32_t[_il];
	memcpy(_ciag1, &el[_od1], _il*4);
	memcpy(_ciag2, &el[_od2], _il*4);
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < il-_il+1; ++_i) {
		UPEWNIJ_NR(memcmp(_ciag1, &el[_i], _il*4), 0) << "_i=" << _i << "\n";
		UPEWNIJ_NR(memcmp(_ciag2, &el[_i], _il*4), 0) << "_i=" << _i << "\n";
	}
	free(_ciag1);
	free(_ciag2);
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(il, 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUnikatLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind;
	for(_ind = 0; _ind < il; ++_ind) {
		if(_mapa[_ind] == 0x80000000) continue;
		break;
	}
	UPEWNIJ_M(_ind, il) << "\n";
	for(uint32_t _i = _ind+1; _i < il; ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		UPEWNIJ_NR(el[_ind], el[_i]) << "_ind=" << _ind << " _i=" << _i << "\n";
		UPEWNIJ_W(_mapa[_ind], _mapa[_i]) << "_ind=" << _ind << " _i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
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
	for(uint32_t _i = 2; _i < il-1; ++_i) {
		if(el[_ind] == el[_i])
			UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else
			UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUnikatLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/10;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind = 0;
	for(uint32_t _i = 1; _i < il; ++_i) {
		if(el[_ind] == el[_i]) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
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
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUnikatNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < il; ++_i) {
		UPEWNIJ_M(el[_i-1], el[_i]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUnikatUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[0], _kon = el[il-1];
	uint32_t* _mapa = 0;
	usun(0);
	usun(il-1);
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_MR(el[_i], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[_i], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUnikatUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/10;
	uint32_t* _ciag1 = new uint32_t[_il];
	uint32_t* _ciag2 = new uint32_t[_il];
	memcpy(_ciag1, &el[_od1], _il*4);
	memcpy(_ciag2, &el[_od2], _il*4);
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < il-_il+1; ++_i) {
		UPEWNIJ_NR(memcmp(_ciag1, &el[_i], _il*4), 0) << "_i=" << _i << "\n";
		UPEWNIJ_NR(memcmp(_ciag2, &el[_i], _il*4), 0) << "_i=" << _i << "\n";
	}
	free(_ciag1);
	free(_ciag2);
	free(_mapa);
	niszcz();
}
void TestWektor::testUlozUnikatUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(il, 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor::testDefragPocz() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = 0, _ind2 = il/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa = 0;
	for(_ind1; _ind1 < il-2; ++_ind1) {
		for(uint32_t _i = 0; _i < il; ++_i) {
			if(_i == _ind1) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 < _ind2 && _i == _ind2) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 >= _ind2 && _i == _ind1+1) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(el[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa, 1);
		defrag_wyk(_mapa);
	}
	UPEWNIJ_R(il, 8) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor::testDefragSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = il/4, _ind2 = il/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa = 0;
	for(_ind1; _ind1 < il-2; ++_ind1) {
		for(uint32_t _i = 0; _i < il; ++_i) {
			if(_i == _ind1) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 < _ind2 && _i == _ind2) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 >= _ind2 && _i == _ind1+1) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(el[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa, 1);
		defrag_wyk(_mapa);
	}
	UPEWNIJ_R(il, 8) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor::testDefragSrodekLaczony() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = il/2-1, _ind2 = il/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa = 0;
	for(_ind1; _ind1 < il-2; ++_ind1) {
		for(uint32_t _i = 0; _i < il; ++_i) {
			if(_i == _ind1) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else if(_i == _ind1+1) UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(el[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa, 1);
		defrag_wyk(_mapa);
	}
	UPEWNIJ_R(il, 8) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor::testDefragKon() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _il = il;
	usun(il-1);
	uint32_t* _mapa = 0;
	defrag_licz(_mapa, 1);
	defrag_wyk(_mapa);
	UPEWNIJ_R(il, _il-1) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor::testDefragCalosc() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = il/4, _ind2 = il/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa = 0;
	defrag_licz(_mapa, 2*il);
	defrag_wyk(_mapa);
	UPEWNIJ_R(il, 8) << "\n";
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_NR(el[_i], pusty) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor::wykonaj() {
	testUlozLiczNieusunieteZadne();
	testUlozLiczUsunietePoczKon();
	testUlozLiczUsunieteSrodek();
	testUlozLiczUsunieteWszystkie();
	testUlozNieusunieteZadne();
	testUlozUsunietePoczKon();
	testUlozUsunieteSrodek();
	testUlozUsunieteWszystkie();
	testUlozUnikatLiczNieusunieteZadne();
	testUlozUnikatLiczUsunietePoczKon();
	testUlozUnikatLiczUsunieteSrodek();
	testUlozUnikatLiczUsunieteWszystkie();
	testUlozUnikatNieusunieteZadne();
	testUlozUnikatUsunietePoczKon();
	testUlozUnikatUsunieteSrodek();
	testUlozUnikatUsunieteWszystkie();
	testDefragPocz();
	testDefragSrodek();
	testDefragSrodekLaczony();
	testDefragKon();
	testDefragCalosc();
}
// -------------------------------------------------------
void TestWektor2::inic() {
	uint32_t _il_unikat = 160;
	uint32_t _il_powt = 5;
	uint32_t t[] = {0, 5, 4, 3, 2, 1};
	for(uint32_t _i = 0; _i < _il_unikat; ++_i) {
		t[0] = _il_unikat-1-_i;
		for(uint32_t _j = 0; _j < _il_powt; ++_j) {
			wstaw_kon(t, _il_powt-_j+1);
		}
	}
}
void TestWektor2::inicMalo() {
	uint32_t _il_unikat = 10;
	uint32_t _il_powt = 5;
	uint32_t t[] = {0, 5, 4, 3, 2, 1};
	for(uint32_t _i = 0; _i < _il_unikat; ++_i) {
		t[0] = _il_unikat-1-_i;
		for(uint32_t _j = 0; _j < _il_powt; ++_j) {
			wstaw_kon(t, _il_powt-_j+1);
		}
	}
}
void TestWektor2::niszcz() {
	usun_kon(wier.wez_il());
}
void TestWektor2::testUlozLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 1; _i < wier.wez_il(); ++_i) {
		if(el[wier[_i-1].pierw] == el[wier[_i].pierw])
			UPEWNIJ_M(_mapa[_i-1], _mapa[_i]) << "_i=" << _i << "\n";
		else
			UPEWNIJ_W(_mapa[_i-1], _mapa[_i]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(wez_il()-1);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	UPEWNIJ_R(_mapa[0], 0x80000000) << "\n";
	UPEWNIJ_R(_mapa[wez_il()-1], 0x80000000) << "\n";
	for(uint32_t _i = 1; _i < wez_il()-1; ++_i) {
		UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = wez_il()*1/4;
	_od2 = wez_il()*3/4;
	_il = wez_il()/10;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 0; _i < wez_il(); ++_i) {
		if(_i < _od1) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il());
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 0; _i < wez_il(); ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < wez_il(); ++_i) {
		UPEWNIJ_MR(el[wier[_i-1].pierw], el[wier[_i].pierw]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[wier[0].pierw], _kon = el[wier[wez_il()-1].pierw];
	uint32_t* _mapa = 0;
	usun(0);
	usun(wez_il()-1);
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < wez_il(); ++_i) {
		UPEWNIJ_MR(el[wier[_i].pierw], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[wier[_i].pierw], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il, _il1 = 0, _il2 = 0;
	_od1 = wez_il()*1/4;
	_od2 = wez_il()*3/4;
	_il = wez_il()/10;
	for(uint32_t _i = _od1; _i < _od1+_il; ++_i) {
		_il1 += wier[_i].drug;
	}
	for(uint32_t _i = _od2; _i < _od2+_il; ++_i) {
		_il2 += wier[_i].drug;
	}
	uint32_t* _ciag1 = new uint32_t[_il1];
	uint32_t* _ciag2 = new uint32_t[_il2];
	memcpy(_ciag1, &el[wier[_od1].pierw], _il1*4);
	memcpy(_ciag2, &el[wier[_od2].pierw], _il2*4);
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < el.wez_il()-_il1+1; ++_i) {
		UPEWNIJ_NR(memcmp(_ciag1, &el[_i], _il1*4), 0) << "_i=" << _i << "\n";
	}
	for(uint32_t _i = 0; _i < el.wez_il()-_il2+1; ++_i) {
		UPEWNIJ_NR(memcmp(_ciag2, &el[_i], _il2*4), 0) << "_i=" << _i << "\n";
	}
	free(_ciag1);
	free(_ciag2);
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il());
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 0) << "\n";
	UPEWNIJ_R(el.wez_il(), 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind;
	for(_ind = 0; _ind < wier.wez_il(); ++_ind) {
		if(_mapa[_ind] == 0x80000000) continue;
		break;
	}
	UPEWNIJ_M(_ind, wier.wez_il()) << "\n";
	for(uint32_t _i = _ind+1; _i < wier.wez_il(); ++_i) {
		if(_mapa[_i] == 0x80000000) continue;
		UPEWNIJ_NR(el[wier[_ind].pierw], el[wier[_i].pierw]) << "_ind=" << _ind << " _i=" << _i << "\n";
		UPEWNIJ_W(_mapa[_ind], _mapa[_i]) << "_ind=" << _ind << " _i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(wez_il()-1);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	UPEWNIJ_R(_mapa[0], 0x80000000) << "\n";
	UPEWNIJ_R(_mapa[wez_il()-1], 0x80000000) << "\n";
	uint32_t _ind = 1;
	for(uint32_t _i = 2; _i < wez_il()-1; ++_i) {
		if(el[wier[_ind].pierw] == el[wier[_i].pierw])
			UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else
			UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = wez_il()*1/4;
	_od2 = wez_il()*3/4;
	_il = wez_il()/10;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uint32_t _ind = 0;
	for(uint32_t _i = 1; _i < wez_il(); ++_i) {
		if(el[wier[_ind].pierw] == el[wier[_i].pierw]) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il());
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	for(uint32_t _i = 0; _i < wez_il(); ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < wez_il(); ++_i) {
		UPEWNIJ_M(el[wier[_i-1].pierw], el[wier[_i].pierw]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[wier[0].pierw], _kon = el[wier[wez_il()-1].pierw];
	uint32_t* _mapa = 0;
	usun(0);
	usun(wez_il()-1);
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < wez_il(); ++_i) {
		UPEWNIJ_MR(el[wier[_i].pierw], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[wier[_i].pierw], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il, _il1 = 0, _il2 = 0;
	_od1 = wez_il()*1/4;
	_od2 = wez_il()*3/4;
	_il = wez_il()/10;
	for(uint32_t _i = _od1; _i < _od1+_il; ++_i) {
		_il1 += wier[_i].drug;
	}
	for(uint32_t _i = _od2; _i < _od2+_il; ++_i) {
		_il2 += wier[_i].drug;
	}
	uint32_t* _ciag1 = new uint32_t[_il1];
	uint32_t* _ciag2 = new uint32_t[_il2];
	memcpy(_ciag1, &el[wier[_od1].pierw], _il1*4);
	memcpy(_ciag2, &el[wier[_od2].pierw], _il2*4);
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < el.wez_il()-_il1+1; ++_i) {
		UPEWNIJ_NR(memcmp(_ciag1, &el[_i], _il1*4), 0) << "_i=" << _i << "\n";
	}
	for(uint32_t _i = 0; _i < el.wez_il()-_il2+1; ++_i) {
		UPEWNIJ_NR(memcmp(_ciag2, &el[_i], _il2*4), 0) << "_i=" << _i << "\n";
	}
	free(_ciag1);
	free(_ciag2);
	free(_mapa);
	niszcz();
}
void TestWektor2::testUlozUnikatUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il());
	uint32_t* _mapa = 0;
	uloz_unikat_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 0) << "\n";
	UPEWNIJ_R(el.wez_il(), 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWektor2::testDefragPocz() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = 0, _ind2 = wez_il()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	for(_ind1; _ind1 < wez_il()-2; ++_ind1) {
		for(uint32_t _i = 0; _i < wez_il(); ++_i) {
			if(_i == _ind1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 < _ind2 && _i == _ind2) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 >= _ind2 && _i == _ind1+1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa_wier, _mapa_el, 1);
		defrag_wyk(_mapa_wier, _mapa_el);
	}
	UPEWNIJ_R(wez_il(), 48) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWektor2::testDefragSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = wez_il()/4, _ind2 = wez_il()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	for(_ind1; _ind1 < wez_il()-2; ++_ind1) {
		for(uint32_t _i = 0; _i < wez_il(); ++_i) {
			if(_i == _ind1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 < _ind2 && _i == _ind2) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 >= _ind2 && _i == _ind1+1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa_wier, _mapa_el, 1);
		defrag_wyk(_mapa_wier, _mapa_el);
	}
	UPEWNIJ_R(wez_il(), 48) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWektor2::testDefragSrodekLaczony() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = wez_il()/2-1, _ind2 = wez_il()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	for(_ind1; _ind1 < wez_il()-2; ++_ind1) {
		for(uint32_t _i = 0; _i < wez_il(); ++_i) {
			if(_i == _ind1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_i == _ind1+1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa_wier, _mapa_el, 1);
		defrag_wyk(_mapa_wier, _mapa_el);
	}
	UPEWNIJ_R(wez_il(), 48) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWektor2::testDefragKon() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _il = wez_il();
	usun(wez_il()-1);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	defrag_licz(_mapa_wier, _mapa_el, 1);
	defrag_wyk(_mapa_wier, _mapa_el);
	UPEWNIJ_R(wez_il(), _il-1) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWektor2::testDefragCalosc() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = wez_il()/4, _ind2 = wez_il()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	defrag_licz(_mapa_wier, _mapa_el, 2*wez_il());
	defrag_wyk(_mapa_wier, _mapa_el);
	UPEWNIJ_R(wez_il(), 48) << "\n";
	for(uint32_t _i = 0; _i < wez_il(); ++_i) {
		UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
	}
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWektor2::wykonaj() {
	testUlozLiczNieusunieteZadne();
	testUlozLiczUsunietePoczKon();
	testUlozLiczUsunieteSrodek();
	testUlozLiczUsunieteWszystkie();
	testUlozNieusunieteZadne();
	testUlozUsunietePoczKon();
	testUlozUsunieteSrodek();
	testUlozUsunieteWszystkie();
	testUlozUnikatLiczNieusunieteZadne();
	testUlozUnikatLiczUsunietePoczKon();
	testUlozUnikatLiczUsunieteSrodek();
	testUlozUnikatLiczUsunieteWszystkie();
	testUlozUnikatNieusunieteZadne();
	testUlozUnikatUsunietePoczKon();
	testUlozUnikatUsunieteSrodek();
	testUlozUnikatUsunieteWszystkie();
	testDefragPocz();
	testDefragSrodek();
	testDefragSrodekLaczony();
	testDefragKon();
	testDefragCalosc();
}
// -------------------------------------------------------
void main() {
	TestWektor().wykonaj();
	TestWektor2().wykonaj();
	std::cout << "Sukces! Nacisnij ENTER";
	std::cin.get();
}
// -------------------------------------------------------

