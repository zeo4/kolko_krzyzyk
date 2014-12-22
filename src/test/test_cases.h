#pragma once

#include<stdint.h>
#include<cassert>
#include <iostream>
#include<test_structure.h>
#include<vec.h>
#include<handles.h>
// -------------------------------------------------------
class TestWek : public Vec<uint32_t> {
public:
	void		inic();
	void		inicMalo();
	void		niszcz();
	void		wykonaj();
	void		testUlozLiczNieusunieteZadne();
	void		testUlozLiczUsunietePoczKon();
	void		testUlozLiczUsunieteSrodek();
	void		testUlozLiczUsunieteWszystkie();
	void		testUlozNieusunieteZadne();
	void		testUlozUsunietePoczKon();
	void		testUlozUsunieteSrodek();
	void		testUlozUsunieteWszystkie();
	void		testUlozUnikatLiczNieusunieteZadne();
	void		testUlozUnikatLiczUsunietePoczKon();
	void		testUlozUnikatLiczUsunieteSrodek();
	void		testUlozUnikatLiczUsunieteWszystkie();
	void		testUlozUnikatNieusunieteZadne();
	void		testUlozUnikatUsunietePoczKon();
	void		testUlozUnikatUsunieteSrodek();
	void		testUlozUnikatUsunieteWszystkie();
	void		testDefragLiczZdefrag();
	void		testDefragPocz();
	void		testDefragSrodek();
	void		testDefragSrodekLaczony();
	void		testDefragKon();
	void		testDefragCalosc();
	void		testUsunDupl();
};
// -------------------------------------------------------
class TestWek_Para : public Vec<Para<uint32_t>> {
public:
	void		niszcz();
	void		wykonaj();
	void		testUsunDuplPoczKonPelny();
	void		testUsunDuplPoczKonPusty();
};
// -------------------------------------------------------
class TestWek2 : public Vec2<uint32_t> {
public:
	void		inic();
	void		inicMalo();
	void		niszcz();
	void		wykonaj();
	void		testUlozLiczNieusunieteZadne();
	void		testUlozLiczUsunietePoczKon();
	void		testUlozLiczUsunieteSrodek();
	void		testUlozLiczUsunieteWszystkie();
	void		testUlozNieusunieteZadne();
	void		testUlozUsunietePoczKon();
	void		testUlozUsunieteSrodek();
	void		testUlozUsunieteWszystkie();
	void		testUlozUnikatLiczNieusunieteZadne();
	void		testUlozUnikatLiczUsunietePoczKon();
	void		testUlozUnikatLiczUsunieteSrodek();
	void		testUlozUnikatLiczUsunieteWszystkie();
	void		testUlozUnikatNieusunieteZadne();
	void		testUlozUnikatUsunietePoczKon();
	void		testUlozUnikatUsunieteSrodek();
	void		testUlozUnikatUsunieteWszystkie();
	void		testDefragLiczZdefrag();
	void		testDefragPocz();
	void		testDefragSrodek();
	void		testDefragSrodekLaczony();
	void		testDefragKon();
	void		testDefragCalosc();
	void		testUsunDupl();
};
// -------------------------------------------------------
class TestWek2_Para : public Vec2<Para<uint32_t>> {
public:
	void		niszcz();
	void		wykonaj();
	void		testUsunDuplPoczKonPelny();
	void		testUsunDuplPoczKonPusty();
};
// -------------------------------------------------------
class TestWekLuz : public WekLuz<uint32_t> {
public:
	void		inic();
	void		niszcz();
	void		wykonaj();
	void		testAktualWszystkie();
	void		testAktualPoczKon();
	void		testAktualSrodek();
	void		testAktualZadne();
};
// -------------------------------------------------------
class TestUchPula : public UchPula {
public:
	void		inic();
	void		niszcz();
	void		wykonaj();
	void		testAktualWszystkie();
	void		testAktualPoczKon();
	void		testAktualSrodek();
	void		testAktualZadne();
};
// -------------------------------------------------------


