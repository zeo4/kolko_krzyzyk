#ifndef _OBIEKTY_H_
#define _OBIEKTY_H_

#include "globalne.h"
#include "debug.h"
#include "fizyka.h"
#include "grafika.h"
#include "typedefy.h"
#include "listaObiekty.h"

class IFizyka;
class IGrafika;

struct Wierzcholek {
	XMFLOAT3		poz;
	XMFLOAT2		pozTekstury;
					Wierzcholek(float, float, float);
					Wierzcholek(float, float, float, float, float);
};

class SiatkaObiekty {
	typedef set<IObiekt3W* const>					ObiektyPole_;
	typedef map<float const, ObiektyPole_>			Siatka1Obiekty_;
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
	typedef ObiektyPole_::iterator					IteratorOb;
	typedef ObiektyPole_::const_iterator			StalyIteratorOb;
	void				dopiszObiekt(float, float, float, IObiekt3W* const);
	void				dopiszSiatka(SiatkaObiekty const);
	void				czysc();
	void				ustawWspolnyObiekt(IObiekt3W* const);
	void				wezKolizje(Kolizje_* const) const;
	bool				wezObiekty(
							set<IObiekt3W* const>* const, float, float, float
						) const;
	//StalyIteratorX		wezPocz() const;
	//StalyIteratorX		wezKon() const;
};

class IObiekt3W {
	friend class FizykaObiektZbiorPodstawa;
	friend class FizykaObiektZbiorZalezny;
	friend class GrafikaObiektZbiorPodstawa;
protected:
	IFizyka*			fiz;
	IGrafika*			graf;
	XMFLOAT4X4			macPrzesun;
	SiatkaObiekty		siatka;
	XMFLOAT3			v;
public:
						IObiekt3W();
	virtual				~IObiekt3W();
	void				aktualizujPoz();
	void				aktualizujSiatka();
	void				rysuj() const;
	void virtual		ustawFizyka() = 0;
	void virtual		ustawGrafika() = 0;
	void				wezSiatka(SiatkaObiekty* const);
	XMMATRIX			wezSwiat() const;
	void				wykonajZdarzKolizjaSiatka(IObiekt3W const* const);
	void virtual		wykonajZdarzRuch(XMVECTOR const) = 0;
};

class Obiekt3W : public IObiekt3W {
	friend class FizykaObiekt3WPodstawa;
	friend class FizykaObiekt3WNieopozniony;
	friend class GrafikaObiekt3WPodstawa;
	ID3D11Buffer*					bufIndeksy;
	ID3D11Buffer*					bufWierz;
	vector<DWORD>					ind;
	XMFLOAT4X4 const* const			macProjekcja;
	XMFLOAT4X4 const* const			macWidok;
	ID3D11ShaderResourceView*		widokTekstura;
	vector<Wierzcholek>				wierz;
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
									Obiekt3W(
										Wierzcholek const *const, UINT const,
										DWORD const *const, UINT const,
										string,
										XMFLOAT4X4 const* const,
										XMFLOAT4X4 const* const
									);
	virtual							~Obiekt3W();
	void virtual					ustawFizyka();
	void virtual					ustawGrafika();
	void virtual					wykonajZdarzRuch(XMVECTOR const);
};

class ObiektZbior : public IObiekt3W {
	friend class FizykaObiektZbiorPodstawa;
	friend class FizykaObiektZbiorZalezny;
	friend class GrafikaObiektZbiorPodstawa;
	ListaObiekty		podobiekty;
public:
						ObiektZbior(
							ListaObiekty::const_iterator const,
							ListaObiekty::const_iterator const
						);
	virtual				~ObiektZbior();
	void virtual		ustawFizyka();
};

#endif