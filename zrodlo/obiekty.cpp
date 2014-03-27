#pragma once

#include "obiekty.h"

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

void SiatkaObiekty::dopiszObiekt(
	float				x,
	float				y,
	float				z,
	IObiekt* const		ob
	) {
	siatka.insert(
		pair<float const, Siatka2Obiekty_>(x, Siatka2Obiekty_())
	);
	siatka.at(x).insert(
		pair<float const, Siatka1Obiekty_>(y, Siatka1Obiekty_())
	);
	siatka.at(x).at(y).insert(
		pair<float const, ObiektyObszar_>(z, ObiektyObszar_())
	);
	siatka.at(x).at(y).at(z).insert(ob);
}
void SiatkaObiekty::dopiszSiatka(
	SiatkaObiekty const		siat
	) {
	SiatkaObiekty::StalyIteratorX itX;
	SiatkaObiekty::StalyIteratorY itY;
	SiatkaObiekty::StalyIteratorZ itZ;
	SiatkaObiekty::StalyIteratorOb itOb;
	for(itX = siat.siatka.begin(); itX != siat.siatka.end(); ++itX) {
	for(itY = itX->second.begin(); itY != itX->second.end(); ++itY) {
	for(itZ = itY->second.begin(); itZ != itY->second.end(); ++itZ) {
	for(itOb = itZ->second.begin(); itOb != itZ->second.end(); ++itOb){
		dopiszObiekt(itX->first, itY->first, itZ->first, *itOb);
	}
	}
	}
	}
}
void SiatkaObiekty::czysc() {
	siatka.clear();
}
void SiatkaObiekty::ustawWspolnyObiekt(
	IObiekt* const		ob
	) {
	SiatkaObiekty::IteratorX itX;
	SiatkaObiekty::IteratorY itY;
	SiatkaObiekty::IteratorZ itZ;
	for(itX = siatka.begin(); itX != siatka.end(); ++itX) {
	for(itY = itX->second.begin(); itY != itX->second.end(); ++itY) {
	for(itZ = itY->second.begin(); itZ != itY->second.end(); ++itZ) {
		itZ->second.clear();
		itZ->second.insert(ob);
	}
	}
	}
}
void SiatkaObiekty::wezKolizje(
	Kolizje_* const		kolizje
	) const {
	Siatka3Obiekty_::const_iterator it3;
	Siatka2Obiekty_::const_iterator it2;
	Siatka1Obiekty_::const_iterator it1;
	ObiektyObszar_::const_iterator itA;
	ObiektyObszar_::const_iterator itB;
	for(it3 = siatka.begin(); it3 != siatka.end(); ++it3) {
	for(it2 = it3->second.begin(); it2 != it3->second.end(); ++it2) {
	for(it1 = it2->second.begin(); it1 != it2->second.end(); ++it1) {
	for(itA = it1->second.begin(); itA != it1->second.end(); ++itA) {
	for(itB = it1->second.begin(); itB != it1->second.end(); ++itB) {
		if(itA == itB) {
			continue;
		}

		kolizje->insert(pair<IObiekt* const, set<IObiekt const* const>>(
			*itA, set<IObiekt const* const>()
		));
		kolizje->at(*itA).insert(*itB);
	}
	}
	}
	}
	}
}
bool SiatkaObiekty::wezObiekty(
	set<IObiekt* const>* const		obiekty,
	float							x,
	float							y,
	float							z
	) const {
	obiekty->clear();

	if(siatka.count(x) == 0) {
		return false;
	}
	if(siatka.at(x).count(y) == 0) {
		return false;
	}
	if(siatka.at(x).at(y).count(z) == 0) {
		return false;
	}
	*obiekty = siatka.at(x).at(y).at(z);
	return true;
}
//SiatkaObiekty::StalyIteratorX SiatkaObiekty::wezPocz() const {
//	return siatka.begin();
//}
//SiatkaObiekty::StalyIteratorX SiatkaObiekty::wezKon() const {
//	return siatka.end();
//}

IObiekt::IObiekt() : fiz(NULL), graf(NULL), v(XMFLOAT3(0,0,0))
	{
	XMStoreFloat4x4(&macPrzesun, XMMatrixIdentity());
}
IObiekt::~IObiekt() {}
void IObiekt::aktualizujPoz() {
	fiz->aktualizujPoz();
}
void IObiekt::aktualizujSiatka() {
	fiz->aktualizujSiatka();
}
void IObiekt::rysuj() const {
	graf->rysuj();
}
XMVECTOR IObiekt::wezPoz() const {
	XMVECTOR poz;
	XMMatrixDecompose(
		&XMVectorSet(0,0,0,0), &XMVectorSet(0,0,0,0), &poz, XMLoadFloat4x4(&macPrzesun)
	);
	return poz;
}
void IObiekt::wezSiatka(
	SiatkaObiekty* const		siat
	) {
	siat->czysc();
	*siat = siatka;
	siat->ustawWspolnyObiekt(this);
}
XMMATRIX IObiekt::wezSwiat() const {
	return XMLoadFloat4x4(&macPrzesun);
}
void IObiekt::wykonajKolizjaSiatka(
	IObiekt const* const		ob
	) {
	fiz->wykonajKolizjaSiatka(&WektObiekty3W_(), ob);
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
void Obiekt3W::tworzBufIndeksy() {
	usunBufIndeksy();
	tworzBufor<DWORD>(
		D3D11_BIND_INDEX_BUFFER,
		ind.size(),
		bufIndeksy
	);
}
void Obiekt3W::tworzBufWierz() {
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
void Obiekt3W::usunBufIndeksy() {
	if(bufIndeksy != NULL) {
		bufIndeksy->Release();
		bufIndeksy = NULL;
		logi.pisz("OK", "Zwolnij bufor indeksow.");
	}
}
void Obiekt3W::usunBufWierz() {
	if(bufWierz != NULL) {
		bufWierz->Release();
		bufWierz = NULL;
		logi.pisz("OK", "Zwolnij bufor wierzcholkow.");
	}
}
void Obiekt3W::usunWidokTekstura() {
	if(widokTekstura != NULL) {
		widokTekstura->Release();
		widokTekstura = NULL;
		logi.pisz("OK", "Zwolnij widok tekstury.");
	}
}
void Obiekt3W::wiazIndeksy() const {
	if(bufIndeksy == NULL) {
		logi.pisz("UWAGA", "Nie powiazano indeksow. Bufor indeksow jest pusty.");
	} else {
		zasoby.render->IASetIndexBuffer(bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
	}
}
void Obiekt3W::wiazTeksture() const {
	if(widokTekstura == NULL) {
		logi.pisz("?", "Nie powiazano tekstury. Widok tekstury jest pusty.");
	} else {
		zasoby.render->PSSetShaderResources(0, 1, &widokTekstura);
	}
}
void Obiekt3W::wiazWierz() const {
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
void Obiekt3W::wypelnijBufIndeksy() {
	zasoby.render->UpdateSubresource(bufIndeksy, 0, NULL, &ind[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor indeksow.");
}
void Obiekt3W::wypelnijBufWierz() {
	zasoby.render->UpdateSubresource(bufWierz, 0, NULL, &wierz[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor wierzcholkow.");
}
Obiekt3W::Obiekt3W(
	Wierzcholek const *const		wierzcholki,
	UINT const						ilWierzcholki,
	DWORD const *const				indeksy,
	UINT const						ilIndeksy,
	string							sciezkaTekstura,
	XMFLOAT4X4 const* const			mProjekcja,
	XMFLOAT4X4 const* const			mWidok
	) : bufIndeksy(NULL), bufWierz(NULL), macProjekcja(mProjekcja), macWidok(mWidok), widokTekstura(NULL)
	{
	nadpiszWierzcholki(wierzcholki, ilWierzcholki);
	nadpiszIndeksy(indeksy, ilIndeksy);
	tworzWidokTekstura(sciezkaTekstura);
	tworzBufWierz();
	tworzBufIndeksy();
	wypelnijBufWierz();
	wypelnijBufIndeksy();
	XMStoreFloat4x4(&macPrzesun, XMMatrixIdentity());
}
Obiekt3W::~Obiekt3W() {
	logi.piszStart("--->", "Niszcz obiekt 3W.");
	usunBufIndeksy();
	usunBufWierz();
	usunWidokTekstura();
	logi.piszStop("<---", "Niszcz obiekt 3W.");
}
void Obiekt3W::ustawFizyka() {
	FizykaPostac* ob = new FizykaPostac(this);
	IObiekt::fiz = ob;
	fiz = ob;
}
void Obiekt3W::ustawGrafika() {
	graf = new Grafika3WPodstawa(this);
}
bool Obiekt3W::wezKolizjePromien(
	set<float>* const		odlKolizje,
	XMVECTOR				pocz,
	XMVECTOR				kier
	) const {
	fiz->usunSwiatPkt(&pocz);
	fiz->usunSwiatWektor(&kier);
	return fiz->wezKolizjePromien(odlKolizje, pocz, kier);
}
void Obiekt3W::wykonajRuch(
	XMVECTOR const		w
	) {
	fiz->dodajPredkosc(w);
}

ObiektZbior::ObiektZbior(
	ListaObiekty::const_iterator const		itPierwszy,
	ListaObiekty::const_iterator const		itOstatni
	) {
	logi.piszStart("--->", "Tworz Obiekty.");

	podobiekty.insert(itPierwszy, itOstatni);

	logi.piszStart("<---", "Tworz Obiekty.");
}
ObiektZbior::~ObiektZbior() {}
void ObiektZbior::ustawFizyka() {
	//fiz = new FizykaObiektZbior(this);
}

