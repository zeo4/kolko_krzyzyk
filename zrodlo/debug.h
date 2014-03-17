#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "globalne.h"

class Logi {
	std::ofstream			plik;
	char const* const		nazwaPliku;
	UINT					nrWiersza;
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
} logi;

// do obsługi błędów / wyjątków
HRESULT wynik;
struct Wyjatek {
	string		opis;
				Wyjatek();
};
void SprawdzWynik(HRESULT, string);
void ObslugaWyjatek(Wyjatek);

#endif