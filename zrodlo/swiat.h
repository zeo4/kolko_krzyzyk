#pragma once
#include "globalne.h"
#include "debug.h"
#include "listaObiekty.h"
#include "drzewo8.h"
#include <fizyka.h>
#include <grafika.h>
// -------------------------------------------------------
class Swiat : protected ZasGraf, protected Zadania {
public:
							Swiat();
							~Swiat();
	void					wyk_zad();
protected:
	Fizyka*					fiz;
	Grafika*				graf;
public:
	//void					dodaj(IObiekt* const);
	//void					wezObPromien(IObiekt** const, Obiekt3w const* const) const;
	//void					wykonajFizyka();
private:
	//MapaSasiedzi_			sasiedzi;
	//void					aktualizujSasiedzi();
	//void					niszczObiektSwiat(IObiekt* const);
	//void					niszczObiektySwiat();
	//void					usunProjekcjaZ1(
	//							XMVECTOR* const, float const, float const
	//						) const;
	//void					usunWidokPkt(XMVECTOR* const) const;
	//void					usunWidokWektor(XMVECTOR* const) const;
};
// -------------------------------------------------------


