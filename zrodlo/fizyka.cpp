#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFIKA_
#define _GRAFIKA_
#include "grafika.cpp"
#endif

#ifndef _ZDARZENIA_
#define _ZDARZENIA_
#include "zdarzenia.cpp"
#endif

typedef list<Zdarzenie const>::const_iterator		IterListaZdarzen_;

class Fizyk {
	UINT						tikCzasStart;
	list<Zdarzenie const>		listaZdarzen;
	IterListaZdarzen_			iterAktZdarzenie;
	void						idzNastZdarzenie();
	void						resetujAktZdarzenie();
public:
								Fizyk();
	void						dodajZdarzenie(Zdarzenie const);
	void						usunZdarzenie(IterListaZdarzen_);
};
void Fizyk::idzNastZdarzenie() {
	if(iterAktZdarzenie == listaZdarzen.end()) {
		return;
	}

	++iterAktZdarzenie;
}
void Fizyk::resetujAktZdarzenie() {
	iterAktZdarzenie = listaZdarzen.begin();
}
Fizyk::Fizyk() : tikCzasStart(0)
	{}
void Fizyk::dodajZdarzenie(
	Zdarzenie const		zdarzenie
	) {
	listaZdarzen.push_back(zdarzenie);
}
void Fizyk::usunZdarzenie(
	IterListaZdarzen_		iter
	) {
	listaZdarzen.erase(iter);
}

