#pragma once

#include "globalne.h"
#include "ifizyka.h"
#include "obiekty.h"

class Fizyka3W : public virtual IFizyka3W {
public:
				~Fizyka3W();
	void		aktualizujParamFiz();
	void		aktualizujPoz();
	void		aktualizujSiatka();
	void		dodajPredkosc(XMVECTOR const);
	bool		sprawdzKolizjaBryly(Obiekt3W const* const) const;
	void		usunSwiatPkt(XMVECTOR* const) const;
	void		usunSwiatWektor(XMVECTOR* const) const;
	bool		wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const) const;
	void		wezKolidujaceZ3W(WektObiekty3W_* const, Obiekt3W const* const) const;
	void		wezKolizjePromien(
					MapaFloatObiekt_* const,
					XMVECTOR const, XMVECTOR const
				) const;
};

class Fizyka3WKolizyjny : public virtual IFizyka3W {
	void		wykonajKolizjaBryly(Obiekt3W const* const);
public:
	void		wykonajKolizjaSiatka(
					WektObiekty3W_* const, IObiekt const* const
				);
};

class Fizyka3WNiekolizyjny : public virtual IFizyka3W {
public:
	void		wykonajKolizjaSiatka(
					WektObiekty3W_* const, IObiekt const* const
				);
};
// ---------------------------------------------
class FizykaPostac : public Fizyka3W, public Fizyka3WKolizyjny {
public:
	FizykaPostac(Obiekt3W* const);
};
// =============================================
class FizykaZbior : public virtual IFizykaZbior {
public:
	virtual			~FizykaZbior();
	void			aktualizujSiatka();
	void			dodajPredkosc(XMVECTOR const);
	void			wezKolidujaceZ3W(
						WektObiekty3W_* const, Obiekt3W const* const
					) const;
};

class FizykaZbiorNiezalezny : public virtual IFizykaZbior {
};

class FizykaZbiorZalezny : public virtual IFizykaZbior {
public:
	void		aktualizujParamFiz();
	void		wezKolizjePromien(
					MapaFloatObiekt_* const,
					XMVECTOR const, XMVECTOR const
				) const;
	void		wykonajKolizjaSiatka(
					WektObiekty3W_* const, IObiekt const* const
				);
};
// ---------------------------------------------
class FizykaTekst : public FizykaZbior, public FizykaZbiorZalezny {
public:
	FizykaTekst(ObiektZbior* const);
};
// =============================================
