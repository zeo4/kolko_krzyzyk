#pragma once

#include "globalne.h"
#include "typedefy.h"

class IObiekt;

class SiatkaObiekty {
	typedef set<IObiekt* const>						ObiektyObszar_;
	typedef map<float const, ObiektyObszar_>		Siatka1Obiekty_;
	typedef map<float const, Siatka1Obiekty_>		Siatka2Obiekty_;
	typedef map<float const, Siatka2Obiekty_>		Siatka3Obiekty_;
	Siatka3Obiekty_		siatka;
public:
	typedef Siatka3Obiekty_::iterator				IteratorX;
	typedef Siatka3Obiekty_::const_iterator			StalyIteratorX;
	typedef Siatka2Obiekty_::iterator				IteratorY;
	typedef Siatka2Obiekty_::const_iterator			StalyIteratorY;
	typedef Siatka1Obiekty_::iterator				IteratorZ;
	typedef Siatka1Obiekty_::const_iterator			StalyIteratorZ;
	typedef ObiektyObszar_::iterator				IteratorOb;
	typedef ObiektyObszar_::const_iterator			StalyIteratorOb;
	void				dopiszObiekt(float, float, float, IObiekt* const);
	void				dopiszSiatka(SiatkaObiekty const);
	void				czysc();
	void				ustawWspolnyObiekt(IObiekt* const);
	void				wezKolizje(MapaKolizje_* const) const;
	bool				wezObiekty(
							set<IObiekt* const>* const, float, float, float
						) const;
};

