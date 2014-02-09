﻿#ifndef _GLOBALNE_
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

#ifndef _GRAF_
#define _GRAF_
#include "graf.cpp"
#endif

class Fizyka {
	Grafika*			grafika;
	MapaObiekty3W_		obiektyScena;
	MapaObiekty3W_		obiektySwiat;
	void				przesunHistoriaObiekty(MapaObiekty3W_* const);
	bool				wezKolizjaPromienSwiat(
							map<float const, UINT const>* const,
							XMVECTOR const,
							XMVECTOR const
						) const;
public:
						Fizyka();
						~Fizyka();
	void				dodajObiektScena(Obiekt3W* const);
	void				dodajObiektSwiat(Obiekt3W* const);
	void				laczGrafika(Grafika* const);
	void				niszczObiektSwiat(UINT const);
	void				niszczObiektySwiat();
	void				przesunObiekt(UINT const, XMVECTOR const);
	void				tworzKolejnaKlatka();
	void				usunObiektScena(UINT const);
	void				wezPozOb(XMVECTOR* const, UINT const) const;
	bool				wezObPromienSwiat(UINT* const, UINT const) const;
};
void Fizyka::przesunHistoriaObiekty(
	MapaObiekty3W_* const		m // mapa obiektów 3W
	) {
	MapaObiekty3W_::const_iterator it;
	for(it = m->begin(); it != m->end(); it++) {
		it->second->przesunHistoria();
	}
}
bool Fizyka::wezKolizjaPromienSwiat(
	map<float const, UINT const>* const		obiektyKol,
	XMVECTOR const		pocz,
	XMVECTOR const		kier
	) const {
	obiektyKol->clear();
	// weź obiekty kolidujące z promieniem, ułożone od najbliższych początku promienia do najdalszych początku promienia
	map<float const, UINT const> trojkatyKol;
	XMVECTOR p, k;
	MapaObiekty3W_::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); it++) {
		p = pocz;
		k = kier;
		it->second->usunSwiatPkt(&p);
		it->second->usunSwiatWektor(&k);
		if(it->second->wezKolizjaPromienModel(&trojkatyKol, p, k)) {
			obiektyKol->insert(pair<float const, UINT const>(
				trojkatyKol.begin()->first, it->first
			));
		}
	}

	// jeśli brak kolizji promienia z obiektami
	if(obiektyKol->size() == 0) {
		return false;
	} else {
		return true;
	}
}
Fizyka::Fizyka() : grafika(NULL)
	{}
Fizyka::~Fizyka() {
	niszczObiektySwiat();
}
void Fizyka::dodajObiektScena(
	Obiekt3W* const		ob
	) {
	if(obiektySwiat.count(UINT(ob)) == 0) {
		logi.pisz("UWAGA", "Dodawanie obiektu do listy obiektow sceny: Obiektu nie ma na liscie obiektow swiata!");
		return;
	}

	pair<MapaObiekty3W_::iterator, bool> p;
	p = obiektyScena.insert(pair<UINT const, Obiekt3W* const>(UINT(ob), ob));
	if(p.second == false) {
		logi.pisz("UWAGA", "Dodawanie obiektu do listy obiektow sceny: Obiekt juz jest na liscie!");
	}
}
void Fizyka::dodajObiektSwiat(
	Obiekt3W* const		ob
	) {
	pair<MapaObiekty3W_::iterator, bool> p;
	p = obiektySwiat.insert(pair<UINT const, Obiekt3W* const>(UINT(ob), ob));
	if(p.second == false) {
		logi.pisz("UWAGA", "Dodawanie obiektu do listy obiektow swiata: Obiekt juz jest na liscie!");
	}
}
void Fizyka::laczGrafika(
	Grafika* const		g
	) {
	grafika = g;
}
void Fizyka::niszczObiektSwiat(
	UINT const		adresOb
	) {
	if(obiektySwiat.count(adresOb) == 0) {
		logi.pisz("UWAGA", "Niszczenie obiektu swiata: Nie ma takiego obiektu na liscie!");
	} else {
		delete obiektySwiat[adresOb];
		obiektySwiat.erase(adresOb);
	}
}
void Fizyka::niszczObiektySwiat() {
	logi.piszStart("START", "Niszczenie obiektow swiata.");
	MapaObiekty3W_::const_iterator it;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); it++) {
		niszczObiektSwiat(it->first);
	}
	logi.piszStop("STOP", "Niszczenie obiektow swiata.");
}
void Fizyka::przesunObiekt(
	UINT const			adresOb,
	XMVECTOR const		w // wektor przesunięcia
	) {
	obiektySwiat[adresOb]->przesun(w);
}
void Fizyka::tworzKolejnaKlatka() {
	przesunHistoriaObiekty(&obiektySwiat);
	grafika->rysujKolejnaKlatka(&obiektySwiat);
}
void Fizyka::usunObiektScena(
	UINT const		adresOb
	) {
	if(obiektyScena.erase(adresOb) == 0) {
		logi.pisz("UWAGA", "Usuwanie obiektu sceny: Nie ma takiego obiektu na liscie!");
	}
}
bool Fizyka::wezObPromienSwiat(
	UINT* const		adrOb,
	UINT const		adrKursor
	) const {
	// początek wektora wyboru
	XMVECTOR pocz;
	grafika->wezPozKamera(&pocz);

	// kierunek wektora wyboru
	XMVECTOR kier;
	wezPozOb(&kier, adrKursor);
	kier = kier - pocz;
	
	// obiekty kolidujące z promieniem wyboru, ułożone od najbliższych początku promienia wyboru do najdalszych początku promienia wyboru
	map<float const, UINT const> obiektyKol;
	wezKolizjaPromienSwiat(&obiektyKol, pocz, kier);
	
	// jeśli promień nie koliduje z obiektami
	if(obiektyKol.size() == 0) {
		*adrOb = NULL;
		return false;
	}

	// usuń obiekt kursora z listy kolidujących
	if(obiektyKol.begin()->second == adrKursor) {
		obiektyKol.erase(obiektyKol.begin());
	}

	// jeśli nie koliduje z żadnym innym oprócz kursora
	if(obiektyKol.size() == 0) {
		*adrOb = NULL;
		return false;
	}

	// zwróć obiekt najblizej początku promienia wyboru
	*adrOb = obiektyKol.begin()->second;
	return true;
}
void Fizyka::wezPozOb(
	XMVECTOR* const		poz,
	UINT const			adresOb
	) const {
	XMMATRIX m1;
	obiektySwiat.at(adresOb)->wezPrzesunAkt(&m1);
	XMFLOAT4X4 m2;
	XMStoreFloat4x4(&m2, m1);
	*poz = XMVectorSet(m2._41, m2._42, m2._43, +0.0f);
}



