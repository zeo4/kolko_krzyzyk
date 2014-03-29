#pragma once

#include "fizyka.h"

Fizyka3W::~Fizyka3W() {}
void Fizyka3W::aktualizujParamFiz() {}
void Fizyka3W::aktualizujPoz(
	) {
	// prędkość
	XMStoreFloat4x4(
		&obiekt->macPrzesun,
		XMMatrixTranslationFromVector(XMLoadFloat3(&obiekt->v) + XMLoadFloat3(&obiekt->vRodzic)) * XMLoadFloat4x4(&obiekt->macPrzesun)
	);
	//obiekt->vRodzic = XMFLOAT3(0,0,0);
	obiekt->v = XMFLOAT3(0,0,0);
}
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
	v += w;
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
	WektObiekty3W_* const		obiektyKol,
	Obiekt3W const* const		ob
	) const {
	obiektyKol->clear();
	if(sprawdzKolizjaBryly(ob)) {
		obiektyKol->push_back(obiekt);
	}
}
void Fizyka3W::wezKolizjePromien(
	MapaFloatObiekt_* const		odlKolizje,
	XMVECTOR const				pocz,
	XMVECTOR const				kier
	) const {
	odlKolizje->clear();

	// weź pary: odległość (od początku promienia) punku kolizjii - obiekt, ułożone rosnąco według odległości (istnieją odległości ujemne)
	float odl;
	for(int i = 0; i < obiekt->ind.size()-2; ++i) {
		if(wezKolizjaPromienTrojkat(
			&odl, pocz, kier,
			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i)).poz),
			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i+1)).poz),
			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i+2)).poz)
		)) {
			// zapisz parę: odległość - obiekt
			odlKolizje->insert(ParaFloatObiekt_(odl, obiekt));
		}
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
	obiektyKol->clear();

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
// ---------------------------------------------
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
	v += w;
	XMStoreFloat3(&obiekt->v, v);
}
void FizykaZbior::wezKolidujaceZ3W(
	WektObiekty3W_* const		obiektyKol,
	Obiekt3W const* const		ob
	) const {
	obiektyKol->clear();
	ListaObiekty::const_iterator it;
	WektObiekty3W_ kolidujace;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->fiz->wezKolidujaceZ3W(&kolidujace, ob);
		obiektyKol->insert(obiektyKol->end(), kolidujace.begin(), kolidujace.end());
	}
}

void FizykaZbiorZalezny::aktualizujParamFiz(
	) {
	ListaObiekty::const_iterator it;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->vRodzic = obiekt->v;
	}
	obiekt->v = XMFLOAT3(0,0,0);
}
void FizykaZbiorZalezny::wezKolizjePromien(
	MapaFloatObiekt_* const		odlKolizje,
	XMVECTOR const				pocz,
	XMVECTOR const				kier
	) const {
	odlKolizje->clear();
	ListaObiekty::const_iterator it;
	MapaFloatObiekt_ odlKol;
	MapaFloatObiekt_::iterator itKol;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->wezKolizjePromien(&odlKol, pocz, kier);

		// zamień wszystkie obiekty w mapie na aktualny obiekt
		for(itKol = odlKol.begin(); itKol != odlKol.end(); ++itKol) {
			itKol->second = obiekt;
		}

		odlKolizje->insert(odlKol.begin(), odlKol.end());
	}
}
void FizykaZbiorZalezny::wykonajKolizjaSiatka(
	WektObiekty3W_* const		obiektyKol,
	IObiekt const* const		ob
	) {
	obiektyKol->clear();

	SiatkaObiekty siatka1, siatka2;
	siatka1 = obiekt->siatka;
	ListaObiekty::const_iterator it;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->wezSiatka(&siatka2);
		siatka1.dopiszSiatka(siatka2);
	}

	MapaKolizje_ kolizje;
	siatka1.wezKolizje(&kolizje);

	MapaKolizje_::const_iterator itA;
	set<IObiekt const* const>::const_iterator itB;
	WektObiekty3W_ obKol;
	for(itA = kolizje.begin(); itA != kolizje.end(); ++itA) {
	for(itB = itA->second.begin(); itB != itA->second.end(); ++itB) {
		itA->first->fiz->wykonajKolizjaSiatka(&obKol, *itB);
		obiektyKol->insert(obiektyKol->end(), obKol.begin(), obKol.end());
	}
	}

	if(obiektyKol->size() > 0) {
		obiekt->v = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
}
// ---------------------------------------------
FizykaTekst::FizykaTekst(
	ObiektZbior* const		ob
	) : IFizykaZbior(ob)
	{}
// =============================================

