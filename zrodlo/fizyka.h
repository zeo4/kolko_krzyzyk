#pragma once

#include "globalne.h"
#include "typedefy.h"

class Obiekt3w;
class SiatkaObiekty;

class IFizyka {
	friend class Fizyka3WKolizyjny;
	friend class Fizyka3WNiekolizyjny;
	friend class ObiektZbior;
	friend class FizykaZbior;
protected:
	IObiekt* const			obiekt;
							IFizyka(IObiekt* const);
	virtual					~IFizyka();
	void					liczPoz();
	bool					sprawdzKulaKula(
								FXMVECTOR const, float const, FXMVECTOR const,
								FXMVECTOR const, float const, CXMVECTOR const,
								float* const
							) const;
	bool					sprawdzPromienKula(
								XMVECTOR const, XMVECTOR const,
								XMVECTOR const, float const,
								float* const
							) const;
	bool					sprawdzKolizjaPudelkoPudelko(
								FXMVECTOR const, FXMVECTOR const,
								FXMVECTOR const, CXMVECTOR const
							) const;
	bool					sprawdzKolizjaTrojkatTrojkat(
								FXMVECTOR const, FXMVECTOR const, FXMVECTOR const,
								CXMVECTOR const, CXMVECTOR const, CXMVECTOR const
							) const;
	bool					wezKolizjaOdcinekTrojkat(
								float* const,
								FXMVECTOR const, FXMVECTOR const,
								FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
							) const;
	virtual void			wezKolizjePromien(
								MapaFloatObiekt_* const,
								XMVECTOR const, XMVECTOR const
							) const = 0;
	bool					wezKolizjaPromienTrojkat(
								float* const,
								FXMVECTOR const, FXMVECTOR const,
								FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
							) const;
	void					wykonajRuch();
public:
	virtual void			liczPozycje() = 0;
	virtual void			uwzglednijKolizje() = 0;
	virtual void			wezObiekty3W(ZbiorOb3w_* const) const = 0;
	XMVECTOR				wezPoz() const;
	void					wezPrzesunMacierz(
								XMFLOAT4X4 const, XMVECTOR* const
							) const;
	virtual void			wezSiatka(SiatkaObiekty* const) const = 0;
	virtual void			wykonajRuchy() = 0;
	virtual void			zadajRuch(
								XMVECTOR const, float const, float const, float const
							) = 0;
};

// #############################################

class Fizyka3w : public IFizyka {
	friend class Obiekt3w;
protected:
	Obiekt3w* const		obiekt;
	void				usunSwiatPkt(XMVECTOR* const) const;
	void				usunSwiatWektor(XMVECTOR* const) const;
	void				wezKolizjePromien(
							MapaFloatObiekt_* const,
							XMVECTOR const, XMVECTOR const
						) const;
public:
						Fizyka3w(Obiekt3w* const);
						~Fizyka3w();
	void				liczPozycje();
	void				uwzglednijKolizje();
	void				wezBrylaGraniczna(
							XMVECTOR* const, XMVECTOR* const, float* const
						) const;
	void				wezObiekty3W(ZbiorOb3w_* const) const;
	void				wezSiatka(SiatkaObiekty* const) const;
	void				wykonajRuchy();
	void				zadajRuch(
							XMVECTOR const, float const, float const, float const
						);
};

// ---------------------------------------------

class FizykaPostac : public virtual Fizyka3w {
	friend class Obiekt3w;
protected:
	FizykaPostac(Obiekt3w* const);
public:
};

// #############################################

class FizykaZbior : public IFizyka {
	friend class ObiektZbior;
protected:
	ObiektZbior* const		obiekt;
public:
							FizykaZbior(ObiektZbior* const);
	virtual					~FizykaZbior();
	void					liczPozycje();
	void					uwzglednijKolizje();
	void					wezObiekty3W(ZbiorOb3w_* const) const;
	void					wezSiatka(SiatkaObiekty* const) const;
	void					wykonajRuchy();
	void					zadajRuch(
								XMVECTOR const, float const, float const, float const
							);
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
};
// ---------------------------------------------

class FizykaTekst : public virtual FizykaZbior, public FizykaZbiorZalezny {
	friend class ObiektZbior;
protected:
	FizykaTekst(ObiektZbior* const);
public:
};

// #############################################
