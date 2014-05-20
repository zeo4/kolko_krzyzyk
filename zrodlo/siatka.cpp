#pragma once

#include "debug.h"
#include "siatka.h"
#include "obiekty.h"
#include "fizyka.h"

UINT Hasz::operator()(tuple<float const, float const, float const> const) const {
	return 1;
}

float const SiatkaObiekty::rozmObszar = +0.5f;
void SiatkaObiekty::czysc() {
	siatka.clear();
}
void SiatkaObiekty::dodajObiekt(Obiekt3w* const ob) {
	XMVECTOR sr1, sr2, R;
	XMFLOAT3 min, maks;
	float r;
	KluczSiatka_ klucz;
	ob->wezFiz()->wezBrylaGraniczna(&sr1, &sr2, &r);
	R = XMVectorSet(r, r, r, 0.0f);
	XMStoreFloat3(&min, sr1 - R);
	XMStoreFloat3(&maks, sr1 + R);
	for(float x = min.x; x < maks.x; x += rozmObszar) {
	for(float y = min.y; y < maks.y; y += rozmObszar) {
	for(float z = min.z; z < maks.z; z += rozmObszar) {
		klucz = KluczSiatka_(x, y, z);
		liczNrObszar(&klucz);
		siatka.insert(ParaSiatka_(klucz, ob));
	}
	}
	}
}
void SiatkaObiekty::liczNrObszar(KluczSiatka_* const klucz) const {
	// licz numery współrzędnych
	get<0>(*klucz) = floor(get<0>(*klucz) / rozmObszar);
	if(abs(get<0>(*klucz)) > 511) {
		logi.pisz("!", "x poza obszarem kolizjii.");
	}
	get<1>(*klucz) = floor(get<1>(*klucz) / rozmObszar);
	if(abs(get<1>(*klucz)) > 511) {
		logi.pisz("!", "y poza obszarem kolizjii.");
	}
	get<2>(*klucz) = floor(get<2>(*klucz) / rozmObszar);
	if(abs(get<2>(*klucz)) > 511) {
		logi.pisz("!", "z poza obszarem kolizjii.");
	}
}
void SiatkaObiekty::wezSasiedzi(MapaSasiedzi_* const sasiedzi) const {
	sasiedzi->clear();

	Siatka_::const_iterator it, iter;
	pair<Siatka_::const_iterator, Siatka_::const_iterator> p;
	for(it = siatka.begin(); it != siatka.end(); ++it) {
		p = siatka.equal_range(it->first);
		for(iter = p.first; iter != p.second; ++iter) {
			if(it->second == iter->second) {
				continue;
			}

			if(sasiedzi->count(it->second) == 0) {
				sasiedzi->insert(ParaSasiedzi_(it->second, ZbiorSasiedzi_()));
			}
			sasiedzi->at(it->second).insert(iter->second);
		}
	}
}


