#pragma once

#include "obiekty.h"
#include "fizyka.h"
#include "grafika.h"

Wierzcholek::Wierzcholek(
	float		x,
	float		y,
	float		z
	) : poz(x, y, z)
	{}
Wierzcholek::Wierzcholek(
	float		x, // kształt
	float		y, // kształt
	float		z, // kształt
	float		u, // tekstura
	float		v // tekstura
	) : poz(x, y, z), pozTekstury(u, v)
	{}

IObiekt::IObiekt(
	) : fiz(NULL), graf(NULL), v(XMFLOAT3(0,0,0))
	{}
IObiekt::~IObiekt(
	) {}
void IObiekt::aktualizujSiatka(
	) {
	fiz->aktualizujSiatka();
}
void IObiekt::rysuj(
	) const {
	graf->rysuj();
}
IFizyka* IObiekt::wezFiz(
	) {
	return fiz;
}
void IObiekt::wezSiatka(
	SiatkaObiekty* const		siat
	) {
	siat->czysc();
	*siat = siatka;
	siat->ustawWspolnyObiekt(this);
}

void Obiekt3W::nadpiszIndeksy(
	DWORD const *const		indeksy,
	UINT const				ilIndeksy
	) {
	ind.assign(indeksy, indeksy+ilIndeksy);
	logi.pisz("OK", "Wgraj indeksy.");
}
void Obiekt3W::nadpiszWierzcholki(
	Wierzcholek const *const		wierzcholki,
	UINT const						ilWierz
	) {
	wierz.assign(wierzcholki, wierzcholki+ilWierz);
	logi.pisz("OK", "Wgraj wierzcholki.");
}
void Obiekt3W::tworzBufIndeksy(
	) {
	usunBufIndeksy();
	tworzBufor<DWORD>(
		D3D11_BIND_INDEX_BUFFER,
		ind.size(),
		bufIndeksy
	);
}
void Obiekt3W::tworzBufWierz(
	) {
	usunBufWierz();
	tworzBufor<Wierzcholek>(
		D3D11_BIND_VERTEX_BUFFER,
		wierz.size(),
		bufWierz
	);
}
void Obiekt3W::tworzWidokTekstura(
	string sciezka
	) {
	usunWidokTekstura();
	wynik = D3DX11CreateShaderResourceViewFromFile(
		zasoby.karta,
		sciezka.c_str(),
		NULL,
		NULL,
		&widokTekstura,
		NULL
	);
	SprawdzWynik(wynik, "Wgranie tekstury.");
}
void Obiekt3W::usunBufIndeksy(
	) {
	if(bufIndeksy != NULL) {
		bufIndeksy->Release();
		bufIndeksy = NULL;
		logi.pisz("OK", "Zwolnij bufor indeksow.");
	}
}
void Obiekt3W::usunBufWierz(
	) {
	if(bufWierz != NULL) {
		bufWierz->Release();
		bufWierz = NULL;
		logi.pisz("OK", "Zwolnij bufor wierzcholkow.");
	}
}
void Obiekt3W::usunWidokTekstura(
	) {
	if(widokTekstura != NULL) {
		widokTekstura->Release();
		widokTekstura = NULL;
		logi.pisz("OK", "Zwolnij widok tekstury.");
	}
}
XMVECTOR Obiekt3W::wezPoz(
	) const {
	XMVECTOR poz;
	XMMatrixDecompose(
		&XMVectorSet(0,0,0,0), &XMVectorSet(0,0,0,0), &poz, XMLoadFloat4x4(&macPrzesun)
	);
	return poz;
}
XMMATRIX Obiekt3W::wezSwiat(
	) const {
	return XMLoadFloat4x4(&macPrzesun);
}
void Obiekt3W::wiazIndeksy(
	) const {
	if(bufIndeksy == NULL) {
		logi.pisz("UWAGA", "Nie powiazano indeksow. Bufor indeksow jest pusty.");
	} else {
		zasoby.render->IASetIndexBuffer(bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
	}
}
void Obiekt3W::wiazTeksture(
	) const {
	if(widokTekstura == NULL) {
		logi.pisz("?", "Nie powiazano tekstury. Widok tekstury jest pusty.");
	} else {
		zasoby.render->PSSetShaderResources(0, 1, &widokTekstura);
	}
}
void Obiekt3W::wiazWierz(
	) const {
	if(bufWierz == NULL) {
		logi.pisz("UWAGA", "Nie powiazano wierzcholkow. Bufor wierzcholkow jest pusty.");
	} else {
		UINT rozmWierz = sizeof(Wierzcholek);
		UINT przesunBufWierz = 0;
		zasoby.render->IASetVertexBuffers(
			0,
			1,
			&bufWierz,
			&rozmWierz,
			&przesunBufWierz);
	}
}
void Obiekt3W::wypelnijBufIndeksy(
	) {
	zasoby.render->UpdateSubresource(bufIndeksy, 0, NULL, &ind[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor indeksow.");
}
void Obiekt3W::wypelnijBufWierz(
	) {
	zasoby.render->UpdateSubresource(bufWierz, 0, NULL, &wierz[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor wierzcholkow.");
}
XMFLOAT4X4 Obiekt3W::macProjekcja;
XMFLOAT4X4 Obiekt3W::macWidok;
Obiekt3W::Obiekt3W(
	Wierzcholek const *const		wierzcholki,
	UINT const						ilWierzcholki,
	DWORD const *const				indeksy,
	UINT const						ilIndeksy,
	string							sciezkaTekstura
	) : bufIndeksy(NULL), bufWierz(NULL), widokTekstura(NULL)
	{
	nadpiszWierzcholki(wierzcholki, ilWierzcholki);
	nadpiszIndeksy(indeksy, ilIndeksy);
	tworzWidokTekstura(sciezkaTekstura);
	tworzBufWierz();
	tworzBufIndeksy();
	wypelnijBufWierz();
	wypelnijBufIndeksy();
	XMStoreFloat4x4(&macPrzesun, XMMatrixIdentity());
	ustawFizyka();
	ustawGrafika();
}
Obiekt3W::~Obiekt3W(
	) {
	logi.piszStart("--->", "Niszcz obiekt 3W.");
	usunBufIndeksy();
	usunBufWierz();
	usunWidokTekstura();
	logi.piszStop("<---", "Niszcz obiekt 3W.");
}
void Obiekt3W::ustawFizyka(
	) {
	FizykaPostac* ob = new FizykaPostac(this);
	IObiekt::fiz = ob;
	fiz = ob;
}
void Obiekt3W::ustawGrafika(
	) {
	graf = new Grafika3WPodstawa(this);
}
void Obiekt3W::wezKolizjePromien(
	MapaFloatObiekt_* const		odlKolizje,
	XMVECTOR const				pocz,
	XMVECTOR const				kier
	) const {
	XMVECTOR p = pocz;
	XMVECTOR k = kier;
	fiz->usunSwiatPkt(&p);
	fiz->usunSwiatWektor(&k);
	fiz->wezKolizjePromien(odlKolizje, p, k);
}

ObiektZbior::ObiektZbior(
	) {
	logi.piszStart("--->", "Tworz ObiektZbior.");
	ustawFizyka();
	ustawGrafika();
	logi.piszStart("<---", "Tworz ObiektZbior.");
}
ObiektZbior::~ObiektZbior() {}
void ObiektZbior::dodaj(
	IObiekt* const		ob
	) {
	podobiekty.insert(ob);
}
void ObiektZbior::ustawFizyka(
	) {
	FizykaTekst* ob = new FizykaTekst(this);
	IObiekt::fiz = ob;
	fiz = ob;
}
void ObiektZbior::ustawGrafika(
	) {
	graf = new GrafikaZbiorPodstawa(this);
}
void ObiektZbior::wezKolizjePromien(
	MapaFloatObiekt_* const		odlKolizje,
	XMVECTOR const				pocz,
	XMVECTOR const				kier
	) const {
	fiz->wezKolizjePromien(odlKolizje, pocz, kier);
}



