#pragma once
#include <swiat.h>
#include <fizyka.h>
#include <grafika.h>
#include "drzewo8.h"
// -------------------------------------------------------
Swiat::Swiat()
	: fiz(new Fizyka),
	graf(new Grafika) {
	zas.tworz_karta_rend_lanc();
	zas.tworz_gleb_szab_wid();
	zas.tworz_cel_rend_wid();
	zas.inic_wierz();
	zas.inic_teks();
	zas.inic_swiat();
	zas.inic_ind();
	zas.tworz_szad_wierz();
	zas.tworz_szad_piks();
	zas.tworz_strukt_we();
	zas.tworz_rzutnia();
	zas.tworz_stan_prob();
	zas.tworz_co_klat();
	zas.wiaz_cele_rend();
	zas.wiaz_szad_wierz();
	zas.wiaz_szad_piks();
	zas.wiaz_strukt_we();
	zas.wiaz_topol_prym();
	zas.wiaz_rzutnia();
	zas.wiaz_stan_prob();
	zas.wiaz_co_klat();
	wstaw_zad(Zad{AKTUAL_KAM, 0});
}
Swiat::~Swiat() {
	delete fiz;
	delete graf;
}
void Swiat::wyk_zad() {
	if(zad.wez_il_wier() > 1) {
		uint32_t* _mapa = 0;
		zad.uloz_licz(_mapa);
		zad.uloz_wyk(_mapa);
		zad.usun_dupl_licz(_mapa, FunHasz<uint8_t>(), FunHasz2<uint8_t>());
		zad.uloz_wyk(_mapa);
		free(_mapa);
	}

	for(uint32_t _i = 0; _i < zad.wez_il_wier(); ++_i) {
		if(zad.wez_wier(_i) == zad.pusty) continue;

		switch(((Zad*)zad[_i])->kod) {
		case USTAW_OB:
			if(!flg_zad[AKTUAL_SWIAT]) wstaw_zad(Zad{AKTUAL_SWIAT, 0});
			if(!flg_zad[RYSUJ]) wstaw_zad(Zad{RYSUJ, 0});
			break;
		case TWORZ_OB:
			if(!flg_zad[ULOZ_OB]) wstaw_zad(Zad{ULOZ_OB, 0});
			if(!flg_zad[AKTUAL_SWIAT]) wstaw_zad(Zad{AKTUAL_SWIAT, 0});
			if(!flg_zad[RYSUJ]) wstaw_zad(Zad{RYSUJ, 0});
			break;
		}
	}

	fiz->wyk_zad();
	graf->wyk_zad();
	flg_zad.czysc();
}
// -------------------------------------------------------

/*
Swiat::~Swiat() {
	niszczObiektySwiat();
}
void Swiat::aktualizujSasiedzi() {
	//logi.pisz("  #", "");
	Drzewo8 drzewo8(XMFLOAT3(0,0,0));
	ListaObiekty::const_iterator itA;
	ZbiorOb3w_::const_iterator itB;
	ZbiorOb3w_ obiektyRob;
	for(itA = obiektySwiat.begin(); itA != obiektySwiat.end(); ++itA) {
		(*itA)->wezFiz()->wezObiekty3W(&obiektyRob);
		for(itB = obiektyRob.begin(); itB != obiektyRob.end(); ++itB) {
			drzewo8.dodaj(*itB);
		}
	}
	//logi.pisz(" ##", "");
	sasiedzi.clear();
	drzewo8.wezSasiedzi(&sasiedzi);
	//logi.pisz("###", "");
}
void Swiat::dodaj(IObiekt* const ob) {
	obiektySwiat.insert(ob);
	ob->ustawSasiedzi(&sasiedzi);
}
void Swiat::niszczObiektSwiat(IObiekt* const ob) {
	if(obiektySwiat.count(ob) != 0) {
		obiektySwiat.erase(ob);
		delete ob;
	}
}
void Swiat::niszczObiektySwiat() {
	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ) {
		niszczObiektSwiat(*it++);
	}
}*/

/*
void Swiat::usunProjekcjaZ1(XMVECTOR* const pkt3W, float const x, float const y) const {
	// współrzędne 3W przy założeniu, że z = 1 (w ten sposób usuwamy projekcję)
	*pkt3W = XMVectorSetX(*pkt3W, x / macProjekcja._11);
	*pkt3W = XMVectorSetY(*pkt3W, y / macProjekcja._22);
	*pkt3W = XMVectorSetZ(*pkt3W, 1.0f);
}
void Swiat::usunWidokPkt(XMVECTOR* const pkt3W) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	*pkt3W = XMVector3TransformCoord(*pkt3W, macOdwrot);
}
void Swiat::usunWidokWektor(XMVECTOR* const wekt3W) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	*wekt3W = XMVector3TransformNormal(*wekt3W, macOdwrot);
}
void Swiat::wezObPromien(IObiekt** const ob, Obiekt3w const* const obWybierajacy) const {
	*ob = NULL;
	if(obiektySwiat.size() == 0) {
		return;
	}

	// początek i kierunek promienia wyboru
	XMVECTOR pocz1 = XMLoadFloat3(&pozKamera);
	XMVECTOR kier1 = obWybierajacy->wezFiz()->wezPrzes();
	kier1 = kier1 - pocz1;

	// obiekty kolidujące z promieniem wyboru, ułożone od najbliższych początku promienia do najdalszych
	ListaObiekty::const_iterator it;
	MapaFloatObiekt_ odlKolizje;
	MapaFloatObiekt_::const_iterator itKol;
	float odlMin = -1;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		if(*it == obWybierajacy) {
			continue;
		}

		(*it)->wezKolizjePromien(&odlKolizje, pocz1, kier1);
		if(odlKolizje.size() > 0) {
			itKol = odlKolizje.upper_bound(0);
			if(odlMin == -1 || itKol->first < odlMin) {
				odlMin = itKol->first;
				*ob = itKol->second;
			}
		}
	}
}
void Swiat::wykonajFizyka() {
	//logi.pisz("test", "przed");
	aktualizujSasiedzi();
	//logi.pisz("test", "po");

	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		(*it)->wezFiz()->liczSwiatyBezkol();
		(*it)->wezFiz()->liczCzasKolizja();
		(*it)->wezFiz()->liczSwiatyParam();
	}
}*/





