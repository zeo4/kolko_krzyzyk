# pragma once

#include "globalne.h"
#include "listaObiekty.h"
#include "typedefy.h"

class Obiekt3w;

class Drzewo8 {
	typedef set<Obiekt3w* const>						Liscie_;
	typedef set<Liscie_ const* const>					ZbiorLiscie_;
	typedef vector<Drzewo8* const>						Galezie_;
	typedef vector<XMFLOAT3 const>						Rogi_;
	typedef set<Obiekt3w const* const>					Sasiedzi_;
	typedef map<Obiekt3w const* const, Sasiedzi_>		MapaSasiedzi_;
	typedef pair<Obiekt3w const* const, Sasiedzi_>		ParaSasiedzi_;
private:
	XMFLOAT3 const		sr;
	float const			r;
	void				dodajDoGalezie(Obiekt3w* const);
	void				dopiszSasiedzi(
							MapaSasiedzi_* const, ZbiorLiscie_* const
						) const;
	byte				liczObszary(Obiekt3w* const) const;
	void				przepiszDoGalezie();
	void				tworzGalezie();
public:
	UINT				test;
	Liscie_				liscie;
	Galezie_			galezie;
						Drzewo8(XMFLOAT3 const, float const);
	void				dodaj(Obiekt3w* const);
	void				wezSasiedzi(MapaSasiedzi_* const) const;
};

//class Drzewo8 {
//	typedef vector<XMFLOAT3 const>		WektorRogi_;
//	typedef set<Obiekt3w* const>		ZbiorOb3w_;
//public:
//	static UINT			test;
//	vector<Drzewo8>		galezie;
//	ZbiorOb3w_			liscie;
//	XMFLOAT3 const		zakresMaks;
//	XMFLOAT3 const		zakresMin;
//						Drzewo8(
//							float const, float const, float const,
//							float const, float const, float const
//						);
//	void				dodaj(Obiekt3w* const);
//	void				dodajDziecko(Obiekt3w* const);
//	void				dopiszSasiedzi(MapaSasiedzi_* const) const;
//	byte				liczNr(XMFLOAT3);
//	XMFLOAT3			liczPozLokal(XMFLOAT3) const;
//	void				przepiszObiektyDzieci();
//	void				tworzDzieci();
//	void				wezSasiedzi(MapaSasiedzi_* const) const;
//	void				wezRogiKula(
//							XMVECTOR const, float const, vector<XMFLOAT3>* const
//						) const;
//	void				wezRogiObiekt(
//							Obiekt3w const* const, WektorRogi_* const
//						) const;
//};



