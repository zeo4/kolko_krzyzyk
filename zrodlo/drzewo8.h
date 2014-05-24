# pragma once

#include "globalne.h"
#include "listaObiekty.h"
#include "typedefy.h"

class Obiekt3w;

class Drzewo8 {
	typedef multimap<uint64_t, Obiekt3w* const>			Liscie_;
	typedef pair<uint64_t, Obiekt3w* const>				ParaLiscie_;
	typedef multimap<uint64_t, Drzewo8* const>			Galezie_;
	typedef pair<uint64_t, Drzewo8* const>				ParaGalezie_;
	typedef set<Obiekt3w const* const>					Sasiedzi_;
	typedef map<Obiekt3w const* const, Sasiedzi_>		MapaSasiedzi_;
	typedef pair<Obiekt3w const* const, Sasiedzi_>		ParaSasiedzi_;
private:
	Galezie_				galezie;
	Liscie_					liscie;
	XMFLOAT3 const			przes;
	UINT const				r;
	XMFLOAT3 const			sr;
	static uint32_t			xMorton[256];
	static uint32_t			yMorton[256];
	static uint32_t			zMorton[256];
	uint64_t				liczMorton(UINT const, UINT const, UINT const) const;
public:
							Drzewo8(XMFLOAT3 const, UINT const);
	void					dodaj(Obiekt3w* const);
	void					wezSasiedzi(MapaSasiedzi_* const) const;
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



