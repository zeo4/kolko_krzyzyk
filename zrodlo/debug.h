#pragma once

#include "globalne.h"
#include <chrono>
#include <iomanip>

using namespace std::chrono;

class Logi {
	typedef time_point<high_resolution_clock>		Czas_;
	void						pisz_calosc(string const, string const);
	void						pisz_czas();
	void						pisz_nowa_linia();
	void						pisz_nr_wiersz();
	void						pisz_odstep(uint8_t const);
	void						pisz_tab();
	void						pisz_tresc(string const);
	void						pisz_tytul(string const);
	void						pisz_wciecie();
	char const* const			_nazwa_plik;
	std::ofstream				_plik;
	std::streampos				_poz_wiersz_ost;
	uint64_t					_nr_wiersz;
	LARGE_INTEGER				_t_start;
	LARGE_INTEGER				_t_nowy;
	LARGE_INTEGER				_t_stary;
	LARGE_INTEGER				_tiki_na_sek;
	static vector<short>		_taby;
	static short				_tab_tresc;
	short						_wciecie_taby;
public:
								Logi(char const* const = "log.txt");
	void						pisz(string const, string const);
	void						pisz_start(string const, string const);
	void						pisz_stop(string const, string const);
} extern logi;

// do obsługi błędów / wyjątków
HRESULT extern wynik;
struct Wyjatek {
	string		opis;
				Wyjatek();
};
void SprawdzWynik(HRESULT, string);
void ObslugaWyjatek(Wyjatek);

