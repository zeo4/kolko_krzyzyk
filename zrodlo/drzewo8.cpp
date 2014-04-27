#pragma once

#include "drzewo8.h"
#include "obiekty.h"
#include "fizyka.h"

Drzewo8::Drzewo8(
	XMFLOAT3 const		min,
	XMFLOAT3 const		max
	) : zakresMaks(max), zakresMin(min)
	{}
void Drzewo8::dodaj(
	Obiekt3w* const		ob
	) {
	if(galezie.size() == 0) {
		liscie.insert(ob);
		if(liscie.size() > 4 && zakresMaks.x - zakresMin.x > 0.5f) {
			przepiszObiektyDzieci();
		}
	} else {
		dodajDziecko(ob);
	}
}
void Drzewo8::dodajDziecko(
	Obiekt3w* const		ob
	) {
	vector<XMFLOAT3> rogi;
	wezRogiObiekt(ob, &rogi);

	// licz obszary zajmowane przez obiekt
	byte obszaryObiekt = 0;
	for(int i = 0; i < rogi.size(); ++i) {
		obszaryObiekt = obszaryObiekt | (1 << liczNr(rogi[i]));
	}

	// dodaj obiekt do gałęzi dla zajmowanych obszarów
	byte przes;
	for(int i = 0; i < 8; ++i) {
		przes = 1 << i;
		if((obszaryObiekt & przes) == przes) {
			galezie[i].dodaj(ob);
		}
	}
}
byte Drzewo8::liczNr(
	XMFLOAT3		pkt
	) {
	pkt = liczPozLokal(pkt);
	byte nr = 7;

	if(pkt.x < 0) {
		nr = nr & 3;
	}
	if(pkt.y < 0) {
		nr = nr & 5;
	}
	if(pkt.z < 0) {
		nr = nr & 6;
	}

	return nr;
}
XMFLOAT3 Drzewo8::liczPozLokal(
	XMFLOAT3		poz
	) const {
	poz.x -= (zakresMin.x + zakresMaks.x) / 2;
	poz.y -= (zakresMin.y + zakresMaks.y) / 2;
	poz.z -= (zakresMin.z + zakresMaks.z) / 2;
	return poz;
}
void Drzewo8::przepiszObiektyDzieci(
	) {
	tworzDzieci();

	ZbiorOb3w_::const_iterator it;
	for(it = liscie.begin(); it != liscie.end(); ++it) {
		dodajDziecko(*it);
	}

	liscie.clear();
}
void Drzewo8::tworzDzieci(
	) {
	galezie.clear();

	float odl = (zakresMaks.x - zakresMin.x) / 2;
	
	for(float x = zakresMin.x; x < zakresMaks.x; x += odl) {
	for(float y = zakresMin.y; y < zakresMaks.y; y += odl) {
	for(float z = zakresMin.z; z < zakresMaks.z; z += odl) {
		galezie.push_back(Drzewo8(XMFLOAT3(x, y, z), XMFLOAT3(x+odl, y+odl, z+odl)));
	}
	}
	}
}
void Drzewo8::wezSasiedzi(
	MapaOb3wObiekty3w_* const		sasiedzi
	) const {
	sasiedzi->clear();

	if(galezie.size() == 0) {
		ZbiorOb3w_::const_iterator itA, itB;
		for(itA = liscie.begin(); itA != liscie.end(); ++itA) {
		for(itB = liscie.begin(); itB != liscie.end(); ++itB) {
			if(itA == itB) {
				continue;
			}

			sasiedzi->insert(ParaOb3wObiekty3w_(*itA, ZbiorOb3wStale_()));
			sasiedzi->at(*itA).insert(*itB);
		}
		}
	} else {
		MapaOb3wObiekty3w_ sasiedziRob;
		for(int i = 0; i < galezie.size(); ++i) {
			galezie[i].wezSasiedzi(&sasiedziRob);
			sasiedzi->insert(sasiedziRob.begin(), sasiedziRob.end());
		}
	}
}
void Drzewo8::wezRogiKula(
	XMVECTOR const				sr,
	float const					r,
	vector<XMFLOAT3>* const		rogi
	) const {
	rogi->clear();

	XMFLOAT3 min, maks, rog;
	XMVECTOR R = XMVectorSet(r, r, r, 0.0f);
	XMStoreFloat3(&min, sr - R);
	XMStoreFloat3(&maks, sr + R);
	rog = maks;
	rogi->push_back(rog);
	rog.y = min.y;
	rogi->push_back(rog);
	rog.z = min.z;
	rogi->push_back(rog);
	rog.y = maks.y;
	rogi->push_back(rog);
	rog = maks;
	rog.x = min.x;
	rogi->push_back(rog);
	rog.y = min.y;
	rogi->push_back(rog);
	rog.z = min.z;
	rogi->push_back(rog);
	rog.y = maks.y;
	rogi->push_back(rog);
}
void Drzewo8::wezRogiObiekt(
	Obiekt3w const* const		ob,
	vector<XMFLOAT3>* const		rogi
	) const {
	rogi->clear();

	XMVECTOR sr1, sr2;
	float r;
	vector<XMFLOAT3> rogiRob;
	ob->wezFiz()->wezBrylaGraniczna(&sr1, &sr2, &r);

	// przypisz rogi z pierwszej kuli
	wezRogiKula(sr1, r, &rogiRob);
	rogi->insert(rogi->end(), rogiRob.begin(), rogiRob.end());
	wezRogiKula(sr2, r, &rogiRob);
	rogi->insert(rogi->end(), rogiRob.begin(), rogiRob.end());
}




