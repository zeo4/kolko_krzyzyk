#pragma once
#include "globalne.h"
#include "debug.h"
#include "listaObiekty.h"
#include "drzewo8.h"
#include <fizyka.h>
#include <grafika.h>
// -------------------------------------------------------
class Swiat : protected ZasGraf {
public:
							Swiat();
							~Swiat();
	void					rys_klatka();
	uint32_t				tworz_ob(uint32_t const&, uint32_t const&);
	//void					dodaj(IObiekt* const);
	//void					wezObPromien(IObiekt** const, Obiekt3w const* const) const;
	//void					wykonajFizyka();
	Fizyka*					fiz;
	Grafika*				graf;
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


