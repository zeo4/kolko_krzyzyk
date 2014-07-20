#pragma once

#include <stdint.h>
#include <obiekty.h>
#include <string>

#include "fizyka.h"
#include "grafika.h"

using std::string;

Obiekty3w::Obiekty3w() {
}
void Obiekty3w::niszcz_ob(uint32_t nr) {

}
void Obiekty3w::tworz_ob(XMFLOAT3* wierz, XMFLOAT2* tekstury, uint32_t il_wierz, DWORD* indeksy, uint32_t il_indeksy, string sciezka) {
	uint32_t il;

	// wgraj wierzchołki, współrzędne tekstur
	_mapa_wierz.wstaw_kon({_wierz.wez_il(), il_wierz});
	il = _wierz.wez_il_rezerw();
	_wierz.wstaw_zakres_kon(wierz, il_wierz);
	_tekstury.wstaw_zakres_kon(tekstury, il_wierz);
	if(il < _wierz.wez_il_rezerw()) {
		_buf_wierz->Release();
		_buf_tekstury->Release();
		tworz_bufor<XMFLOAT3>(D3D11_BIND_VERTEX_BUFFER, _wierz.wez_il_rezerw(), _buf_wierz);
		tworz_bufor<XMFLOAT2>(D3D11_BIND_VERTEX_BUFFER, _wsp_tekstury.wez_il_rezerw(), _buf_wsp_tekstury);
	}
	zasoby.render->UpdateSubresource(_buf_wierz, 0, 0, &(_wierz[0]), 0, 0);
	zasoby.render->UpdateSubresource(_buf_wsp_tekstury, 0, 0, &(_wsp_tekstury[0]), 0, 0);

	// wgraj indeksy
	_mapa_indeksy.wstaw_kon({_indeksy.wez_il(), il_indeksy});
	il = _indeksy.wez_il_rezerw();
	_indeksy.wstaw_zakres_kon(indeksy, il_indeksy);
	if(il < _indeksy.wez_il_rezerw()) {
		_buf_indeksy->Release();
		tworz_bufor<DWORD>(D3D11_BIND_INDEX_BUFFER, _indeksy.wez_il_rezerw(), _buf_indeksy);
	}
	zasoby.render->UpdateSubresource(_buf_indeksy, 0, 0, &(_indeksy[0]), 0, 0);

	// wgraj tekstury
	_wid_tekstury.wstaw_kon();
	D3DX11CreateShaderResourceViewFromFile(zasoby.karta, sciezka.c_str(), 0, 0, &(_wid_tekstury[_wid_tekstury.wez_il()-1]), 0);
}

IObiekt::IObiekt(XMFLOAT3 const przesPocz) : fiz(NULL), graf(NULL), przes(przesPocz), rodzic(NULL), tKolizja(1.0f)
	{
	XMStoreFloat4x4(&macSwiat, XMMatrixIdentity());
	XMStoreFloat4x4(&macSwiatBezkol, XMMatrixIdentity());
}
IObiekt::~IObiekt() {}
void IObiekt::rysuj() const {
	graf->rysuj();
}
IFizyka* IObiekt::wezFiz() const {
	return fiz;
}
IGrafika* IObiekt::wezGraf() const {
	return graf;
}

IObiekt* Obiekt3w::test = NULL;
void Obiekt3w::nadpiszIndeksy(DWORD const *const indeksy, UINT const ilIndeksy) {
	ind.assign(indeksy, indeksy+ilIndeksy);
	logi.pisz("OK", "Wgraj indeksy.");
}
void Obiekt3w::nadpiszWierzcholki(Wierzcholek const *const wierzcholki, UINT const ilWierz) {
	wierz.assign(wierzcholki, wierzcholki+ilWierz);
	logi.pisz("OK", "Wgraj wierzcholki.");
}
void Obiekt3w::tworzBufIndeksy() {
	usunBufIndeksy();
	tworzBufor<DWORD>(
		D3D11_BIND_INDEX_BUFFER,
		ind.size(),
		bufIndeksy
	);
}
void Obiekt3w::tworzBufWierz() {
	usunBufWierz();
	tworzBufor<Wierzcholek>(
		D3D11_BIND_VERTEX_BUFFER,
		wierz.size(),
		bufWierz
	);
}
void Obiekt3w::tworzWidokTekstura(string sciezka) {
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
void Obiekt3w::usunBufIndeksy() {
	if(bufIndeksy != NULL) {
		bufIndeksy->Release();
		bufIndeksy = NULL;
		logi.pisz("OK", "Zwolnij bufor indeksow.");
	}
}
void Obiekt3w::usunBufWierz() {
	if(bufWierz != NULL) {
		bufWierz->Release();
		bufWierz = NULL;
		logi.pisz("OK", "Zwolnij bufor wierzcholkow.");
	}
}
void Obiekt3w::usunWidokTekstura() {
	if(widokTekstura != NULL) {
		widokTekstura->Release();
		widokTekstura = NULL;
		logi.pisz("OK", "Zwolnij widok tekstury.");
	}
}
void Obiekt3w::wiazIndeksy() const {
	if(bufIndeksy == NULL) {
		logi.pisz("UWAGA", "Nie powiazano indeksow. Bufor indeksow jest pusty.");
	} else {
		zasoby.render->IASetIndexBuffer(bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
	}
}
void Obiekt3w::wiazTeksture() const {
	if(widokTekstura == NULL) {
		logi.pisz("?", "Nie powiazano tekstury. Widok tekstury jest pusty.");
	} else {
		zasoby.render->PSSetShaderResources(0, 1, &widokTekstura);
	}
}
void Obiekt3w::wiazWierz() const {
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
void Obiekt3w::wypelnijBufIndeksy() {
	zasoby.render->UpdateSubresource(bufIndeksy, 0, NULL, &ind[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor indeksow.");
}
void Obiekt3w::wypelnijBufWierz() {
	zasoby.render->UpdateSubresource(bufWierz, 0, NULL, &wierz[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor wierzcholkow.");
}
XMFLOAT4X4 Obiekt3w::macProjekcja;
XMFLOAT4X4 Obiekt3w::macWidok;
Obiekt3w::Obiekt3w(Wierzcholek const *const wierzcholki, UINT const ilWierzcholki, DWORD const *const indeksy, UINT const ilIndeksy, XMFLOAT3 const pozPocz, string sciezkaTekstura) : bufIndeksy(NULL), IObiekt(pozPocz), bufWierz(NULL), sasiedzi(NULL), widokTekstura(NULL)
	{
	nadpiszWierzcholki(wierzcholki, ilWierzcholki);
	nadpiszIndeksy(indeksy, ilIndeksy);
	tworzWidokTekstura(sciezkaTekstura);
	tworzBufWierz();
	tworzBufIndeksy();
	wypelnijBufWierz();
	wypelnijBufIndeksy();
	ustawFizykaPostac();
	ustawGrafika();
}
Obiekt3w::~Obiekt3w() {
	logi.pisz_start("--->", "Niszcz obiekt 3W.");
	usunBufIndeksy();
	usunBufWierz();
	usunWidokTekstura();
	logi.pisz_stop("<---", "Niszcz obiekt 3W.");
}
void Obiekt3w::ustawFizykaLitera() {
	FizykaLitera* ob = new FizykaLitera(this);
	IObiekt::fiz = ob;
	fiz = ob;
}
void Obiekt3w::ustawFizykaPostac() {
	FizykaPostac* ob = new FizykaPostac(this);
	IObiekt::fiz = ob;
	fiz = ob;
}
void Obiekt3w::ustawGrafika() {
	graf = new Grafika3w(this);
}
void Obiekt3w::ustawSasiedzi(MapaSasiedzi_ const* const s) {
	sasiedzi = s;
}
Fizyka3w* Obiekt3w::wezFiz() const {
	return fiz;
}
void Obiekt3w::wezKolizjePromien(MapaFloatObiekt_* const odlKolizje, XMVECTOR const pocz, XMVECTOR const kier) const {
	XMVECTOR p = pocz;
	XMVECTOR k = kier;
	fiz->usunSwiatPkt(&p);
	fiz->usunSwiatWektor(&k);
	fiz->wezKolizjePromien(odlKolizje, p, k);
}

ObiektZbior::ObiektZbior(XMFLOAT3 const pozPocz) : IObiekt(pozPocz)
	{
	logi.pisz_start("--->", "Tworz ObiektZbior.");
	ustawFizykaTekst();
	ustawGrafika();
	logi.pisz_stop("<---", "Tworz ObiektZbior.");
}
ObiektZbior::~ObiektZbior() {}
void ObiektZbior::dodaj(IObiekt* const ob) {
	podobiekty.insert(ob);
}
void ObiektZbior::ustawFizykaTekst() {
	FizykaTekst* ob = new FizykaTekst(this);
	IObiekt::fiz = ob;
	fiz = ob;
}
void ObiektZbior::ustawGrafika() {
	graf = new GrafikaZbior(this);
}
void ObiektZbior::ustawSasiedzi(MapaSasiedzi_ const* const s) {
	ListaObiekty::const_iterator it;
	for(it = podobiekty.begin(); it != podobiekty.end(); ++it) {
		(*it)->ustawSasiedzi(s);
	}
}
void ObiektZbior::wezKolizjePromien(MapaFloatObiekt_* const odlKolizje, XMVECTOR const pocz, XMVECTOR const kier) const {
	fiz->wezKolizjePromien(odlKolizje, pocz, kier);
}



