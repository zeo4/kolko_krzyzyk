#pragma once

#include "debug.h"

bool Logi::flgWlaczone = true;
void Logi::piszTytul(string const tytul) {
	plik << tytul << "\t";
}
void Logi::piszTresc(string const tresc) {
	piszWciecie();

	UINT i = -1;
	while(++i != tresc.length()) {
		switch(tresc[i]) {
		case '\n':
			piszNowaLinie();
			break;
		default:
			plik << tresc[i];
			break;
		}
	}
}
void Logi::piszNrWiersza() {
	plik << ++nrWiersza << "\t";
}
void Logi::piszCzas() {
	UINT tik = clock();
	plik << float(tik - tikProgramStart) / CLOCKS_PER_SEC << "\t";
	plik << float(tik - tikPoprzedni) / CLOCKS_PER_SEC << "\t";
	plik << tik - tikPoprzedni << "\t";
	tikPoprzedni = tik;
}
void Logi::piszNowaLinie() {
	plik << "\n";
	piszNrWiersza();
	//za pozostałe kolumny
	for(int i = 0; i < tabowDoTresci-1; i++) {
		plik << "\t";
	}
	piszWciecie();
}
void Logi::piszWciecie() {
	for(int i = 0; i < poziomAktWciecia; i++) {
		plik << ".\t";
	}
}
Logi::Logi(char const* const nazwa) : nazwaPliku(nazwa), nrWiersza(0), poziomAktWciecia(0), tabowDoTresci(0) {
	// otwórz wyczyszczony
	plik.open(nazwaPliku, std::ios::trunc);

	// pisz nagłówki
	plik << "Nr\t" << "t[s]\t" << "dt[s]\t" << "dt[tik]\t" << "Tytul\t" << "Tresc";
	plik << "\n-------------------------------------------------------------";
	tabowDoTresci = 5;

	plik.close();
}
void Logi::pisz(string const tytul, string const tresc) {
	if(flgWlaczone == false) {
		return;
	}

	plik.open(nazwaPliku, std::ios::app);

	plik << "\n";
	piszNrWiersza();
	piszCzas();
	piszTytul(tytul);
	piszTresc(tresc);

	plik.close();
}
void Logi::piszStart(string const tytul, string const tresc) {
	if(flgWlaczone == false) {
		return;
	}

	pisz(tytul, tresc);
	++poziomAktWciecia;
}
void Logi::piszStop(string const tytul, string const tresc) {
	if(flgWlaczone == false) {
		return;
	}

	--poziomAktWciecia;
	pisz(tytul, tresc);
}
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

