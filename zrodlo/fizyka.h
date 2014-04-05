#pragma once

#include "globalne.h"
#include "typedefy.h"

class Obiekt3W;

class IFizyka {
	friend class Fizyka3WKolizyjny;
	friend class Fizyka3WNiekolizyjny;
	friend class ObiektZbior;
	friend class FizykaZbior;
protected:
	virtual				~IFizyka();
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
public:
	virtual void		aktualizujSiatka() = 0;
	virtual void		dodajPredkosc(XMVECTOR const) = 0;
	virtual void		wykonajKolizjaSiatka(
							WektObiekty3W_* const, IObiekt const* const
						) = 0;
};

class Fizyka3W : public IFizyka {
	friend class Obiekt3W;
protected:
	Obiekt3W* const		obiekt;
						Fizyka3W(Obiekt3W* const);
						~Fizyka3W();
	bool				sprawdzKolizjaBryly(Obiekt3W const* const) const;
	void				usunSwiatPkt(XMVECTOR* const) const;
	void				usunSwiatWektor(XMVECTOR* const) const;
	bool				wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const) const;
	void				wezKolidujaceZ3W(
							WektObiekty3W_* const, Obiekt3W const* const
						) const;
	void				wezKolizjePromien(
							MapaFloatObiekt_* const,
							XMVECTOR const, XMVECTOR const
						) const;
public:
	void				aktualizujSiatka();
	void				dodajPredkosc(XMVECTOR const);
};

class Fizyka3WKolizyjny : public virtual Fizyka3W {
	void		wykonajKolizjaBryly(Obiekt3W const* const);
public:
				Fizyka3WKolizyjny();
	void		wykonajKolizjaSiatka(
					WektObiekty3W_* const, IObiekt const* const
				);
};

class Fizyka3WNiekolizyjny : public virtual Fizyka3W {
public:
				Fizyka3WNiekolizyjny();
	void		wykonajKolizjaSiatka(
					WektObiekty3W_* const, IObiekt const* const
				);
};
// ---------------------------------------------
class FizykaPostac : public virtual Fizyka3W, public Fizyka3WKolizyjny {
	friend class Obiekt3W;
protected:
	FizykaPostac(Obiekt3W* const);
public:
};
// =============================================
class FizykaZbior : public IFizyka {
	friend class ObiektZbior;
protected:
	ObiektZbior* const		obiekt;
							FizykaZbior(ObiektZbior* const);
	virtual					~FizykaZbior();
	void					aktualizujSiatka();
	void					wezKolidujaceZ3W(
								WektObiekty3W_* const, Obiekt3W const* const
							) const;
public:
	void					dodajPredkosc(XMVECTOR const);
};

class FizykaZbiorNiezalezny : public virtual FizykaZbior {
};

class FizykaZbiorZalezny : public virtual FizykaZbior {
protected:
	void		wezKolizjePromien(
					MapaFloatObiekt_* const,
					XMVECTOR const, XMVECTOR const
				) const;
public:
				FizykaZbiorZalezny();
	void		wykonajKolizjaSiatka(
					WektObiekty3W_* const, IObiekt const* const
				);
};
// ---------------------------------------------
class FizykaTekst : public virtual FizykaZbior, public FizykaZbiorZalezny {
	friend class ObiektZbior;
protected:
	FizykaTekst(ObiektZbior* const);
public:
};
// =============================================
