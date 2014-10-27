#pragma once

#include <test_przypadki.h>
#include <debug.h>
// -------------------------------------------------------
void TestWek::inic() {
	uint32_t _il = 800;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		wstaw_kon((_il-1-_i)/5);
	}
}
void TestWek::inicMalo() {
	uint32_t _il = 10;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		wstaw_kon(_i);
	}
}
void TestWek::niszcz() {
	usun_kon(il);
}
void TestWek::testUlozLiczNieusunieteZadne() {
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
void TestWek::testUlozLiczUsunietePoczKon() {
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
void TestWek::testUlozLiczUsunieteSrodek() {
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
void TestWek::testUlozLiczUsunieteWszystkie() {
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
void TestWek::testUlozNieusunieteZadne() {
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
void TestWek::testUlozUsunietePoczKon() {
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
void TestWek::testUlozUsunieteSrodek() {
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
void TestWek::testUlozUsunieteWszystkie() {
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
void TestWek::testUlozUnikatLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
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
void TestWek::testUlozUnikatLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(il-1);
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
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
void TestWek::testUlozUnikatLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = il*1/4;
	_od2 = il*3/4;
	_il = il/10;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
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
void TestWek::testUlozUnikatLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, il);
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek::testUlozUnikatNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < il; ++_i) {
		UPEWNIJ_M(el[_i-1], el[_i]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek::testUlozUnikatUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[0], _kon = el[il-1];
	uint32_t* _mapa = 0;
	usun(0);
	usun(il-1);
	uloz_uni_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_MR(el[_i], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[_i], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek::testUlozUnikatUsunieteSrodek() {
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
	uloz_uni_licz(_mapa);
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
void TestWek::testUlozUnikatUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, il);
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(il, 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek::testDefragLiczZdefrag() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t* _mapa = 0;
	defrag_licz(_mapa, il);
	for(uint32_t _i = 0; _i < il; ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	UPEWNIJ_R(il, 10) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek::testDefragPocz() {
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
void TestWek::testDefragSrodek() {
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
void TestWek::testDefragSrodekLaczony() {
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
void TestWek::testDefragKon() {
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
void TestWek::testDefragCalosc() {
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
void TestWek::testUsunDupl() {
	std::cout << __FUNCTION__ << "\n";
	wstaw_kon(pusty);
	wstaw_kon(1);
	wstaw_kon(1);
	wstaw_kon(pusty);
	wstaw_kon(2);
	wstaw_kon(pusty);
	wstaw_kon(2);
	wstaw_kon(pusty);
	wstaw_kon(pusty);
	wstaw_kon(3);
	wstaw_kon(3);
	wstaw_kon(1);
	wstaw_kon(pusty);
	uint32_t* _mapa = 0;
	usun_dupl_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(il, 3) << "\n";
	UPEWNIJ_R(el[0], 1) << "\n";
	UPEWNIJ_R(el[1], 2) << "\n";
	UPEWNIJ_R(el[2], 3) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek::wykonaj() {
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
	testDefragLiczZdefrag();
	testDefragPocz();
	testDefragSrodek();
	testDefragSrodekLaczony();
	testDefragKon();
	testDefragCalosc();
	testUsunDupl();
}
// -------------------------------------------------------
void TestWek_Para::niszcz() {
	usun_kon(il);
}
void TestWek_Para::testUsunDuplPoczKonPelny() {
	std::cout << __FUNCTION__ << "\n";
	wstaw_kon({1, 1});
	wstaw_kon({1, 1});
	wstaw_kon({1, 2});
	wstaw_kon({1, 1});
	wstaw_kon(pusty);
	wstaw_kon(pusty);
	wstaw_kon({1, 1});
	wstaw_kon({2, 1});
	wstaw_kon(pusty);
	wstaw_kon({3, 1});
	wstaw_kon({3, 2});
	wstaw_kon({3, 2});
	wstaw_kon({3, 1});
	uint32_t* _mapa = 0;
	usun_dupl_licz(_mapa, FunHasz<Para<uint32_t>>(), FunHasz2<Para<uint32_t>>());
	uloz_wyk(_mapa);
	UPEWNIJ_R(il, 7) << "\n";
	UPEWNIJ_R(el[0], Para<uint32_t>({1, 1})) << "\n";
	UPEWNIJ_R(el[1], Para<uint32_t>({1, 2})) << "\n";
	UPEWNIJ_R(el[2], Para<uint32_t>({1, 1})) << "\n";
	UPEWNIJ_R(el[3], Para<uint32_t>({2, 1})) << "\n";
	UPEWNIJ_R(el[4], Para<uint32_t>({3, 1})) << "\n";
	UPEWNIJ_R(el[5], Para<uint32_t>({3, 2})) << "\n";
	UPEWNIJ_R(el[6], Para<uint32_t>({3, 1})) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek_Para::testUsunDuplPoczKonPusty() {
	std::cout << __FUNCTION__ << "\n";
	wstaw_kon(pusty);
	wstaw_kon({1, 1});
	wstaw_kon({1, 1});
	wstaw_kon({1, 2});
	wstaw_kon({1, 1});
	wstaw_kon(pusty);
	wstaw_kon(pusty);
	wstaw_kon({1, 1});
	wstaw_kon({2, 1});
	wstaw_kon(pusty);
	wstaw_kon({3, 1});
	wstaw_kon({3, 2});
	wstaw_kon({3, 2});
	wstaw_kon({3, 1});
	wstaw_kon(pusty);
	uint32_t* _mapa = 0;
	usun_dupl_licz(_mapa, FunHasz<Para<uint32_t>>(), FunHasz2<Para<uint32_t>>());
	uloz_wyk(_mapa);
	UPEWNIJ_R(il, 7) << "\n";
	UPEWNIJ_R(el[0], Para<uint32_t>({1, 1})) << "\n";
	UPEWNIJ_R(el[1], Para<uint32_t>({1, 2})) << "\n";
	UPEWNIJ_R(el[2], Para<uint32_t>({1, 1})) << "\n";
	UPEWNIJ_R(el[3], Para<uint32_t>({2, 1})) << "\n";
	UPEWNIJ_R(el[4], Para<uint32_t>({3, 1})) << "\n";
	UPEWNIJ_R(el[5], Para<uint32_t>({3, 2})) << "\n";
	UPEWNIJ_R(el[6], Para<uint32_t>({3, 1})) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek_Para::wykonaj() {
	testUsunDuplPoczKonPelny();
	testUsunDuplPoczKonPusty();
}
// -------------------------------------------------------
void TestWekLuz::inic() {
	uint32_t _il = 10;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		if((_i + 1) % 3) el[_i] = _il-1-_i;
	}
}
void TestWekLuz::niszcz() {
	uint32_t _il = 10;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		el[_i] = pusty;
	}
}
void TestWekLuz::testAktualWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	aktual(_mapa);
	for(uint32_t _i = 0; _i < 10; ++_i) {
		if((_i + 1) % 3) UPEWNIJ_R(el[_i], _i) << "_i=" << _i << "\n";
		else UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWekLuz::testAktualPoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[10];
	for(uint32_t _i = 1; _i < 10-1; ++_i) _mapa[_i] = 0x80000000;
	_mapa[0] = 9;
	_mapa[9] = 0;
	aktual(_mapa);
	UPEWNIJ_R(el[0], 0) << "\n";
	UPEWNIJ_R(el[9], 9) << "\n";
	for(uint32_t _i = 1; _i < 10-1; ++_i) {
		if((_i + 1) % 3) UPEWNIJ_R(el[_i], 9-_i) << "_i=" << _i << "\n";
		else UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWekLuz::testAktualSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[10];
	_mapa[0] = 0x80000000;
	_mapa[9] = 0x80000000;
	for(uint32_t _i = 1; _i < 10-1; ++_i) _mapa[_i] = 9-_i;
	aktual(_mapa);
	UPEWNIJ_R(el[0], 9) << "\n";
	UPEWNIJ_R(el[9], 0) << "\n";
	for(uint32_t _i = 1; _i < 10-1; ++_i) {
		if((_i + 1) % 3) UPEWNIJ_R(el[_i], _i) << "_i=" << _i << "\n";
		else UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWekLuz::testAktualZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[10];
	for(uint32_t _i = 0; _i < 10; ++_i) _mapa[_i] = 0x80000000;
	aktual(_mapa);
	for(uint32_t _i = 0; _i < 10; ++_i) {
		if((_i + 1) % 3) UPEWNIJ_R(el[_i], 9-_i) << "_i=" << _i << "\n";
		else UPEWNIJ_R(el[_i], pusty) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestWekLuz::wykonaj() {
	testAktualWszystkie();
	testAktualPoczKon();
	testAktualSrodek();
	testAktualZadne();
}
// -------------------------------------------------------
void TestWek2::inic() {
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
void TestWek2::inicMalo() {
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
void TestWek2::niszcz() {
	usun_kon(wier.wez_il());
}
void TestWek2::testUlozLiczNieusunieteZadne() {
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
void TestWek2::testUlozLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(wez_il_wier()-1);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	UPEWNIJ_R(_mapa[0], 0x80000000) << "\n";
	UPEWNIJ_R(_mapa[wez_il_wier()-1], 0x80000000) << "\n";
	for(uint32_t _i = 1; _i < wez_il_wier()-1; ++_i) {
		UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = wez_il_wier()*1/4;
	_od2 = wez_il_wier()*3/4;
	_il = wez_il_wier()/10;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		if(_i < _od1) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od1+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2) UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else if(_i < _od2+_il) UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il_wier());
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_MR(el[wier[_i-1].pierw], el[wier[_i].pierw]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[wier[0].pierw], _kon = el[wier[wez_il_wier()-1].pierw];
	uint32_t* _mapa = 0;
	usun(0);
	usun(wez_il_wier()-1);
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_MR(el[wier[_i].pierw], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[wier[_i].pierw], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il, _il1 = 0, _il2 = 0;
	_od1 = wez_il_wier()*1/4;
	_od2 = wez_il_wier()*3/4;
	_il = wez_il_wier()/10;
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
void TestWek2::testUlozUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il_wier());
	uint32_t* _mapa = 0;
	uloz_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 0) << "\n";
	UPEWNIJ_R(el.wez_il(), 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUnikatLiczNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
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
void TestWek2::testUlozUnikatLiczUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0);
	usun(wez_il_wier()-1);
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	UPEWNIJ_R(_mapa[0], 0x80000000) << "\n";
	UPEWNIJ_R(_mapa[wez_il_wier()-1], 0x80000000) << "\n";
	uint32_t _ind = 1;
	for(uint32_t _i = 2; _i < wez_il_wier()-1; ++_i) {
		if(el[wier[_ind].pierw] == el[wier[_i].pierw])
			UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		else
			UPEWNIJ_NR(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
		_ind = _i;
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUnikatLiczUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il;
	_od1 = wez_il_wier()*1/4;
	_od2 = wez_il_wier()*3/4;
	_il = wez_il_wier()/10;
	usun(_od1, _il);
	usun(_od2, _il);
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	uint32_t _ind = 0;
	for(uint32_t _i = 1; _i < wez_il_wier(); ++_i) {
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
void TestWek2::testUlozUnikatLiczUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il_wier());
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_R(_mapa[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUnikatNieusunieteZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 1; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_M(el[wier[_i-1].pierw], el[wier[_i].pierw]) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUnikatUsunietePoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _pocz = el[wier[0].pierw], _kon = el[wier[wez_il_wier()-1].pierw];
	uint32_t* _mapa = 0;
	usun(0);
	usun(wez_il_wier()-1);
	uloz_uni_licz(_mapa);
	uloz_wyk(_mapa);
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_MR(el[wier[_i].pierw], _pocz) << "_i=" << _i << "\n";
		UPEWNIJ_WR(el[wier[_i].pierw], _kon) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::testUlozUnikatUsunieteSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _od1, _od2, _il, _il1 = 0, _il2 = 0;
	_od1 = wez_il_wier()*1/4;
	_od2 = wez_il_wier()*3/4;
	_il = wez_il_wier()/10;
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
	uloz_uni_licz(_mapa);
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
void TestWek2::testUlozUnikatUsunieteWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	usun(0, wez_il_wier());
	uint32_t* _mapa = 0;
	uloz_uni_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 0) << "\n";
	UPEWNIJ_R(el.wez_il(), 0) << "\n";
	free(_mapa);
	niszcz();
}
void TestWek2::testDefragLiczZdefrag() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	defrag_licz(_mapa_wier, _mapa_el, wez_il_wier());
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_R(_mapa_wier[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	for(uint32_t _i = 0; _i < el.wez_il(); ++_i) {
		UPEWNIJ_R(_mapa_el[_i], 0x80000000) << "_i=" << _i << "\n";
	}
	UPEWNIJ_R(wez_il_wier(), 50) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWek2::testDefragPocz() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = 0, _ind2 = wez_il_wier()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	for(_ind1; _ind1 < wez_il_wier()-2; ++_ind1) {
		for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
			if(_i == _ind1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 < _ind2 && _i == _ind2) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 >= _ind2 && _i == _ind1+1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa_wier, _mapa_el, 1);
		defrag_wyk(_mapa_wier, _mapa_el);
	}
	UPEWNIJ_R(wez_il_wier(), 48) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWek2::testDefragSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = wez_il_wier()/4, _ind2 = wez_il_wier()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	for(_ind1; _ind1 < wez_il_wier()-2; ++_ind1) {
		for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
			if(_i == _ind1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 < _ind2 && _i == _ind2) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_ind1 >= _ind2 && _i == _ind1+1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa_wier, _mapa_el, 1);
		defrag_wyk(_mapa_wier, _mapa_el);
	}
	UPEWNIJ_R(wez_il_wier(), 48) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWek2::testDefragSrodekLaczony() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = wez_il_wier()/2-1, _ind2 = wez_il_wier()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	for(_ind1; _ind1 < wez_il_wier()-2; ++_ind1) {
		for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
			if(_i == _ind1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else if(_i == _ind1+1) UPEWNIJ_R(wier[_i], pusty) << "_i=" << _i << "\n";
			else UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
		}
		defrag_licz(_mapa_wier, _mapa_el, 1);
		defrag_wyk(_mapa_wier, _mapa_el);
	}
	UPEWNIJ_R(wez_il_wier(), 48) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWek2::testDefragKon() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _il = wez_il_wier();
	usun(wez_il_wier()-1);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	defrag_licz(_mapa_wier, _mapa_el, 1);
	defrag_wyk(_mapa_wier, _mapa_el);
	UPEWNIJ_R(wez_il_wier(), _il-1) << "\n";
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWek2::testDefragCalosc() {
	std::cout << __FUNCTION__ << "\n";
	inicMalo();
	uint32_t _ind1 = wez_il_wier()/4, _ind2 = wez_il_wier()/2;
	usun(_ind1);
	usun(_ind2);
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	defrag_licz(_mapa_wier, _mapa_el, 2*wez_il_wier());
	defrag_wyk(_mapa_wier, _mapa_el);
	UPEWNIJ_R(wez_il_wier(), 48) << "\n";
	for(uint32_t _i = 0; _i < wez_il_wier(); ++_i) {
		UPEWNIJ_NR(wier[_i], pusty) << "_i=" << _i << "\n";
	}
	free(_mapa_wier);
	free(_mapa_el);
	niszcz();
}
void TestWek2::testUsunDupl() {
	std::cout << __FUNCTION__ << "\n";
	uint32_t _t[] = {0, 4, 3, 2, 1};
	wstaw_kon(_t, 5); usun(0);
	_t[0] = 1; wstaw_kon(_t, 5);
	_t[0] = 1; wstaw_kon(_t, 5);
	wstaw_kon(_t, 5); usun(3);
	_t[0] = 2; wstaw_kon(_t, 5);
	wstaw_kon(_t, 5); usun(5);
	_t[0] = 2; wstaw_kon(_t, 5);
	wstaw_kon(_t, 5); usun(7);
	wstaw_kon(_t, 5); usun(8);
	_t[0] = 3; wstaw_kon(_t, 5);
	_t[0] = 3; wstaw_kon(_t, 5);
	_t[0] = 1; wstaw_kon(_t, 5);
	wstaw_kon(_t, 5); usun(12);
	uint32_t* _mapa = 0;
	usun_dupl_licz(_mapa);
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 3) << "\n";
	UPEWNIJ_R(el.wez_il(), 15) << "\n";
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		UPEWNIJ_R(el[wier[_i].pierw], _i + 1) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw + 1], 4) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw + 2], 3) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw + 3], 2) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw + 4], 1) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2::wykonaj() {
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
	testDefragLiczZdefrag();
	testDefragPocz();
	testDefragSrodek();
	testDefragSrodekLaczony();
	testDefragKon();
	testDefragCalosc();
	testUsunDupl();
}
// -------------------------------------------------------
void TestWek2_Para::niszcz() {
	usun_kon(wier.wez_il());
}
void TestWek2_Para::testUsunDuplPoczKonPelny() {
	std::cout << __FUNCTION__ << "\n";
	Para<uint32_t> _t[] = {{0,0}, {5,5}, {4,4}, {3,3}, {2,2}, {1,1}};
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	_t[0] = {1,2}; wstaw_kon(_t, 6);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	wstaw_kon(_t, 6); usun(4);
	wstaw_kon(_t, 6); usun(5);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	_t[0] = {2,1}; wstaw_kon(_t, 6);
	wstaw_kon(_t, 6); usun(8);
	_t[0] = {3,1}; wstaw_kon(_t, 6);
	_t[0] = {3,2}; wstaw_kon(_t, 6);
	_t[0] = {3,2}; wstaw_kon(_t, 6);
	_t[0] = {3,1}; wstaw_kon(_t, 6);
	uint32_t* _mapa = 0;
	usun_dupl_licz(_mapa, FunHasz<Para<uint32_t>>(), FunHasz2<Para<uint32_t>>());
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 7);
	UPEWNIJ_R(el.wez_il(), 42);
	UPEWNIJ_R(el[wier[0].pierw], Para<uint32_t>({1,1})) << "\n";
	UPEWNIJ_R(el[wier[1].pierw], Para<uint32_t>({1,2})) << "\n";
	UPEWNIJ_R(el[wier[2].pierw], Para<uint32_t>({1,1})) << "\n";
	UPEWNIJ_R(el[wier[3].pierw], Para<uint32_t>({2,1})) << "\n";
	UPEWNIJ_R(el[wier[4].pierw], Para<uint32_t>({3,1})) << "\n";
	UPEWNIJ_R(el[wier[5].pierw], Para<uint32_t>({3,2})) << "\n";
	UPEWNIJ_R(el[wier[6].pierw], Para<uint32_t>({3,1})) << "\n";
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		UPEWNIJ_R(el[wier[_i].pierw+1], Para<uint32_t>({5,5})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+2], Para<uint32_t>({4,4})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+3], Para<uint32_t>({3,3})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+4], Para<uint32_t>({2,2})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+5], Para<uint32_t>({1,1})) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2_Para::testUsunDuplPoczKonPusty() {
	std::cout << __FUNCTION__ << "\n";
	Para<uint32_t> _t[] = {{0,0}, {5,5}, {4,4}, {3,3}, {2,2}, {1,1}};
	wstaw_kon(_t, 6); usun(0);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	_t[0] = {1,2}; wstaw_kon(_t, 6);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	wstaw_kon(_t, 6); usun(5);
	wstaw_kon(_t, 6); usun(6);
	_t[0] = {1,1}; wstaw_kon(_t, 6);
	_t[0] = {2,1}; wstaw_kon(_t, 6);
	wstaw_kon(_t, 6); usun(9);
	_t[0] = {3,1}; wstaw_kon(_t, 6);
	_t[0] = {3,2}; wstaw_kon(_t, 6);
	_t[0] = {3,2}; wstaw_kon(_t, 6);
	_t[0] = {3,1}; wstaw_kon(_t, 6);
	wstaw_kon(_t, 6); usun(14);
	uint32_t* _mapa = 0;
	usun_dupl_licz(_mapa, FunHasz<Para<uint32_t>>(), FunHasz2<Para<uint32_t>>());
	uloz_wyk(_mapa);
	UPEWNIJ_R(wier.wez_il(), 7);
	UPEWNIJ_R(el.wez_il(), 42);
	UPEWNIJ_R(el[wier[0].pierw], Para<uint32_t>({1,1})) << "\n";
	UPEWNIJ_R(el[wier[1].pierw], Para<uint32_t>({1,2})) << "\n";
	UPEWNIJ_R(el[wier[2].pierw], Para<uint32_t>({1,1})) << "\n";
	UPEWNIJ_R(el[wier[3].pierw], Para<uint32_t>({2,1})) << "\n";
	UPEWNIJ_R(el[wier[4].pierw], Para<uint32_t>({3,1})) << "\n";
	UPEWNIJ_R(el[wier[5].pierw], Para<uint32_t>({3,2})) << "\n";
	UPEWNIJ_R(el[wier[6].pierw], Para<uint32_t>({3,1})) << "\n";
	for(uint32_t _i = 0; _i < wier.wez_il(); ++_i) {
		UPEWNIJ_R(el[wier[_i].pierw+1], Para<uint32_t>({5,5})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+2], Para<uint32_t>({4,4})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+3], Para<uint32_t>({3,3})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+4], Para<uint32_t>({2,2})) << "_i=" << _i << "\n";
		UPEWNIJ_R(el[wier[_i].pierw+5], Para<uint32_t>({1,1})) << "_i=" << _i << "\n";
	}
	free(_mapa);
	niszcz();
}
void TestWek2_Para::wykonaj() {
	testUsunDuplPoczKonPelny();
	testUsunDuplPoczKonPusty();
}
// -------------------------------------------------------
void TestUchPula::inic() {
	uint32_t _il = 10;
	for(uint32_t _i = 0; _i < _il; ++_i) {
		wstaw(_il-1-_i);
	}
}
void TestUchPula::niszcz() {
	uint32_t _il = 10;
	for(int32_t _i = 9; _i >= 0; --_i) {
		usun(_i);
	}
}
void TestUchPula::testAktualWszystkie() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	aktual(_mapa);
	for(uint32_t _i = 0; _i < 10; ++_i) {
		UPEWNIJ_R(el[_i], _i) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestUchPula::testAktualPoczKon() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[10];
	for(uint32_t _i = 1; _i < 10-1; ++_i) _mapa[_i] = 0x80000000;
	_mapa[0] = 9;
	_mapa[9] = 0;
	aktual(_mapa);
	UPEWNIJ_R(el[0], 0) << "\n";
	UPEWNIJ_R(el[9], 9) << "\n";
	for(uint32_t _i = 1; _i < 10-1; ++_i) {
		UPEWNIJ_R(el[_i], 9-_i) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestUchPula::testAktualSrodek() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[10];
	_mapa[0] = 0x80000000;
	_mapa[9] = 0x80000000;
	for(uint32_t _i = 1; _i < 10-1; ++_i) _mapa[_i] = 9-_i;
	aktual(_mapa);
	UPEWNIJ_R(el[0], 9) << "\n";
	UPEWNIJ_R(el[9], 0) << "\n";
	for(uint32_t _i = 1; _i < 10-1; ++_i) {
		UPEWNIJ_R(el[_i], _i) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestUchPula::testAktualZadne() {
	std::cout << __FUNCTION__ << "\n";
	inic();
	uint32_t _mapa[10];
	for(uint32_t _i = 0; _i < 10; ++_i) _mapa[_i] = 0x80000000;
	aktual(_mapa);
	for(uint32_t _i = 0; _i < 10; ++_i) {
		UPEWNIJ_R(el[_i], 9-_i) << "_i=" << _i << "\n";
	}
	niszcz();
}
void TestUchPula::wykonaj() {
	testAktualWszystkie();
	testAktualPoczKon();
	testAktualSrodek();
	testAktualZadne();
}
// -------------------------------------------------------
void main() {
	TestWek _test_Wek;
	_test_Wek.wykonaj();
	_test_Wek.niszcz();
	TestWek_Para _test_Wek_Para;
	_test_Wek_Para.wykonaj();
	_test_Wek_Para.niszcz();
	TestWekLuz _test_WekLuz;
	_test_WekLuz.wykonaj();
	_test_WekLuz.niszcz();
	TestWek2 _test_Wek2;
	_test_Wek2.wykonaj();
	_test_Wek2.niszcz();
	TestWek2_Para _test_Wek2_Para;
	_test_Wek2_Para.wykonaj();
	_test_Wek2_Para.niszcz();
	TestUchPula _test_UchPula;
	_test_UchPula.wykonaj();
	_test_UchPula.niszcz();
	std::cout << "Sukces! Nacisnij ENTER";
	std::cin.get();
}
// -------------------------------------------------------

