#pragma once

#include "globalne.h"

class Logi {
	bool static				flgWlaczone;
	char const* const		nazwaPliku;
	UINT					nrWiersza;
	std::ofstream			plik;
	UINT					poziomAktWciecia;
	short					tabowDoTresci;
	void					piszTytul(string const);
	void					piszTresc(string const);
	void					piszNrWiersza();
	void					piszCzas();
	void					piszNowaLinie();
	void					piszWciecie();
public:
							Logi(char const* const = "log.txt");
	void					pisz(string const, string const);
	void					piszStart(string const, string const);
	void					piszStop(string const, string const);
} extern logi;

// do obsługi błędów / wyjątków
HRESULT extern wynik;
struct Wyjatek {
	string		opis;
				Wyjatek();
};
void SprawdzWynik(HRESULT, string);
void ObslugaWyjatek(Wyjatek);

