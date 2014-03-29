#pragma once

#include "ifizyka.h"

IFizyka::~IFizyka() {}
bool IFizyka::sprawdzKolizjaPudelkoPudelko(
	FXMVECTOR const		pktMin1,
	FXMVECTOR const		pktMaks1,
	FXMVECTOR const		pktMin2,
	CXMVECTOR const		pktMaks2
	) const {
	XMFLOAT3 roznica1, roznica2;
	XMStoreFloat3(&roznica1, pktMaks2 - pktMin1);
	XMStoreFloat3(&roznica2, pktMaks1 - pktMin2);
	if(roznica1.x > 0 && roznica2.x > 0 && roznica1.y > 0 && roznica2.y > 0 && roznica1.z > 0 && roznica2.z > 0) {
		return true;
	}
	return false;
}
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

IFizyka3W::IFizyka3W() : obiekt(NULL)
	{}
IFizyka3W::IFizyka3W(
	Obiekt3W* const		ob
	) : obiekt(ob)
	{}
IFizyka3W::~IFizyka3W() {}

IFizykaZbior::IFizykaZbior() : obiekt(NULL)
	{}
IFizykaZbior::IFizykaZbior(
	ObiektZbior* const		ob
	) : obiekt(ob)
	{}
IFizykaZbior::~IFizykaZbior() {}

