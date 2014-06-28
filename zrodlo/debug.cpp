#pragma once

#include "debug.h"

using namespace std::chrono;

Logi::Logi(char const* const nazwa) : _nazwa_plik(nazwa), _nr_wiersz(0), _wciecie_taby(0), _poz_wiersz_ost(0) {
	QueryPerformanceCounter(&_t_start);
	_t_stary = _t_start;
	QueryPerformanceFrequency(&_tiki_na_sek);

	// otwórz wyczyszczony
	_plik.open(_nazwa_plik, std::ios::trunc);
	_plik << std::setprecision(2) << std::fixed;

	// pisz nagłówki
	_plik << "Nr"; pisz_tab();
	_plik << "t[s]"; pisz_tab();
	_plik << "dt[ms]"; pisz_tab();
	_plik << "Tytul"; pisz_tab();
	_plik << "Tresc";
	_plik << "\n-------------------------------------------------------------";

	_plik.close();
}
void Logi::czas() {
	QueryPerformanceCounter(&_t_nowy);
	pisz_calosc("czas", "czas");
	QueryPerformanceCounter(&_t_stary);
}
void Logi::pisz(string const tytul, string const tresc) {
	QueryPerformanceCounter(&_t_nowy);
	pisz_calosc(tytul, tresc);
	QueryPerformanceCounter(&_t_stary);
}
void Logi::pisz_calosc(string const tytul, string const tresc) {
	_plik.open(_nazwa_plik, std::ios::app);
	_plik << "\n";
	_poz_wiersz_ost = _plik.tellp();
	pisz_nr_wiersz();
	pisz_czas();
	pisz_tytul(tytul);
	pisz_tresc(tresc);
	_plik.close();
}
void Logi::pisz_czas() {
	_plik << double(_t_nowy.QuadPart - _t_start.QuadPart) / _tiki_na_sek.QuadPart;
	pisz_tab();
	_plik << std::setprecision(4);
	_plik << double(_t_nowy.QuadPart - _t_stary.QuadPart) * 1000 / _tiki_na_sek.QuadPart;
	_plik << std::setprecision(2);
	pisz_tab();
}
void Logi::pisz_nowa_linia() {
	_plik << "\n";
	_poz_wiersz_ost = _plik.tellp();

	pisz_nr_wiersz();
	pisz_odstep(_tab_tresc - (_plik.tellp() - _poz_wiersz_ost));
	pisz_wciecie();
}
void Logi::pisz_nr_wiersz() {
	_plik << ++_nr_wiersz;
	pisz_tab();
}
void Logi::pisz_start(string const tytul, string const tresc) {
	QueryPerformanceCounter(&_t_nowy);
	pisz_calosc(tytul, tresc);
	++_wciecie_taby;
	QueryPerformanceCounter(&_t_stary);
}
void Logi::pisz_stop(string const tytul, string const tresc) {
	QueryPerformanceCounter(&_t_nowy);
	--_wciecie_taby;
	pisz_calosc(tytul, tresc);
	QueryPerformanceCounter(&_t_stary);
}
void Logi::pisz_odstep(uint8_t const odstep) {
	for(uint8_t i = 0; i < odstep; ++i) {
		_plik << " ";
	}
}
void Logi::pisz_tab() {
	uint32_t poz = _plik.tellp() - _poz_wiersz_ost;
	for(short i = 0; i < _taby.size(); ++i) {
		if(poz < _taby[i]) {
			pisz_odstep(_taby[i] - poz);
			break;
		}
	}
}
void Logi::pisz_tresc(string const tresc) {
	pisz_wciecie();

	UINT i = -1;
	while(++i != tresc.length()) {
		switch(tresc[i]) {
		case '\n':
			pisz_nowa_linia();
			break;
		default:
			_plik << tresc[i];
			break;
		}
	}
}
void Logi::pisz_tytul(string const tytul) {
	_plik << tytul;
	pisz_tab();
}
void Logi::pisz_wciecie() {
	for(short i = 0; i < _wciecie_taby; ++i) {
		_plik << ".";
		pisz_tab();
	}
}
short Logi::_tab_tresc = 34;
vector<short> Logi::_taby = {8, 17, 26, 34, 38, 42, 46, 50, 54, 58, 62, 66};
Logi logi;

HRESULT wynik;
Wyjatek::Wyjatek() : opis("")
	{}
void SprawdzWynik(HRESULT wynik, string opis) {
	if(FAILED(wynik)){
		Wyjatek wyj;
		wyj.opis = opis;
		throw wyj;
	}else{
		logi.pisz("OK", opis);
	}
}
void ObslugaWyjatek(Wyjatek wyj) {
	logi.pisz("BLAD", wyj.opis);
}

