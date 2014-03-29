#pragma once

#include "globalne.h"
#include "debug.h"
#include "fizyka.h"
#include "grafika.h"
#include "typedefy.h"
#include "listaObiekty.h"
#include "ifizyka.h"

class IGrafika;

struct Wierzcholek {
	XMFLOAT3		poz;
	XMFLOAT2		pozTekstury;
					Wierzcholek(float, float, float);
					Wierzcholek(float, float, float, float, float);
};

class SiatkaObiekty {
	typedef set<IObiekt* const>						ObiektyObszar_;
	typedef map<float const, ObiektyObszar_>		Siatka1Obiekty_;
	typedef map<float const, Siatka1Obiekty_>		Siatka2Obiekty_;
	typedef map<float const, Siatka2Obiekty_>		Siatka3Obiekty_;
	Siatka3Obiekty_		siatka;
public:
	typedef Siatka3Obiekty_::iterator				IteratorX;
	typedef Siatka3Obiekty_::const_iterator			StalyIteratorX;
	typedef Siatka2Obiekty_::iterator				IteratorY;
	typedef Siatka2Obiekty_::const_iterator			StalyIteratorY;
	typedef Siatka1Obiekty_::iterator				IteratorZ;
	typedef Siatka1Obiekty_::const_iterator			StalyIteratorZ;
	typedef ObiektyObszar_::iterator				IteratorOb;
	typedef ObiektyObszar_::const_iterator			StalyIteratorOb;
	void				dopiszObiekt(float, float, float, IObiekt* const);
	void				dopiszSiatka(SiatkaObiekty const);
	void				czysc();
	void				ustawWspolnyObiekt(IObiekt* const);
	void				wezKolizje(MapaKolizje_* const) const;
	bool				wezObiekty(
							set<IObiekt* const>* const, float, float, float
						) const;
	//StalyIteratorX		wezPocz() const;
	//StalyIteratorX		wezKon() const;
};

class IObiekt {
	friend class Fizyka3WKolizyjny;
	friend class Fizyka3WNiekolizyjny;
	friend class FizykaZbior;
	friend class FizykaZbiorZalezny;
	friend class GrafikaZbiorPodstawa;
protected:
	IFizyka*			fiz;
	IGrafika*			graf;
	SiatkaObiekty		siatka;
	XMFLOAT3			v;
	XMFLOAT3			vRodzic;
public:
						IObiekt();
	virtual				~IObiekt();
	void				aktualizujParamFiz();
	void				aktualizujSiatka();
	void				rysuj() const;
	virtual void		ustawFizyka() = 0;
	virtual void		ustawGrafika() = 0;
	virtual void		wezKolizjePromien(
							MapaFloatObiekt_* const, XMVECTOR const, XMVECTOR const
						) const = 0;
	void				wezSiatka(SiatkaObiekty* const);
	void				wykonajKolizjaSiatka(IObiekt const* const);
	virtual void		wykonajRuch(XMVECTOR const) = 0;
};

class Obiekt3W : public IObiekt {
	friend class Fizyka3W;
	friend class Fizyka3WKolizyjny;
	friend class Fizyka3WNieopozniony;
	friend class Grafika3WPodstawa;
	ID3D11Buffer*					bufIndeksy;
	ID3D11Buffer*					bufWierz;
	IFizyka3W*						fiz;
	vector<DWORD>					ind;
	XMFLOAT4X4						macPrzesun;
	ID3D11ShaderResourceView*		widokTekstura;
	vector<Wierzcholek>				wierz;
	void							aktualizujPoz();
	void							nadpiszIndeksy(
										DWORD const *const,
										UINT const
									);
	void							nadpiszWierzcholki(
										Wierzcholek const *const,
										UINT const
									);
	void							tworzBufIndeksy();
	void							tworzBufWierz();
	void							tworzWidokTekstura(string);
	void							usunBufIndeksy();
	void							usunBufWierz();
	void							usunWidokTekstura();
	void							wiazIndeksy() const;
	void							wiazTeksture() const;
	void							wiazWierz() const;
	void							wypelnijBufIndeksy();
	void							wypelnijBufWierz();
public:
	XMFLOAT4X4 static				macProjekcja;
	XMFLOAT4X4 static				macWidok;
									Obiekt3W(
										Wierzcholek const *const, UINT const,
										DWORD const *const, UINT const,
										string
									);
	virtual							~Obiekt3W();
	void							ustawFizyka();
	void							ustawGrafika();
	void							wezKolizjePromien(
										MapaFloatObiekt_* const,
										XMVECTOR const, XMVECTOR const
									) const;
	XMVECTOR						wezPoz() const;
	XMMATRIX						wezSwiat() const;
	void							wykonajRuch(XMVECTOR const);
};

class ObiektZbior : public IObiekt {
	friend class FizykaZbior;
	friend class FizykaZbiorZalezny;
	friend class GrafikaZbiorPodstawa;
	ListaObiekty		podobiekty;
public:
						ObiektZbior();
	virtual				~ObiektZbior();
	void				dodaj(IObiekt* const);
	void				ustawFizyka();
	void				ustawGrafika();
	void				wezKolizjePromien(
							MapaFloatObiekt_* const, XMVECTOR const, XMVECTOR const
						) const;
	void				wykonajRuch(XMVECTOR const);
};

