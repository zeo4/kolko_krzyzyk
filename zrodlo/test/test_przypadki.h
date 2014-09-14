#pragma once

#include<stdint.h>
#include<cassert>
#include<test_struktura.h>
#include<wek.h>
// -------------------------------------------------------
class TestWektor : public Wektor<uint32_t> {
public:
	void			inic();
	void			wykonaj();
	void			niszcz();
	void			testUlozLiczNieusunieteZadne();
	void			testUlozUnikatLiczNieusunieteZadne();

	//void			testUlozUsunietePoczKon();
	//void			testUlozUsunieteSrodek();
	//void			testUlozUsunietePoczSrodekKon();
	//void			testUlozUsunieteWszystkie();
private:
	uint32_t		test_il;
};
// -------------------------------------------------------


