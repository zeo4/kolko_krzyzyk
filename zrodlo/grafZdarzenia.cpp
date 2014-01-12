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

class ZdarzGraf {
public:
	Ekran*			ekran;
	Kamera*			kamera;
	Projektor*		projektor;
	void			virtual wykonajZdarzenie() const = 0;
};
typedef list<ZdarzGraf* const>		ListaZdarzGraf_;

class ZdarzRysujOb : public ZdarzGraf {
	void			aktualCoObiekt() const;
public:
	Obiekt3W*		obiekt3W;
					ZdarzRysujOb();
	void			virtual wykonajZdarzenie() const;
};
void ZdarzRysujOb::aktualCoObiekt() const {
	// przed wysłaniem do szadera zawsze trzeba transponować
	zasoby.daneCoObiekt.macSWP = XMMatrixTranspose(
		obiekt3W->wezSwiat() * kamera->wezWidok() * projektor->wezProjekcja());
	
	zasoby.wypelnijCoObiekt();
	zasoby.wiazCoObiekt();
}
ZdarzRysujOb::ZdarzRysujOb() : obiekt3W(NULL)
	{}
void ZdarzRysujOb::wykonajZdarzenie() const {
	if(obiekt3W == NULL) {
		Wyjatek wyj;
		wyj.opis = "Wykonanie zdarzenia graficznego RysujOb. Nie podano obiektu zdarzenia.";
	}
	obiekt3W->wiazWierz();
	obiekt3W->wiazIndeksy();
	aktualCoObiekt();
	zasoby.render->DrawIndexed(3, 0, 0);
}

