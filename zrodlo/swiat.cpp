#ifndef _SWIAT_C_
#define _SWIAT_C_

#include "swiat.h"

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
	logi.pisz("OK", "Ustaw widok kamery sceny.");
}
void Swiat::niszczObiektSwiat(
	IObiekt3W* const		ob
	) {
	if(obiektySwiat.count(ob) == 0) {
		logi.pisz("?", "Niszczenie obiektu swiata: Nie ma takiego obiektu na liscie.");
	} else {
		obiektySwiat.erase(ob);
		delete ob;
	}
}
void Swiat::niszczObiektySwiat() {
	logi.piszStart("--->", "Niszczenie obiektow swiata.");

	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		niszczObiektSwiat(*it);
	}

	logi.piszStop("<---", "Niszczenie obiektow swiata.");
}
void Swiat::ustawBlizszaPlaszcz(
	float const		odl
	) {
	odlBlizszaPlaszcz = odl;
}
void Swiat::ustawCelKamera(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat3(&celKamera, XMVectorSet(x, y, z, 0.0f));
}
void Swiat::ustawDalszaPlaszcz(
	float const		odl
	) {
	odlDalszaPlaszcz = odl;
}
void Swiat::ustawGoraKamera(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat3(&goraKamera, XMVectorSet(x, y, z, 0.0f));
}
void Swiat::ustawKatProjekcja(
	float const		kat
	) {
	katProjekcja = kat;
}
void Swiat::ustawPozycjaKamera(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat3(&pozKamera, XMVectorSet(x, y, z, 0.0f));
}
void Swiat::usunProjekcjaZ1(
	XMVECTOR* const		pkt3W,
	float const			x,
	float const			y
	) const {
	// współrzędne 3W przy założeniu, że z = 1 (w ten sposób usuwamy projekcję)
	*pkt3W = XMVectorSetX(*pkt3W, x / macProjekcja._11);
	*pkt3W = XMVectorSetY(*pkt3W, y / macProjekcja._22);
	*pkt3W = XMVectorSetZ(*pkt3W, 1.0f);
}
void Swiat::usunWidokPkt(
	XMVECTOR* const		pkt3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	*pkt3W = XMVector3TransformCoord(*pkt3W, macOdwrot);
}
void Swiat::usunWidokWektor(
	XMVECTOR* const		wekt3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	*wekt3W = XMVector3TransformNormal(*wekt3W, macOdwrot);
}
void Swiat::wezBlizszaPlaszczyzna(
	float* const		odl
	) const {
	*odl = odlBlizszaPlaszcz;
}
void Swiat::wezPozKamera(
	XMVECTOR* const		poz
	) const {
	*poz = XMLoadFloat3(&pozKamera);
}
void Swiat::wykonajKolizjeSiatka() const {
	SiatkaObiekty siatka, siatkaRob;
	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		(*it)->wezSiatka(&siatkaRob);
		siatka.dopiszSiatka(siatkaRob);
	}

	Kolizje_ kolizje;
	siatka.wezKolizje(&kolizje);

	Kolizje_::const_iterator itA;
	set<IObiekt3W const* const>::const_iterator itB;
	bool rob;
	for(itA = kolizje.begin(); itA != kolizje.end(); ++itA) {
	for(itB = itA->second.begin(); itB != itA->second.end(); ++itB) {
		itA->first->wykonajZdarzKolizjaSiatka(*itB);
	}
	}
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
	logi.piszStart("--->", "Niszczenie swiata.");

	niszczObiektySwiat();
	
	logi.piszStop("<---", "Niszczenie swiata.");
}
void Swiat::tworzKolejnaKlatka() {
	//wykonajKolizjeSiatka();

	ListaObiekty::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); ++it) {
		(*it)->aktualizujPoz();
		(*it)->rysuj();
	}
}
IObiekt3W* Swiat::tworzObiektKursor() {
	Wierzcholek wierzcholki[] = {
		Wierzcholek(+0.0f, -0.2f, +0.0f, +0.0f, +1.0f),
		Wierzcholek(+0.0f, +0.0f, +0.0f, +0.5f, +0.0f),
		Wierzcholek(+0.2f, -0.0f, +0.0f, +1.0f, +1.0f),
	};
	DWORD indeksy[] = {
		0, 1, 2
	};

	IObiekt3W* const ob = new Obiekt3W(
		wierzcholki,
		3,
		indeksy,
		3,
		"tekstura\\t1.jpg",
		&macProjekcja,
		&macWidok
	);
	ob->ustawFizyka();
	ob->ustawGrafika();
	obiektySwiat.insert(ob);
	return ob;
}
IObiekt3W* Swiat::tworzObiektRycerz() {
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

	IObiekt3W* const ob = new Obiekt3W(
		wierzcholki,
		6,
		indeksy,
		24,
		"tekstura\\t2.jpg",
		&macProjekcja,
		&macWidok
	);
	ob->ustawFizyka();
	ob->ustawGrafika();
	ob->wykonajZdarzRuch(XMVectorSet(-2.0f, +0.0f, +4.0f, 0.0f));
	obiektySwiat.insert(ob);
	return ob;
}
IObiekt3W* Swiat::tworzObiektSmok() {
	Wierzcholek wierzcholki[] = {
		Wierzcholek(+0.5f, -0.5f, +0.0f, +1.0f, +0.0f),
		Wierzcholek(-0.5f, -0.5f, +0.0f, +1.0f, +1.0f),
		Wierzcholek(-0.5f, +0.5f, +0.0f, +0.0f, +1.0f),
	};
	DWORD indeksy[] = {
		0, 1, 2
	};

	IObiekt3W* const ob = new Obiekt3W(
		wierzcholki,
		3,
		indeksy,
		3,
		"tekstura\\t1.jpg",
		&macProjekcja,
		&macWidok
	);
	ob->ustawFizyka();
	ob->ustawGrafika();
	ob->wykonajZdarzRuch(XMVectorSet(+2.0f, +0.0f, +4.0f, 0.0f));
	obiektySwiat.insert(ob);
	return ob;
}

#endif