#pragma once

#include "globalne.h"
#include "typedefy.h"

class Obiekt3w;

class SiatkaObiekty {
	typedef tuple<float, float, float>									KluczSiatka_;
	//typedef unordered_multimap<KluczSiatka_, Obiekt3w* const, Hasz>		Siatka_;
	typedef multimap<KluczSiatka_ const, Obiekt3w* const>				Siatka_;
	typedef pair<KluczSiatka_ const, Obiekt3w* const>					ParaSiatka_;
	Siatka_					siatka;
	void					liczNrObszar(KluczSiatka_* const) const;
public:
	static float const		rozmObszar;
	void					czysc();
	void					dodajObiekt(Obiekt3w* const);
	void					wezSasiedzi(MapaSasiedzi_* const) const;
};

