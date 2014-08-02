#pragma once

#include "swiat.h"
#include "fizyka.h"
#include "grafika.h"
#include "drzewo8.h"

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
void Swiat::inic() {
	_graf.przypisz(&_ob);
	wgraj_ob();
	_graf.wyk_raz();
}
void Swiat::rysuj() {
	_graf.czysc();
	_graf.wyk_co_klatka();
}
void Swiat::wgraj_ob() {
	// kursor
	XMFLOAT3 kurs_wierz[] = {
		XMFLOAT3(+0.0f, -0.2f, +0.0f),
		XMFLOAT3(+0.0f, +0.0f, +0.0f),
		XMFLOAT3(+0.2f, -0.0f, +0.0f),
	};
	XMFLOAT2 kurs_wierz_teks[] = {
		XMFLOAT2(+0.0f, +1.0f),
		XMFLOAT2(+0.5f, +0.0f),
		XMFLOAT2(+1.0f, +1.0f),
	};
	DWORD kurs_ind[] = {0, 1, 2};
	_ob.tworz_ob(kurs_wierz, kurs_wierz_teks, 3, kurs_ind, 3, 1);

	// rycerz
	XMFLOAT3 ryc_wierz[] = {
		XMFLOAT3(-0.25f, +0.0f, -0.25f),
		XMFLOAT3(+0.25f, +0.0f, -0.25f),
		XMFLOAT3(+0.25f, +0.0f, +0.25f),
		XMFLOAT3(-0.25f, +0.0f, +0.25f),
		XMFLOAT3(+0.0f, +0.5f, +0.0f),
		XMFLOAT3(+0.0f, -0.5f, +0.0f),
	};
	XMFLOAT2 ryc_wierz_teks[] = {
		XMFLOAT2(+0.0f, +0.5f),
		XMFLOAT2(+0.5f, +0.5f),
		XMFLOAT2(+1.0f, +0.5f),
		XMFLOAT2(+1.0f, +1.0f),
		XMFLOAT2(+0.5f, +0.0f),
		XMFLOAT2(+0.5f, +1.0f),
	};
	DWORD ryc_ind[] = {
		0, 4, 1,
		1, 5, 0,
		1, 4, 2,
		2, 5, 1,
		2, 4, 3,
		3, 5, 2,
		3, 4, 0,
		0, 5, 3,
	};
	_ob.tworz_ob(ryc_wierz, ryc_wierz_teks, 6, ryc_ind, 24, 2);

	// smok
	XMFLOAT3 smok_wierz[] = {
		XMFLOAT3(+0.5f, -0.5f, +0.0f),
		XMFLOAT3(-0.5f, -0.5f, +0.0f),
		XMFLOAT3(-0.5f, +0.5f, +0.0f),
		XMFLOAT3(+0.5f, +0.5f, +0.0f),
	};
	XMFLOAT2 smok_wierz_teks[] = {
		XMFLOAT2(+1.0f, +0.0f),
		XMFLOAT2(+1.0f, +1.0f),
		XMFLOAT2(+0.0f, +1.0f),
		XMFLOAT2(+0.0f, +0.0f),
	};
	DWORD smok_ind [] = {
		0, 1, 2,
		0, 2, 3
	};
	_ob.tworz_ob(smok_wierz, smok_wierz_teks, 4, smok_ind, 6, 1);
}
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





