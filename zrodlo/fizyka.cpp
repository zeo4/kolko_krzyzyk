#ifndef _FIZYKA_C_
#define _FIZYKA_C_

#include "fizyka.h"

IFizyka::~IFizyka() {}
bool IFizyka::sprawdzKolizjaTrojkatTrojkat(
	FXMVECTOR const		wA1,
	FXMVECTOR const		wA2,
	FXMVECTOR const		wA3,
	CXMVECTOR const		wB1,
	CXMVECTOR const		wB2,
	CXMVECTOR const		wB3
	) const {
	float f1;
	if(wezKolizjaOdcinekTrojkat(&f1, wA1, wA2, wB1, wB2, wB3)) {
		return true;
	}
	if(wezKolizjaOdcinekTrojkat(&f1, wA1, wA3, wB1, wB2, wB3)) {
		return true;
	}
	if(wezKolizjaOdcinekTrojkat(&f1, wA2, wA3, wB1, wB2, wB3)) {
		return true;
	}
	return false;
}
bool IFizyka::wezKolizjaOdcinekTrojkat(
	float* const		odlKolizja,
	FXMVECTOR const		pocz,
	FXMVECTOR const		kon,
	FXMVECTOR const		w0,
	CXMVECTOR const		w1,
	CXMVECTOR const		w2
	) const {
	/*XMVECTOR kier = kon - pocz;
	if(wezKolizjaPromienTrojkat(odlKolizja, pocz, kier, w0, w1, w2)) {
		XMVECTOR p1 = XMVectorAbs(*pktKol);
		XMVECTOR p2 = XMVectorAbs(kon);
		XMVECTOR roznica = p2 - p1;
		if(XMVectorGetX(roznica) < 0 || XMVectorGetY(roznica) < 0 || XMVectorGetZ(roznica) < 0) {
			*pktKol = XMVectorSet(+0.0f, +0.0f, +0.0f, +0.0f);
			return false;
		} else {
			return true;
		}
	} else {
		return false;
	}*/
	return false;
}
bool IFizyka::wezKolizjaPromienTrojkat(
	float* const		odlKol,
	FXMVECTOR const		pocz,
	FXMVECTOR const		kier,
	FXMVECTOR const		w0,
	CXMVECTOR const		w1,
	CXMVECTOR const		w2
	) const {
	//------------------WZORY-------------------------
	// pkt promienia r(t) = poz + t*kier
	// w1 = wierz1 - wierz0, w2 = wierz2 - wierz0
	// pkt trojkata T(u,v) = wierz0 + u*w1 + v*w2, dla u >= 0, v >= 0, u+v <= 1
	// m = poz - wierz0
	// t = w2 * (m x w1) / w1 * (kier x w2)
	// u = m * (kier x w2) / w1 * (kier x w2)
	// v = kier * (m x w1) / w1 * (kier x w2)
	//------------------------------------------------
	XMVECTOR wekt1 = w1 - w0;
	XMVECTOR wekt2 = w2 - w0;
	XMVECTOR m = pocz - w0;
	XMVECTOR kxw2 = XMVector3Cross(kier, wekt2);
	XMVECTOR U = XMVector3Dot(m, kxw2) / XMVector3Dot(w1, kxw2);
	XMVECTOR V = XMVector3Dot(kier, XMVector3Cross(m, wekt1)) / XMVector3Dot(wekt1, kxw2);
	float u = XMVectorGetX(U);
	float v = XMVectorGetX(V);

	// jeśli kolizja
	if(u >= 0 && v >=0 && u+v <= 1) {
		// zwróć punkt kolizji
		XMVECTOR T = XMVector3Dot(wekt2, XMVector3Cross(m, wekt1)) / XMVector3Dot(wekt1, kxw2);
		*odlKol = XMVectorGetX(T);
		return true;
	} else {
		return false;
	}
}

bool FizykaObiekt3WPodstawa::wezBrylaGraniczna(
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
FizykaObiekt3WPodstawa::FizykaObiekt3WPodstawa() : obiekt(NULL)
	{}
FizykaObiekt3WPodstawa::FizykaObiekt3WPodstawa(
	Obiekt3W* const		ob
	) : obiekt(ob)
	{}
FizykaObiekt3WPodstawa::~FizykaObiekt3WPodstawa() {}
void FizykaObiekt3WPodstawa::aktualizujSiatka() {
	// rozmiar pojedynczego obszaru
	float rozm = 0.5f;

	obiekt->siatka.czysc();

	XMVECTOR min; // lewy dolny bliski
	XMVECTOR max; // prawy górny daleki
	wezBrylaGraniczna(&min, &max);

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
void FizykaObiekt3WPodstawa::dodajPredkosc(
	XMVECTOR const		w
	) {
	XMVECTOR v = XMLoadFloat3(&obiekt->v);
	v = v + w;
	XMStoreFloat3(&obiekt->v, v);
}
bool FizykaObiekt3WPodstawa::sprawdzKolizjaBryly(
	IObiekt3W const* const		ob
	) const {
	XMVECTOR pktMinOb1, pktMaxOb1, pktMinOb2, pktMaxOb2;
	if(wezBrylaGraniczna(&pktMinOb1, &pktMaxOb1) && wezBrylaGraniczna(&pktMinOb2, &pktMaxOb2)) {
		XMFLOAT3 roznica1, roznica2;
		XMStoreFloat3(&roznica1, pktMinOb1 - pktMaxOb2);
		XMStoreFloat3(&roznica2, pktMaxOb1 - pktMinOb2);
		if(roznica1.x <= 0 && roznica1.y <= 0 && roznica1.z <= 0 && roznica2.x >= 0 && roznica2.y >= 0 && roznica2.z >= 0) {
			return true;
		}
	}
	return false;
}
bool FizykaObiekt3WPodstawa::sprawdzKolizjaModele(
	IObiekt3W const* const		ob
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
void FizykaObiekt3WPodstawa::usunSwiatPkt(
	XMVECTOR* const		pkt
	) const {
	XMMATRIX mOdwrot = XMMatrixInverse(&XMVectorSet(0,0,0,0), obiekt->wezSwiat());
	*pkt = XMVector3TransformCoord(*pkt, mOdwrot);
}
void FizykaObiekt3WPodstawa::usunSwiatWektor(
	XMVECTOR* const		w
	) const {
	XMMATRIX mOdwrot = XMMatrixInverse(&XMVectorSet(0,0,0,0), obiekt->wezSwiat());
	*w = XMVector3TransformNormal(*w, mOdwrot);
}
bool FizykaObiekt3WPodstawa::wezKolizjePromien(
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
void FizykaObiekt3WPodstawa::wezWierzcholkiSwiat(
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

RaportKolizja FizykaObiekt3WKolizyjny::wykonajZdarzKolizjaSiatka(
	IObiekt3W const* const		ob
	) {


	RaportKolizja raport;
	raport.flgKolizjaModel = true;
	return raport;
}

RaportKolizja FizykaObiekt3WNiekolizyjny::wykonajZdarzKolizjaSiatka(
	IObiekt3W const* const		ob
	) {
	RaportKolizja raport;
	raport.flgKolizjaModel = false;
	return raport;
}

void FizykaObiekt3WNieopozniony::aktualizujPoz() {
	// prędkość
	XMStoreFloat4x4(
		&obiekt->macPrzesun,
		XMMatrixTranslationFromVector(XMLoadFloat3(&obiekt->v)) * XMLoadFloat4x4(&obiekt->macPrzesun)
	);
	obiekt->v = XMFLOAT3(0,0,0);
}

FizykaObiekt3W::FizykaObiekt3W(
	Obiekt3W* const		ob
	) : FizykaObiekt3WPodstawa(ob)
	{}

FizykaObiektZbiorPodstawa::FizykaObiektZbiorPodstawa(
	ObiektZbior* const		ob
	) : obiekt(ob)
	{}
FizykaObiektZbiorPodstawa::~FizykaObiektZbiorPodstawa() {}
void FizykaObiektZbiorPodstawa::aktualizujSiatka() {
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
void FizykaObiektZbiorPodstawa::dodajPredkosc(
	XMVECTOR const		w
	) {
	XMVECTOR v = XMLoadFloat3(&obiekt->v);
	v = v + w;
	XMStoreFloat3(&obiekt->v, v);
}
bool FizykaObiektZbiorPodstawa::sprawdzKolizjaBryly(IObiekt3W const* const) const {return false;}
bool FizykaObiektZbiorPodstawa::sprawdzKolizjaModele(IObiekt3W const* const) const {return false;}
void FizykaObiektZbiorPodstawa::usunSwiatPkt(XMVECTOR* const) const {}
void FizykaObiektZbiorPodstawa::usunSwiatWektor(XMVECTOR* const) const {}
bool FizykaObiektZbiorPodstawa::wezKolizjePromien(set<float>* const, XMVECTOR const, XMVECTOR const) const {return false;}
void FizykaObiektZbiorPodstawa::wezWierzcholkiSwiat(vector<XMFLOAT3>* const) const {}

RaportKolizja FizykaObiektZbiorZalezny::wykonajZdarzKolizjaSiatka(
	IObiekt3W const* const		ob
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
	set<IObiekt3W const* const>::const_iterator itB;
	RaportKolizja raport;
	for(itA = kolizje.begin(); itA != kolizje.end(); ++itA) {
	for(itB = itA->second.begin(); itB != itA->second.end(); ++itB) {
		raport = itA->first->fiz->wykonajZdarzKolizjaSiatka(*itB);
	}
	}

	if(raport.flgKolizjaModel) {
		obiekt->v = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	
	return raport;
}

//class FizykaSwiat {
//	Grafika*			grafika;
//	MapaObiekty3W_		obiektyScena;
//	MapaObiekty3W_		obiektySwiat;
//public:
//	void				tworzKolejnaKlatka();
//};
//void FizykaSwiat::tworzKolejnaKlatka() {
//	wykryjKolizje();
//	grafika->rysujKolejnaKlatka(&obiektySwiat);
//}


#endif