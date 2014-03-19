#ifndef _FIZYKA_H_
#define _FIZYKA_H_

#include "globalne.h"
#include "debug.h"
#include "obiekty.h"
#include "typedefy.h"
#include "listaObiekty.h"

class SiatkaObiekty;
class IObiekt3W;
class Obiekt3W;
class ObiektZbior;

class IFizyka {
public:
	virtual						~IFizyka();
	void virtual				aktualizujPoz() = 0;
	void virtual				aktualizujSiatka() = 0;
	void virtual				dodajPredkosc(XMVECTOR const) = 0;
	bool virtual				sprawdzKolizjaBryly(IObiekt3W const* const) const = 0;
	bool virtual				sprawdzKolizjaModele(
									IObiekt3W const* const
								) const = 0;
	bool						sprawdzKolizjaTrojkatTrojkat(
									FXMVECTOR const, FXMVECTOR const, FXMVECTOR const,
									CXMVECTOR const, CXMVECTOR const, CXMVECTOR const
								) const;
	void virtual				usunSwiatPkt(XMVECTOR* const) const = 0;
	void virtual				usunSwiatWektor(XMVECTOR* const) const = 0;
	bool						wezKolizjaOdcinekTrojkat(
									float* const,
									FXMVECTOR const, FXMVECTOR const,
									FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
								) const;
	bool virtual				wezKolizjePromien(
									set<float>* const,
									XMVECTOR const, XMVECTOR const
								) const = 0;
	bool						wezKolizjaPromienTrojkat(
									float* const,
									FXMVECTOR const, FXMVECTOR const,
									FXMVECTOR const, CXMVECTOR const, CXMVECTOR const
								) const;
	void virtual				wezWierzcholkiSwiat(
									vector<XMFLOAT3>* const
								) const = 0;
	RaportKolizja virtual		wykonajZdarzKolizjaSiatka(IObiekt3W const* const) = 0;
};

class FizykaObiekt3WPodstawa : public IFizyka {
	bool				wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const) const;
	bool				wezLewyDolnyBliski(XMVECTOR* const) const;
public:
	Obiekt3W* const		obiekt;
						FizykaObiekt3WPodstawa();
						FizykaObiekt3WPodstawa(Obiekt3W* const);
	virtual				~FizykaObiekt3WPodstawa();
	void virtual		aktualizujSiatka();
	void virtual		dodajPredkosc(XMVECTOR const);
	bool virtual		sprawdzKolizjaBryly(IObiekt3W const* const) const;
	bool virtual		sprawdzKolizjaModele(IObiekt3W const* const) const;
	void virtual		usunSwiatPkt(XMVECTOR* const) const;
	void virtual		usunSwiatWektor(XMVECTOR* const) const;
	bool virtual		wezKolizjePromien(
							set<float>* const,
							XMVECTOR const, XMVECTOR const
						) const;
	void virtual		wezWierzcholkiSwiat(vector<XMFLOAT3>* const) const;
};

class FizykaObiekt3WKolizyjny : public virtual FizykaObiekt3WPodstawa {
public:
	RaportKolizja virtual		wykonajZdarzKolizjaSiatka(IObiekt3W const* const);
};

class FizykaObiekt3WNiekolizyjny : public virtual FizykaObiekt3WPodstawa {
public:
	RaportKolizja virtual		wykonajZdarzKolizjaSiatka(IObiekt3W const* const);
};

class FizykaObiekt3WNieopozniony : public virtual FizykaObiekt3WPodstawa {
public:
	void virtual		aktualizujPoz();
};

class FizykaObiekt3W : public FizykaObiekt3WKolizyjny, public FizykaObiekt3WNieopozniony {
public:
	FizykaObiekt3W(Obiekt3W* const);
};

class FizykaObiektZbiorPodstawa : public IFizyka {
protected:
	ObiektZbior* const		obiekt;
public:
							FizykaObiektZbiorPodstawa(ObiektZbior* const);
	virtual					~FizykaObiektZbiorPodstawa();
	void virtual			aktualizujSiatka();
	void virtual			dodajPredkosc(XMVECTOR const);
	bool virtual			sprawdzKolizjaBryly(IObiekt3W const* const) const;
	bool virtual			sprawdzKolizjaModele(IObiekt3W const* const) const;
	void virtual			usunSwiatPkt(XMVECTOR* const) const;
	void virtual			usunSwiatWektor(XMVECTOR* const) const;
	bool virtual			wezKolizjePromien(
								set<float>* const,
								XMVECTOR const, XMVECTOR const
							) const;
	void virtual			wezWierzcholkiSwiat(vector<XMFLOAT3>* const) const;
};

class FizykaObiektZbiorZalezny : public virtual FizykaObiektZbiorPodstawa {
	RaportKolizja virtual		wykonajZdarzKolizjaSiatka(IObiekt3W const* const);
};

class FizykaObiektZbiorNiezalezny : public virtual FizykaObiektZbiorPodstawa {
};

#endif