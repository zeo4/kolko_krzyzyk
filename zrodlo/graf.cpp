#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFZASOBY_
#define _GRAFZASOBY_
#include "grafZasoby.cpp"
#endif

#ifndef _GRAFZDARZENIA_
#define _GRAFZDARZENIA_
#include "grafZdarzenia.cpp"
#endif

class Grafik {
	Ekran					ekran;
	Kamera					kamera;
	Projektor				projektor;
	ListaZdarzGraf_*		zdarzeniaGraf;
	void					usunNiszczZdarzenie(ListaZdarzGraf_::const_iterator);
public:
							Grafik();
	void					przypnijListaGraf(ListaZdarzGraf_* const);
	void					wykonajZdarzenia();
};
void Grafik::usunNiszczZdarzenie(
	ListaZdarzGraf_::const_iterator		iter
	) {
	delete *iter;
	zdarzeniaGraf->erase(iter);
}
Grafik::Grafik() {
	logi.piszStart("START", "Stworz grafika.");
	kamera.ustawPozycja(+0.0f, +0.0f, -0.5f);
	kamera.ustawCel(+0.0f, 0.0f, 0.0f);
	kamera.ustawGora(+0.0f, +1.0f, +0.0f);
	kamera.tworzWidok();
	projektor.ustawKat(90);
	projektor.ustawBlizszaPlaszcz(1.0f);
	projektor.ustawDalszaPlaszcz(100.0f);
	projektor.tworzProjekcja();
	logi.piszStop("STOP", "Stworz grafika.");
}
void Grafik::przypnijListaGraf(
	ListaZdarzGraf_* const		lista
	) {
	zdarzeniaGraf = lista;
}
void Grafik::wykonajZdarzenia() {
	ListaZdarzGraf_::const_iterator iter = zdarzeniaGraf->begin();
	while(iter != zdarzeniaGraf->end()) {
		(*iter)->ekran = &ekran;
		(*iter)->kamera = &kamera;
		(*iter)->projektor = &projektor;
		(*iter)->wykonajZdarzenie();
		usunNiszczZdarzenie(iter++);
	}
}


