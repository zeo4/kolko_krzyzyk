# pragma once

#include "globalne.h"
#include "listaObiekty.h"
#include "typedefy.h"

class Obiekt3w;

class Drzewo8 {
	typedef set<Obiekt3w* const>							ZbiorOb3w_;
	typedef set<Obiekt3w const* const>						ZbiorOb3wStale_;
	typedef map<Obiekt3w const* const, ZbiorOb3wStale_>		MapaOb3wObiekty3w_;
public:
	vector<Drzewo8>		galezie;
	ZbiorOb3w_			liscie;
	XMFLOAT3 const		zakresMaks;
	XMFLOAT3 const		zakresMin;
						Drzewo8(XMFLOAT3 const, XMFLOAT3 const);
	void				dodaj(Obiekt3w* const);
	void				dodajDziecko(Obiekt3w* const);
	byte				liczNr(XMFLOAT3);
	XMFLOAT3			liczPozLokal(XMFLOAT3) const;
	void				przepiszObiektyDzieci();
	void				tworzDzieci();
	void				wezSasiedzi(MapaOb3wObiekty3w_* const) const;
	void				wezRogiKula(
							XMVECTOR const, float const, vector<XMFLOAT3>* const
						) const;
	void				wezRogiObiekt(
							Obiekt3w const* const, vector<XMFLOAT3>* const
						) const;
};



