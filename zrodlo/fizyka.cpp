#pragma once

#include "fizyka.h"

Fizyka3W::~Fizyka3W() {}
void Fizyka3W::aktualizujSiatka() {
	// rozmiar pojedynczego obszaru
	float rozm = 0.5f;

	obiekt->siatka.czysc();

	XMVECTOR min; // lewy dolny bliski
	XMVECTOR max; // prawy górny daleki
	wezBrylaGraniczna(&min, &max);
	XMVECTOR v = XMLoadFloat3(&obiekt->v);
	min += v;
	max += v;

	// licz numery obszarów rogów bryły granicznej
	min = XMVectorFloor(min / rozm) * rozm;
	max = XMVectorFloor(max / rozm) * rozm;

	// zapisz numery zajmowanych przez obiekt obszarów siatki
	for(float i = XMVectorGetX(min); i <= XMVectorGetX(max); i += rozm) {
	for(float j = XMVectorGetY(min); j <= XMVectorGetY(max); j += rozm) {
	for(float k = XMVectorGetZ(min); k <= XMVectorGetZ(max); k += rozm) {
		obiekt->siatka.dopiszObiekt(i, j, k, obiekt);
	}
	}
	}
}
void Fizyka3W::dodajPredkosc(
	XMVECTOR const		w
	) {
	XMVECTOR v = XMLoadFloat3(&obiekt->v);
	v = v + w;
	XMStoreFloat3(&obiekt->v, v);
}
bool Fizyka3W::sprawdzKolizjaBryly(
	Obiekt3W const* const		ob
	) const {
	// obiekt - 1, ob - 2

	XMVECTOR pktMin1, pktMaks1, pktMin2, pktMaks2;
	if(wezBrylaGraniczna(&pktMin1, &pktMaks1) && ob->fiz->wezBrylaGraniczna(&pktMin2, &pktMaks2)) {
		XMVECTOR v1 = XMLoadFloat3(&obiekt->v);
		XMVECTOR v2 = XMLoadFloat3(&ob->v);
		return sprawdzKolizjaPudelkoPudelko(pktMin1 + v1, pktMaks1 + v1, pktMin2 + v2, pktMaks2 + v2);
	}
	return false;
}
bool Fizyka3W::sprawdzKolizjaModele(
	IObiekt const* const		ob
	) const {
	//vector<XMFLOAT3> wierzcholkiA, wierzcholkiB;
	//wezWierzcholkiSwiat(&wierzcholkiA);
	//ob->fiz->wezWierzcholkiSwiat(&wierzcholkiB);
	//for(int i = 0; i < obiekt3W->ind.size()-2; i += 3) {
	//for(int j = 0; j < ob->ind.size()-2; j += 3) {
	//	if(sprawdzKolizjaTrojkatTrojkat(
	//		XMLoadFloat3(&obiekt3W->wierz.at(obiekt3W->ind.at(i))),
	//		XMLoadFloat3(&obiekt3W->wierz.at(obiekt3W->ind.at(i+1))),
	//		XMLoadFloat3(&obiekt3W->wierz.at(obiekt3W->ind.at(i+2))),
	//		XMLoadFloat3(&ob->wierz.at(ob->ind.at(j))),
	//		XMLoadFloat3(&ob->wierz.at(ob->ind.at(j+1))),
	//		XMLoadFloat3(&ob->wierz.at(ob->ind.at(j+2)))
	//	)) {
	//		return true;
	//	}
	//}
	//}
	return false;
}
void Fizyka3W::usunSwiatPkt(
	XMVECTOR* const		pkt
	) const {
	XMMATRIX mOdwrot = XMMatrixInverse(&XMVectorSet(0,0,0,0), obiekt->wezSwiat());
	*pkt = XMVector3TransformCoord(*pkt, mOdwrot);
}
void Fizyka3W::usunSwiatWektor(
	XMVECTOR* const		w
	) const {
	XMMATRIX mOdwrot = XMMatrixInverse(&XMVectorSet(0,0,0,0), obiekt->wezSwiat());
	*w = XMVector3TransformNormal(*w, mOdwrot);
}
bool Fizyka3W::wezBrylaGraniczna(
	XMVECTOR* const				pktMin,
	XMVECTOR* const				pktMax
	) const {
	if(obiekt->wierz.size() == 0) {
		return false;
	}

	// wartość początkowa pktMin i pktMax
	*pktMin = XMLoadFloat3(&obiekt->wierz.at(0).poz);
	*pktMax = XMLoadFloat3(&obiekt->wierz.at(0).poz);
	XMMATRIX m = XMLoadFloat4x4(&obiekt->macPrzesun);
	*pktMin = XMVector3TransformCoord(*pktMin, m);
	*pktMax = XMVector3TransformCoord(*pktMax, m);

	// sprawdź pozostałe wierzchołki
	XMVECTOR pkt;
	for(UINT i = 1; i < obiekt->wierz.size(); ++i) {
		pkt = XMLoadFloat3(&obiekt->wierz.at(i).poz);
		pkt = XMVector3TransformCoord(pkt, m);
		if(XMVectorGetX(pkt) < XMVectorGetX(*pktMin)) {
			*pktMin = XMVectorSetX(*pktMin, XMVectorGetX(pkt));
		}
		if(XMVectorGetY(pkt) < XMVectorGetY(*pktMin)) {
			*pktMin = XMVectorSetY(*pktMin, XMVectorGetY(pkt));
		}
		if(XMVectorGetZ(pkt) < XMVectorGetZ(*pktMin)) {
			*pktMin = XMVectorSetZ(*pktMin, XMVectorGetZ(pkt));
		}
		if(XMVectorGetX(pkt) > XMVectorGetX(*pktMax)) {
			*pktMax = XMVectorSetX(*pktMax, XMVectorGetX(pkt));
		}
		if(XMVectorGetY(pkt) > XMVectorGetY(*pktMax)) {
			*pktMax = XMVectorSetY(*pktMax, XMVectorGetY(pkt));
		}
		if(XMVectorGetZ(pkt) > XMVectorGetZ(*pktMax)) {
			*pktMax = XMVectorSetZ(*pktMax, XMVectorGetZ(pkt));
		}
	}
	return true;
}
void Fizyka3W::wezKolidujaceZ3W(
	WektObiekty3W_* const		wekt,
	Obiekt3W const* const		ob
	) const {
	wekt->clear();
	if(sprawdzKolizjaBryly(ob)) {
		wekt->push_back(obiekt);
	}
}
bool Fizyka3W::wezKolizjePromien(
	set<float>* const		odlKolizje,
	XMVECTOR const			pocz,
	XMVECTOR const			kier
	) const {
	odlKolizje->clear();

	// weź odległości (od początku promienia) punktów kolizji, ułożone według odległości od najmniejszej do największej (istnieją odległości ujemne)
	float odl;
	for(int i = 0; i < obiekt->ind.size()-2; ++i) {
		if(wezKolizjaPromienTrojkat(
			&odl, pocz, kier,
			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i)).poz),
			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i+1)).poz),
			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i+2)).poz)
		)) {
			// zapisz odległość punktu kolizji
			odlKolizje->insert(odl);
		}
	}

	// jeśli brak kolizji promienia z obiektem
	if(odlKolizje->size() == 0) {
		return false;
	} else {
		return true;
	}
}
void Fizyka3W::wezWierzcholkiSwiat(
	vector<XMFLOAT3>* const		w
	) const 
{
	w->clear();

	XMMATRIX m = obiekt->wezSwiat();
	XMFLOAT3 pkt;
	for(int i = 0; i < obiekt->wierz.size(); ++i) {
		XMStoreFloat3(
			&pkt, XMVector3TransformCoord(XMLoadFloat3(&obiekt->wierz[i].poz), m)
		);
		w->push_back(pkt);
	}
}

void Fizyka3WKolizyjny::wykonajKolizjaBryly(
	Obiekt3W const* const		ob
	) {
	obiekt->v = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
void Fizyka3WKolizyjny::wykonajKolizjaSiatka(
	WektObiekty3W_* const		obiektyKol,
	IObiekt const* const		ob
	) {
	WektObiekty3W_ kolidujace;
	ob->fiz->wezKolidujaceZ3W(&kolidujace, obiekt);
	
	for(int i = 0; i < kolidujace.size(); ++i) {
		wykonajKolizjaBryly(kolidujace[i]);
	}
	obiektyKol->insert(obiektyKol->end(), kolidujace.begin(), kolidujace.end());
}

void Fizyka3WNiekolizyjny::wykonajKolizjaSiatka(
	WektObiekty3W_* const		obiektyKol,
	IObiekt const* const		ob
	) {
	WektObiekty3W_ kolidujace;
	ob->fiz->wezKolidujaceZ3W(&kolidujace, obiekt);
	obiektyKol->insert(obiektyKol->end(), kolidujace.begin(), kolidujace.end());
}

void Fizyka3WNieopozniony::aktualizujPoz() {
	// prędkość
	XMStoreFloat4x4(
		&obiekt->macPrzesun,
		XMMatrixTranslationFromVector(XMLoadFloat3(&obiekt->v)) * XMLoadFloat4x4(&obiekt->macPrzesun)
	);
	obiekt->v = XMFLOAT3(0,0,0);
}
// ---------------------------------------------
FizykaLitera::FizykaLitera(
	Obiekt3W* const		ob
	) : IFizyka3W(ob)
	{}

FizykaPostac::FizykaPostac(
	Obiekt3W* const		ob
	) : IFizyka3W(ob)
	{}
// =============================================
FizykaZbior::~FizykaZbior() {}
void FizykaZbior::aktualizujSiatka() {
	obiekt->siatka.czysc();

	ListaObiekty::const_iterator it;
	SiatkaObiekty siatkaRob;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->fiz->aktualizujSiatka();
		siatkaRob = (*it)->siatka;
		siatkaRob.ustawWspolnyObiekt(*it);
		obiekt->siatka.dopiszSiatka(siatkaRob);
	}
}
void FizykaZbior::dodajPredkosc(
	XMVECTOR const		w
	) {
	XMVECTOR v = XMLoadFloat3(&obiekt->v);
	v = v + w;
	XMStoreFloat3(&obiekt->v, v);
}
bool FizykaZbior::sprawdzKolizjaBryly(IObiekt const* const) const {return false;}
bool FizykaZbior::sprawdzKolizjaModele(IObiekt const* const) const {return false;}
void FizykaZbior::usunSwiatPkt(XMVECTOR* const) const {}
void FizykaZbior::usunSwiatWektor(XMVECTOR* const) const {}
bool FizykaZbior::wezKolizjePromien(set<float>* const, XMVECTOR const, XMVECTOR const) const {return false;}
void FizykaZbior::wezWierzcholkiSwiat(vector<XMFLOAT3>* const) const {}

void FizykaZbiorZalezny::wykonajKolizjaSiatka(
	WektObiekty3W_* const		obiektyKol,
	IObiekt const* const		ob
	) {
	SiatkaObiekty siatka1, siatka2;
	siatka1 = obiekt->siatka;
	ListaObiekty::const_iterator it;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->wezSiatka(&siatka2);
		siatka1.dopiszSiatka(siatka2);
	}

	Kolizje_ kolizje;
	siatka1.wezKolizje(&kolizje);

	Kolizje_::const_iterator itA;
	set<IObiekt const* const>::const_iterator itB;
	WektObiekty3W_ obKol1;
	for(itA = kolizje.begin(); itA != kolizje.end(); ++itA) {
	for(itB = itA->second.begin(); itB != itA->second.end(); ++itB) {
		itA->first->fiz->wykonajKolizjaSiatka(&obKol1, *itB);
	}
	}

	if(obKol1.size() > 0) {
		obiekt->v = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}
// ---------------------------------------------
FizykaTekst::FizykaTekst(
	ObiektZbior* const		ob
	) : IFizykaZbior(ob)
	{}
// =============================================

