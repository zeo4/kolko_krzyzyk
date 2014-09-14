#pragma once

#include<stdint.h>
#include<cassert>
#include <iostream>
#include<test_struktura.h>
#include<wek.h>
// -------------------------------------------------------
class TestWektor : public Wektor<uint32_t> {
public:
	void			inic();
	void			wykonaj();
	void			niszcz();
	void			testUlozLiczNieusunieteZadne();
	void			testUlozLiczUsunietePoczKon();
	void			testUlozLiczUsunieteSrodek();
	void			testUlozLiczUsunieteWszystkie();
	void			testUlozUnikatLiczNieusunieteZadne();
	void			testUlozUnikatLiczUsunietePoczKon();
	void			testUlozUnikatLiczUsunieteSrodek();
	void			testUlozUnikatLiczUsunieteWszystkie();
};
// -------------------------------------------------------


