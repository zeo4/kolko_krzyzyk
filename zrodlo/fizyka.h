#pragma once

#include "globalne.h"
#include "ifizyka.h"
#include "obiekty.h"

class Fizyka3W : public virtual IFizyka3W {
public:
				~Fizyka3W();
	void		aktualizujSiatka();
	void		dodajPredkosc(XMVECTOR const);
	bool		sprawdzKolizjaBryly(Obiekt3W const* const) const;
	bool		sprawdzKolizjaModele(IObiekt const* const) const;
	void		usunSwiatPkt(XMVECTOR* const) const;
	void		usunSwiatWektor(XMVECTOR* const) const;
	bool		wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const) const;
	void		wezKolidujaceZ3W(WektObiekty3W_* const, Obiekt3W const* const) const;
	bool		wezKolizjePromien(
							set<float>* const,
							XMVECTOR const, XMVECTOR const
						) const;
	void		wezWierzcholkiSwiat(vector<XMFLOAT3>* const) const;
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

class Fizyka3WNieopozniony : public virtual IFizyka3W {
public:
	void		aktualizujPoz();
};
// ---------------------------------------------
class FizykaLitera : public Fizyka3W, public Fizyka3WNiekolizyjny, public Fizyka3WNieopozniony {
public:
	FizykaLitera(Obiekt3W* const);
};

class FizykaPostac : public Fizyka3W, public Fizyka3WKolizyjny, public Fizyka3WNieopozniony {
public:
	FizykaPostac(Obiekt3W* const);
};
// =============================================
class FizykaZbior : public virtual IFizykaZbior {
public:
	virtual					~FizykaZbior();
	void virtual			aktualizujSiatka();
	void virtual			dodajPredkosc(XMVECTOR const);
	bool virtual			sprawdzKolizjaBryly(IObiekt const* const) const;
	bool virtual			sprawdzKolizjaModele(IObiekt const* const) const;
	void virtual			usunSwiatPkt(XMVECTOR* const) const;
	void virtual			usunSwiatWektor(XMVECTOR* const) const;
	bool virtual			wezKolizjePromien(
								set<float>* const,
								XMVECTOR const, XMVECTOR const
							) const;
	void virtual			wezWierzcholkiSwiat(vector<XMFLOAT3>* const) const;
};

class FizykaZbiorZalezny : public virtual IFizykaZbior {
	void virtual		wykonajKolizjaSiatka(
							WektObiekty3W_* const, IObiekt const* const
						);
};

class FizykaZbiorNiezalezny : public virtual IFizykaZbior {
};
// ---------------------------------------------
class FizykaTekst : public FizykaZbior, public FizykaZbiorZalezny {
public:
	FizykaTekst(ObiektZbior* const);
};
// =============================================
