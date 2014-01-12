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

class ZdarzFiz {
public:
	void		virtual wykonajZdarzenie() const = 0;
};

class ZdarzRuchOb : public ZdarzFiz {
public:
	Obiekt3W*			obiekt;
	XMVECTOR			wektorRuch;
						ZdarzRuchOb();
	void				virtual wykonajZdarzenie() const;
};
ZdarzRuchOb::ZdarzRuchOb() : obiekt(NULL), wektorRuch(XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
	{}
void ZdarzRuchOb::wykonajZdarzenie() const {
	if(obiekt == NULL) {
		Wyjatek wyj;
		wyj.opis = "Wykonanie zdarzenia fizycznego Ruch. Nie podano obiektu zdarzenia.";
		throw wyj;
	}
	obiekt->przesun(wektorRuch);
}

typedef list<ZdarzFiz const* const>		ListaZdarzFiz_;