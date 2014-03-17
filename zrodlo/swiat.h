#ifndef _SWIAT_H_
#define _SWIAT_H_

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
	void				niszczObiektSwiat(IObiekt3W* const);
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
	void				tworzKolejnaKlatka();
	IObiekt3W*			tworzObiektKursor();
	IObiekt3W*			tworzObiektRycerz();
	IObiekt3W*			tworzObiektSmok();
};

#endif