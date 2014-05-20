#pragma once

#include "swiat.h"
#include "fizyka.h"
#include "grafika.h"
#include "drzewo8.h"

void Swiat::aktualizujMacProjekcja() {
	XMStoreFloat4x4(&macProjekcja, XMMatrixPerspectiveFovLH(
		katProjekcja*3.14f/180,
		float(szerRend)/wysRend,
		odlBlizszaPlaszcz,
		odlDalszaPlaszcz
	));
}
void Swiat::aktualizujMacWidok() {
	XMVECTOR w1 = XMLoadFloat3(&pozKamera);
	XMVECTOR w2 = XMLoadFloat3(&celKamera);
	XMVECTOR w3 = XMLoadFloat3(&goraKamera);

	XMStoreFloat4x4(&macWidok, XMMatrixLookAtLH(w1, w2, w3));
}
void Swiat::aktualizujSasiedzi() {
	logi.pisz("0", "");
	//SiatkaObiekty siatka;
	Drzewo8 drzewo8(XMFLOAT3(0,0,0), 10.0f);
	//Drzewo8 drzewo8(-10.0f, -10.0f, -10.0f, 10.0f, 10.0f, 10.0f);
	ListaObiekty::const_iterator itA;
	ZbiorOb3w_::const_iterator itB;
	ZbiorOb3w_ obiektyRob;
	for(itA = obiektySwiat.begin(); itA != obiektySwiat.end(); ++itA) {
		(*itA)->wezFiz()->wezObiekty3W(&obiektyRob);
		for(itB = obiektyRob.begin(); itB != obiektyRob.end(); ++itB) {
			//siatka.dodajObiekt(*itB);
			drzewo8.dodaj(*itB);
		}
	}
	logi.pisz("1", "");
	//logi.pisz("ttt", to_string(Drzewo8::il));
	//siatka.wezSasiedzi(&sasiedzi);
	drzewo8.wezSasiedzi(&sasiedzi);
	logi.pisz("2", "");
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
}
void Swiat::ustawBlizszaPlaszcz(float const odl) {
	odlBlizszaPlaszcz = odl;
}
void Swiat::ustawCelKamera(float const x, float const y, float const z) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat3(&celKamera, XMVectorSet(x, y, z, 0.0f));
}
void Swiat::ustawDalszaPlaszcz(float const odl) {
	odlDalszaPlaszcz = odl;
}
void Swiat::ustawGoraKamera(float const x, float const y, float const z) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat3(&goraKamera, XMVectorSet(x, y, z, 0.0f));
}
void Swiat::ustawKatProjekcja(float const kat) {
	katProjekcja = kat;
}
void Swiat::ustawPozycjaKamera(float const x, float const y, float const z) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat3(&pozKamera, XMVectorSet(x, y, z, 0.0f));
}
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
void Swiat::wezBlizszaPlaszczyzna(float* const odl) const {
	*odl = odlBlizszaPlaszcz;
}
void Swiat::wezPozKamera(XMVECTOR* const poz) const {
	*poz = XMLoadFloat3(&pozKamera);
}
Swiat::Swiat() {
	logi.piszStart("--->", "Tworzenie swiata.");

	pozKamera = XMFLOAT3(+0.0f, +0.0f, -0.5f);
	celKamera = XMFLOAT3(+0.0f, +0.0f, +0.0f);
	goraKamera = XMFLOAT3(+0.0f, +1.0f, +0.0f);
	aktualizujMacWidok();

	katProjekcja = 90;
	odlBlizszaPlaszcz = 0.5f;
	odlDalszaPlaszcz = 100.0f;
	aktualizujMacProjekcja();

	logi.piszStop("<---", "Tworzenie swiata.");
}
Swiat::~Swiat() {
	niszczObiektySwiat();
}
void Swiat::dodaj(IObiekt* const ob) {
	obiektySwiat.insert(ob);
	ob->ustawKolizje(&sasiedzi);
}
Obiekt3w* Swiat::tworzObiektKursor() {
	Wierzcholek wierzcholki[] = {
		Wierzcholek(+0.0f, -0.2f, +0.0f, +0.0f, +1.0f),
		Wierzcholek(+0.0f, +0.0f, +0.0f, +0.5f, +0.0f),
		Wierzcholek(+0.2f, -0.0f, +0.0f, +1.0f, +1.0f),
	};
	DWORD indeksy[] = {
		0, 1, 2
	};

	Obiekt3w* const ob = new Obiekt3w(
		wierzcholki, 3,
		indeksy, 3,
		XMFLOAT3(0,0,0),
		"tekstura\\t1.jpg"
	);
	dodaj(ob);
	return ob;
}
IObiekt* Swiat::tworzObiektRycerz() {
	Wierzcholek wierzcholki[] = {
		Wierzcholek(-0.25f, +0.0f, -0.25f, +0.0f, +0.5f),
		Wierzcholek(+0.25f, +0.0f, -0.25f, +0.5f, +0.5f),
		Wierzcholek(+0.25f, +0.0f, +0.25f, +1.0f, +0.5f),
		Wierzcholek(-0.25f, +0.0f, +0.25f, +1.0f, +1.0f),
		Wierzcholek(+0.0f, +0.5f, +0.0f, +0.5f, +0.0f),
		Wierzcholek(+0.0f, -0.5f, +0.0f, +0.5f, +1.0f),
	};
	DWORD indeksy[] = {
		0, 4, 1,
		1, 5, 0,
		1, 4, 2,
		2, 5, 1,
		2, 4, 3,
		3, 5, 2,
		3, 4, 0,
		0, 5, 3,
	};

	IObiekt* const ob = new Obiekt3w(
		wierzcholki, 6,
		indeksy, 24,
		//XMFLOAT3(-2.0f, +0.0f, +4.0f),
		XMFLOAT3(+0.0f, +0.0f, +0.0f),
		"tekstura\\t2.jpg"
	);
	dodaj(ob);
	return ob;
}
IObiekt* Swiat::tworzObiektSmok() {
	Wierzcholek wierzcholki[] = {
		Wierzcholek(+0.5f, -0.5f, +0.0f, +1.0f, +0.0f),
		Wierzcholek(-0.5f, -0.5f, +0.0f, +1.0f, +1.0f),
		Wierzcholek(-0.5f, +0.5f, +0.0f, +0.0f, +1.0f),
		Wierzcholek(+0.5f, +0.5f, +0.0f, +0.0f, +0.0f)
	};
	DWORD indeksy[] = {
		0, 1, 2,
		0, 2, 3
	};

	IObiekt* const ob = new Obiekt3w(
		wierzcholki, 4,
		indeksy, 6,
		//XMFLOAT3(+2.0f, +0.0f, +4.0f),
		XMFLOAT3(+0.0f, +0.0f, +0.0f),
		"tekstura\\t1.jpg"
	);
	dodaj(ob);
	return ob;
}
void Swiat::wezObPromien(IObiekt** const ob, Obiekt3w const* const obWybierajacy) const {
	*ob = NULL;
	if(obiektySwiat.size() == 0) {
		return;
	}

	// początek i kierunek promienia wyboru
	XMVECTOR pocz1 = XMLoadFloat3(&pozKamera);
	XMVECTOR kier1 = obWybierajacy->wezFiz()->wezPoz();
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
		(*it)->wezFiz()->liczPozycje();
		(*it)->wezFiz()->liczCzasRuch();
		(*it)->wezFiz()->wykonajRuchy();
	}
}
void Swiat::wykonajGrafika() {
	Obiekt3w::macProjekcja = macProjekcja;
	Obiekt3w::macWidok = macWidok;
	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		(*it)->wezGraf()->rysuj();
	}
}




