#pragma once

#include<stdio.h>
#include<test_szkielet.h>
#include<wek.h>
// -------------------------------------------------------
template<class T>
class TestWektor : public Wektor<T> {
public:
	void		inic();
	void		wykonaj();
	void		niszcz();
	void		testUlozNieusunieteZadne();
	void		testUlozUsunietePoczKon();
	void		testUlozUsunieteSrodek();
	void		testUlozUsunietePoczSrodekKon();
	void		testUlozUsunieteWszystkie();
};
template<class T>
void TestWektor<T>::inic() {
	for(uint32_t _i = 0; _i < 800; ++_i) {
		wstaw_kon((800-1-_i)/10);
	}
}
template<class T>
void TestWektor<T>::wykonaj() {

}
template<class T>
void TestWektor<T>::niszcz() {}
template<class T>
void TestWektor<T>::testUlozNieusunieteZadne() {
	inic();
	uint32_t* _mapa = 0;
	w.uloz_licz(_mapa);

}
// -------------------------------------------------------

