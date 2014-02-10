#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFZASOBY_
#define _GRAFZASOBY_
#include "grafZasoby.cpp"
#endif

class Ekran {
public:
	void				usunEkranWsp(float* const, float* const) const;
};
void Ekran::usunEkranWsp(
	float* const		x,
	float* const		y
	) const {
	*x = 2.0f * *x / float(szerRend) - 1.0f;
	*y = -(2.0f * *y / float(wysRend) - 1.0f);
}

class Projektor {
	float			katProjekcja;
	float			odlBlizszaPlaszcz;
	float			odlDalszaPlaszcz;
	XMFLOAT4X4		macProjekcja;
public:
	void			ustawKat(float const);
	void			ustawBlizszaPlaszcz(float const);
	void			ustawDalszaPlaszcz(float const);
	void			tworzProjekcja();
	XMMATRIX		wezProjekcja() const;
	void			usunProjekcjaZ1(XMVECTOR* const, float const, float const) const;
};
void Projektor::ustawKat(
	float const		kat
	) {
	katProjekcja = kat;
}
void Projektor::ustawBlizszaPlaszcz(
	float const		odl
	) {
	odlBlizszaPlaszcz = odl;
}
void Projektor::ustawDalszaPlaszcz(
	float const		odl
	) {
	odlDalszaPlaszcz = odl;
}
void Projektor::tworzProjekcja() {
	XMMATRIX mac = XMMatrixPerspectiveFovLH(
		katProjekcja*3.14f/180,
		float(szerRend)/wysRend,
		odlBlizszaPlaszcz,
		odlDalszaPlaszcz);
	XMStoreFloat4x4(&macProjekcja, mac);
}
XMMATRIX Projektor::wezProjekcja() const {
	return XMLoadFloat4x4(&macProjekcja);
}
void Projektor::usunProjekcjaZ1(
	XMVECTOR* const		pkt3W,
	float const			x,
	float const			y
	) const {
	// współrzędne 3W przy założeniu, że z = 1 (w ten sposób usuwamy projekcję)
	*pkt3W = XMVectorSetX(*pkt3W, x / macProjekcja._11);
	*pkt3W = XMVectorSetY(*pkt3W, y / macProjekcja._22);
	*pkt3W = XMVectorSetZ(*pkt3W, 1.0f);
}

class Kamera {
	XMFLOAT4		pozKamery;
	XMFLOAT4		celKamery;
	XMFLOAT4		goraKamery;
	XMFLOAT4X4		macWidok;
public:
	void			ustawPozycja(
							float const, float const, float const
					);
	void			ustawCel(
							float const, float const, float const
					);
	void			ustawGora(
							float const, float const, float const
					);
	void			tworzWidok();
	void			wezPoz(XMVECTOR* const) const;
	XMMATRIX		wezWidok() const;
	void			usunWidokPkt(XMVECTOR* const) const;
	void			usunWidokWektor(XMVECTOR* const) const;
};
void Kamera::ustawPozycja(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat4(&pozKamery, XMVectorSet(x, y, z, 0.0f));
}
void Kamera::ustawCel(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat4(&celKamery, XMVectorSet(x, y, z, 0.0f));
}
void Kamera::ustawGora(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat4(&goraKamery, XMVectorSet(x, y, z, 0.0f));
}
void Kamera::tworzWidok() {
	XMVECTOR w1 = XMLoadFloat4(&pozKamery);
	XMVECTOR w2 = XMLoadFloat4(&celKamery);
	XMVECTOR w3 = XMLoadFloat4(&goraKamery);

	XMStoreFloat4x4(&macWidok, XMMatrixLookAtLH(w1, w2, w3));
	logi.pisz("OK", "Ustaw widok kamery sceny.");
}
void Kamera::wezPoz(
	XMVECTOR* const		poz
	) const {
	*poz = XMLoadFloat4(&pozKamery);
}
XMMATRIX Kamera::wezWidok() const {
	return XMLoadFloat4x4(&macWidok);
}
void Kamera::usunWidokPkt(
	XMVECTOR* const		pkt3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	*pkt3W = XMVector3TransformCoord(*pkt3W, macOdwrot);
}
void Kamera::usunWidokWektor(
	XMVECTOR* const		wekt3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	*wekt3W = XMVector3TransformNormal(*wekt3W, macOdwrot);
}

class Grafika {
	Ekran			ekran;
	Kamera			kamera;
	Projektor		projektor;
	void			aktualCoObiekt(Obiekt3W const* const) const;
	void			rysujObiekt(Obiekt3W const* const) const;
public:
					Grafika();
	void			rysujKolejnaKlatka(MapaObiekty3W_ const* const);
	void			wezPozKamera(XMVECTOR* const) const;
	void			wezWektorSwiat(
						XMVECTOR* const, XMVECTOR* const, float, float
					) const;
};
void Grafika::aktualCoObiekt(
	Obiekt3W const* const		ob
	) const {
	XMMATRIX p;
	ob->wezPrzesunAkt(&p);
	// macierz SWP musi być transponowana przez wysłaniem do szadera
	zasoby.daneCoObiekt.macSWP = XMMatrixTranspose(
		p * kamera.wezWidok() * projektor.wezProjekcja()
	);
}
void Grafika::rysujObiekt(
	Obiekt3W const* const		ob
	) const {
	ob->wiazWierz();
	ob->wiazIndeksy();
	ob->wiazTeksture();
	aktualCoObiekt(ob);
	zasoby.wypelnijCoObiekt();
	zasoby.wiazCoObiekt();
	zasoby.render->DrawIndexed(ob->wezIloscIndeksy(), 0, 0);
}
Grafika::Grafika() {
	logi.piszStart("--->", "Stworz grafika.");
	kamera.ustawPozycja(+0.0f, +0.0f, -0.5f);
	kamera.ustawCel(+0.0f, 0.0f, 0.0f);
	kamera.ustawGora(+0.0f, +1.0f, +0.0f);
	kamera.tworzWidok();
	projektor.ustawKat(90);
	projektor.ustawBlizszaPlaszcz(0.5f);
	projektor.ustawDalszaPlaszcz(100.0f);
	projektor.tworzProjekcja();
	logi.piszStop("<---", "Stworz grafika.");
}
void Grafika::rysujKolejnaKlatka(
	MapaObiekty3W_ const* const		obiektyScena
	) {
	MapaObiekty3W_::const_iterator it;
	for(it = obiektyScena->begin(); it != obiektyScena->end(); it++) {
		rysujObiekt(it->second);
	}
	zasoby.lancWym->Present(0, 0);
}
void Grafika::wezPozKamera(
	XMVECTOR* const		poz
	) const {
	kamera.wezPoz(poz);
}
void Grafika::wezWektorSwiat(
	XMVECTOR* const		poz,
	XMVECTOR* const		kier,
	float				x,
	float				y
	) const {
	ekran.usunEkranWsp(&x, &y);
	projektor.usunProjekcjaZ1(kier, x, y);
	kamera.usunWidokWektor(kier);
	*poz = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	kamera.usunWidokPkt(poz);
}


