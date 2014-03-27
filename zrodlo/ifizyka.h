#pragma once

#include "globalne.h"
#include "debug.h"
#include "typedefy.h"

class IObiekt;
class Obiekt3W;
class ObiektZbior;

class IFizyka {
public:
	virtual				~IFizyka();
	void virtual		aktualizujPoz() = 0;
	void virtual		aktualizujSiatka() = 0;
	void virtual		dodajPredkosc(XMVECTOR const) = 0;
	bool virtual		sprawdzKolizjaModele(IObiekt const* const) const = 0;
	bool				sprawdzKolizjaPudelkoPudelko(
							FXMVECTOR const, FXMVECTOR const,
							FXMVECTOR const, CXMVECTOR const
						) const;
	bool				sprawdzKolizjaTrojkatTrojkat(
							FXMVECTOR const, FXMVECTOR const, FXMVECTOR const,
							CXMVECTOR const, CXMVECTOR const, CXMVECTOR const
						) const;
	void virtual		usunSwiatPkt(XMVECTOR* const) const = 0;
	void virtual		usunSwiatWektor(XMVECTOR* const) const = 0;
	void virtual		wezKolidujaceZ3W(
							WektObiekty3W_* const, Obiekt3W const* const
						) const = 0;
	bool				wezKolizjaOdcinekTrojkat(
							float* const,
							FXMVECTOR const, FXMVECTOR const,
							FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
						) const;
	bool virtual		wezKolizjePromien(
							set<float>* const,
							XMVECTOR const, XMVECTOR const
						) const = 0;
	bool				wezKolizjaPromienTrojkat(
							float* const,
							FXMVECTOR const, FXMVECTOR const,
							FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
						) const;
	void virtual		wezWierzcholkiSwiat(
							vector<XMFLOAT3>* const
						) const = 0;
	void virtual		wykonajKolizjaSiatka(
							WektObiekty3W_* const, IObiekt const* const
						) = 0;
};

class IFizyka3W : public IFizyka {
public:
						IFizyka3W();
						IFizyka3W(Obiekt3W* const);
	virtual				~IFizyka3W();
	Obiekt3W* const		obiekt;
	bool virtual		sprawdzKolizjaBryly(Obiekt3W const* const) const = 0;
	bool virtual		wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const) const = 0;
};

class IFizykaZbior : public IFizyka {
public:
							IFizykaZbior();
							IFizykaZbior(ObiektZbior* const);
	virtual					~IFizykaZbior();
	ObiektZbior* const		obiekt;
};


