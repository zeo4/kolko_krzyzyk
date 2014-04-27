#pragma once

#include "globalne.h"
#include "typedefy.h"

class IObiekt;

class SiatkaObiekty {
	typedef map<float const, ZbiorOb3wStale_>		Siatka1Obiekty_;
	typedef map<float const, Siatka1Obiekty_>		Siatka2Obiekty_;
	typedef map<float const, Siatka2Obiekty_>		Siatka3Obiekty_;
	Siatka3Obiekty_		siatka;
public:
	typedef Siatka3Obiekty_::iterator			IteratorX;
	typedef Siatka3Obiekty_::const_iterator		StalyIteratorX;
	typedef Siatka2Obiekty_::iterator			IteratorY;
	typedef Siatka2Obiekty_::const_iterator		StalyIteratorY;
	typedef Siatka1Obiekty_::iterator			IteratorZ;
	typedef Siatka1Obiekty_::const_iterator		StalyIteratorZ;
	typedef ZbiorOb3wStale_::iterator			IteratorOb;
	typedef ZbiorOb3wStale_::const_iterator		StalyIteratorOb;
	void				dopiszObiekt(float, float, float, Obiekt3w const* const);
	void				dopiszSiatka(SiatkaObiekty const);
	void				czysc();
	void				ustawWspolnyObiekt(Obiekt3w* const);
	void				wezKolizje(MapaOb3wObiekty3w_&) const;
	StalyIteratorX		wezKon() const;
	bool				wezObiekty(
							ZbiorOb3wStale_&, float, float, float
						) const;
	StalyIteratorX		wezPocz() const;
	UINT				wezRozm() const;
};

