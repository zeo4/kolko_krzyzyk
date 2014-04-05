#pragma once

#include "globalne.h"
#include "debug.h"
#include "obiekty.h"
#include "listaObiekty.h"

class Swiat {
private:
	XMFLOAT3			celKamera;
	XMFLOAT3			goraKamera;
	float				katProjekcja;
	XMFLOAT4X4			macProjekcja;
	XMFLOAT4X4			macWidok;
	ListaObiekty		obiektySwiat;
	float				odlBlizszaPlaszcz;
	float				odlDalszaPlaszcz;
	XMFLOAT3			pozKamera;
	void				aktualizujMacProjekcja();
	void				aktualizujMacWidok();
	void				niszczObiektSwiat(IObiekt* const);
	void				niszczObiektySwiat();
	void				ustawBlizszaPlaszcz(float const);
	void				ustawCelKamera(
								float const, float const, float const
						);
	void				ustawDalszaPlaszcz(float const);
	void				ustawGoraKamera(
								float const, float const, float const
						);
	void				ustawKatProjekcja(float const);
	void				ustawPozycjaKamera(
								float const, float const, float const
						);
	void				usunProjekcjaZ1(
							XMVECTOR* const, float const, float const
						) const;
	void				usunWidokPkt(XMVECTOR* const) const;
	void				usunWidokWektor(XMVECTOR* const) const;
	void				wezBlizszaPlaszczyzna(float* const) const;
	void				wezPozKamera(XMVECTOR* const) const;
	void				wykonajKolizjeSiatka() const;
public:
						Swiat();
						~Swiat();
	void				dodaj(IObiekt* const);
	void				tworzKolejnaKlatka();
	Obiekt3W*			tworzObiektKursor();
	IObiekt*			tworzObiektRycerz();
	IObiekt*			tworzObiektSmok();
	void				wezObPromien(IObiekt** const, Obiekt3W const* const) const;
};



