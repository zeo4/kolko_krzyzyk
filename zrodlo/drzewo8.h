#pragma once

#include "globalne.h"
#include "listaObiekty.h"
#include "typedefy.h"

class Obiekt3w;

class Drzewo8 {
	typedef multimap<uint64_t, Obiekt3w* const>			Liscie_;
	typedef pair<uint64_t, Obiekt3w* const>				ParaLiscie_;
	typedef multimap<uint64_t, Drzewo8* const>			Galezie_;
	typedef pair<uint64_t, Drzewo8* const>				ParaGalezie_;
private:
	Galezie_				galezie;
	Liscie_					liscie;
	XMFLOAT3 const			pktMin;
	UINT const				r;
	XMFLOAT3 const			sr;
	static uint32_t			xMorton[256];
	static uint32_t			yMorton[256];
	static uint32_t			zMorton[256];
	uint64_t				liczMorton(UINT const, UINT const, UINT const) const;
public:
							Drzewo8(XMFLOAT3 const);
	void					czysc();
	void					dodaj(Obiekt3w* const);
	UINT					wezRozm() const;
	void					wezSasiedzi(MapaSasiedzi_* const) const;
};





