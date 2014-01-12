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

#ifndef _GRAF_
#define _GRAF_
#include "graf.cpp"
#endif

#ifndef _FIZZDARZENIA_
#define _FIZZDARZENIA_
#include "fizZdarzenia.cpp"
#endif

class Fizyk {
	ListaOb_*				obiektyScena;
	ListaZdarzFiz_*			zdarzeniaFiz;
	ListaZdarzGraf_*		zdarzeniaGraf;
public:
	void					dodajZdarzenie(ZdarzFiz const* const);
	void					przypnijListaFiz(ListaZdarzFiz_* const);
	void					przypnijListaGraf(ListaZdarzGraf_* const);
	void					przypnijObiektyScena(ListaOb_* const);
	void					tworzZdarzenia();
	void					usunNiszczZdarzenie(ListaZdarzFiz_::const_iterator);
	void					wykonajZdarzenia();
};
void Fizyk::dodajZdarzenie(
	ZdarzFiz const* const		zdarzenie
	) {
	zdarzeniaFiz->push_back(zdarzenie);
}
void Fizyk::przypnijListaFiz(
	ListaZdarzFiz_* const		lista
	) {
	zdarzeniaFiz = lista;
}
void Fizyk::przypnijListaGraf(
	ListaZdarzGraf_* const		lista
	) {
	zdarzeniaGraf = lista;
}
void Fizyk::przypnijObiektyScena(
	ListaOb_* const		lista
	) {
	obiektyScena = lista;
}
void Fizyk::tworzZdarzenia() {
	ListaOb_::const_iterator iterObiektyScena = obiektyScena->begin();
	while(iterObiektyScena != obiektyScena->end()) {
		ZdarzRysujOb* const zdarzenie = new ZdarzRysujOb;
		zdarzenie->obiekt3W = (*iterObiektyScena);
		zdarzeniaGraf->push_back(zdarzenie);
		++iterObiektyScena;
	}
}
void Fizyk::usunNiszczZdarzenie(
	ListaZdarzFiz_::const_iterator		iter
	) {
	delete *iter;
	zdarzeniaFiz->erase(iter);
}
void Fizyk::wykonajZdarzenia() {
	ListaZdarzFiz_::const_iterator iter = zdarzeniaFiz->begin();
	while(iter != zdarzeniaFiz->end()) {
		(*iter)->wykonajZdarzenie();
		usunNiszczZdarzenie(iter++);
	}
}


