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

#ifndef _GRAF_
#define _GRAF_
#include "graf.cpp"
#endif

class Fizyka {
	Grafika*			grafika;
	MapaObiekty3W_		obiektyScena;
	MapaObiekty3W_		obiektySwiat;
	void				przesunHistoriaObiekty(MapaObiekty3W_* const);
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
	void				wez(UINT* const, UINT const) const;
};
void Fizyka::przesunHistoriaObiekty(
	MapaObiekty3W_* const		m // mapa obiektów 3W
	) {
	MapaObiekty3W_::const_iterator it;
	for(it = m->begin(); it != m->end(); it++) {
		it->second->przesunHistoria();
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

	std::pair<MapaObiekty3W_::iterator, bool> p;
	p = obiektyScena.insert(std::pair<UINT const, Obiekt3W* const>(UINT(ob), ob));
	if(p.second == false) {
		logi.pisz("UWAGA", "Dodawanie obiektu do listy obiektow sceny: Obiekt juz jest na liscie!");
	}
}
void Fizyka::dodajObiektSwiat(
	Obiekt3W* const		ob
	) {
	std::pair<MapaObiekty3W_::iterator, bool> p;
	p = obiektySwiat.insert(std::pair<UINT const, Obiekt3W* const>(UINT(ob), ob));
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
void Fizyka::wezPozOb(
	XMVECTOR* const		poz,
	UINT const			adresOb
	) const {
	XMMATRIX m;
	obiektySwiat.at(adresOb)->wezPrzesunAkt(&m);
	*poz = XMVectorSet(m._41, m._42, m._43, +0.0f);
}
void Fizyka::wez(
	UINT* const		adrOb,
	UINT const		adrKursor
	) const {
	float x, y, z;
	// początek wektora wyboru
	XMVECTOR pocz;
	grafika->wezPozKamera(&pocz);
	// kierunek wektora wyboru
	XMVECTOR kier;
	wezPozOb(&kier, adrKursor);
	kier = kier - pocz;
	// szukaj kolizji
	MapaObiekty3W_::const_iterator it;
	XMVECTOR p, k;
	bool flg;
	*adrOb = adrKursor;
	for(it = obiektySwiat.begin(); it != obiektySwiat.end(); it++) {
		it++;
		p = pocz;
		k = kier;
		it->second->usunSwiatPkt(&p);
		it->second->usunSwiatWektor(&k);
		it->second->sprawdzKolizjaModel(&flg, p, k);
		if(flg == true) {
			*adrOb = it->first;
			return;
		}
	}
}

