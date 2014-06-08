#pragma once

#include "globalne.h"
#include "debug.h"
#include "typedefy.h"
#include "siatka.h"
#include "listaObiekty.h"

class IFizyka;
class Fizyka3w;
class IGrafika;
class Grafika3w;
class Drzewo8;

struct Wierzcholek {
	XMFLOAT3		poz;
	XMFLOAT2		pozTekstury;
					Wierzcholek(float, float, float);
					Wierzcholek(float, float, float, float, float);
};

class IObiekt {
	friend class IFizyka;
	friend class Fizyka3WKolizyjny;
	friend class Fizyka3WNiekolizyjny;
	friend class FizykaZbior;
	friend class FizykaZbiorZalezny;
	friend class GrafikaZbior;
	friend class Swiat;
protected:
	IFizyka*			fiz;
	IGrafika*			graf;
	XMFLOAT4X4			macSwiat;
	XMFLOAT4X4			macSwiatBezkol;
	XMFLOAT3			obrot;
	XMFLOAT3			omega;
	XMFLOAT3			przes;
	IObiekt const*		rodzic;
	float				tKolizja;
	XMFLOAT3			v;
public:
						IObiekt(XMFLOAT3 const);
	virtual				~IObiekt();
	void				rysuj() const;
	virtual void		ustawGrafika() = 0;
	virtual void		ustawSasiedzi(MapaSasiedzi_ const* const) = 0;
	IFizyka*			wezFiz() const;
	IGrafika*			wezGraf() const;
	virtual void		wezKolizjePromien(
							MapaFloatObiekt_* const, XMVECTOR const, XMVECTOR const
						) const = 0;
};

class Obiekt3w : public IObiekt {
	friend class Fizyka3w;
	friend class Fizyka3wKoliz;
	friend class Fizyka3wNiekoliz;
	friend class Grafika3w;
	ID3D11Buffer*					bufIndeksy;
	ID3D11Buffer*					bufWierz;
	Fizyka3w*						fiz;
	vector<DWORD>					ind;
	MapaSasiedzi_ const*			sasiedzi;
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
	static IObiekt*					test;
	XMFLOAT4X4 static				macProjekcja;
	XMFLOAT4X4 static				macWidok;
									Obiekt3w(
										Wierzcholek const *const, UINT const,
										DWORD const *const, UINT const,
										XMFLOAT3 const,
										string);
	virtual							~Obiekt3w();
	void							ustawFizykaLitera();
	void							ustawFizykaPostac();
	void							ustawGrafika();
	virtual void					ustawSasiedzi(MapaSasiedzi_ const* const);
	Fizyka3w*						wezFiz() const;
	void							wezKolizjePromien(
										MapaFloatObiekt_* const,
										XMVECTOR const, XMVECTOR const) const;
};

class ObiektZbior : public IObiekt {
	friend class FizykaZbior;
	friend class FizykaZbiorZalezny;
	friend class GrafikaZbior;
	FizykaZbior*		fiz;
	ListaObiekty		podobiekty;
public:
						ObiektZbior(XMFLOAT3 const);
	virtual				~ObiektZbior();
	void				dodaj(IObiekt* const);
	void				ustawFizykaTekst();
	void				ustawGrafika();
	void				ustawSasiedzi(MapaSasiedzi_ const* const);
	void				wezKolizjePromien(
							MapaFloatObiekt_* const, XMVECTOR const, XMVECTOR const
						) const;
};

