#pragma once

#include "globalne.h"
#include "debug.h"
#include "typedefy.h"
#include "siatka.h"
#include "listaObiekty.h"

class IFizyka;
class IGrafika;

struct Wierzcholek {
	XMFLOAT3		poz;
	XMFLOAT2		pozTekstury;
					Wierzcholek(float, float, float);
					Wierzcholek(float, float, float, float, float);
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
public:
						IObiekt();
	virtual				~IObiekt();
	void				aktualizujSiatka();
	void				rysuj() const;
	virtual void		ustawFizyka() = 0;
	virtual void		ustawGrafika() = 0;
	IFizyka*			wezFiz();
	virtual void		wezKolizjePromien(
							MapaFloatObiekt_* const, XMVECTOR const, XMVECTOR const
						) const = 0;
	void				wezSiatka(SiatkaObiekty* const);
};

class Obiekt3W : public IObiekt {
	friend class Fizyka3W;
	friend class Fizyka3WKolizyjny;
	friend class Fizyka3WNieopozniony;
	friend class Grafika3WPodstawa;
	ID3D11Buffer*					bufIndeksy;
	ID3D11Buffer*					bufWierz;
	Fizyka3W*						fiz;
	vector<DWORD>					ind;
	XMFLOAT4X4						macPrzesun;
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
};

class ObiektZbior : public IObiekt {
	friend class FizykaZbior;
	friend class FizykaZbiorZalezny;
	friend class GrafikaZbiorPodstawa;
	FizykaZbior*		fiz;
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
};

