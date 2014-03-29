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
	virtual void		aktualizujParamFiz() = 0;
	virtual void		aktualizujSiatka() = 0;
	virtual void		dodajPredkosc(XMVECTOR const) = 0;
	bool				sprawdzKolizjaPudelkoPudelko(
							FXMVECTOR const, FXMVECTOR const,
							FXMVECTOR const, CXMVECTOR const
						) const;
	bool				sprawdzKolizjaTrojkatTrojkat(
							FXMVECTOR const, FXMVECTOR const, FXMVECTOR const,
							CXMVECTOR const, CXMVECTOR const, CXMVECTOR const
						) const;
	virtual void		wezKolidujaceZ3W(
							WektObiekty3W_* const, Obiekt3W const* const
						) const = 0;
	bool				wezKolizjaOdcinekTrojkat(
							float* const,
							FXMVECTOR const, FXMVECTOR const,
							FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
						) const;
	virtual void		wezKolizjePromien(
							MapaFloatObiekt_* const,
							XMVECTOR const, XMVECTOR const
						) const = 0;
	bool				wezKolizjaPromienTrojkat(
							float* const,
							FXMVECTOR const, FXMVECTOR const,
							FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
						) const;
	virtual void		wykonajKolizjaSiatka(
							WektObiekty3W_* const, IObiekt const* const
						) = 0;
};

class IFizyka3W : public IFizyka {
public:
						IFizyka3W();
						IFizyka3W(Obiekt3W* const);
	virtual				~IFizyka3W();
	Obiekt3W* const		obiekt;
	virtual void		aktualizujPoz() = 0;
	virtual bool		sprawdzKolizjaBryly(Obiekt3W const* const) const = 0;
	virtual void		usunSwiatPkt(XMVECTOR* const) const = 0;
	virtual void		usunSwiatWektor(XMVECTOR* const) const = 0;
	virtual bool		wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const) const = 0;
};

class IFizykaZbior : public IFizyka {
public:
							IFizykaZbior();
							IFizykaZbior(ObiektZbior* const);
	virtual					~IFizykaZbior();
	ObiektZbior* const		obiekt;
};


