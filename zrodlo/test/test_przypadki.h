#pragma once

#include<stdint.h>
#include<cassert>
#include <iostream>
#include<test_struktura.h>
#include<wek.h>
// -------------------------------------------------------
class TestWektor : public Wektor<uint32_t> {
public:
	void		inic();
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
};
// -------------------------------------------------------
class TestWektor2 : public Wektor2<uint32_t> {
public:
	void		inic();
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
};
// -------------------------------------------------------


