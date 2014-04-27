#pragma once

#include "siatka.h"

void SiatkaObiekty::dopiszObiekt(
	float						x,
	float						y,
	float						z,
	Obiekt3w const* const		ob
	) {
	siatka.insert(
		pair<float const, Siatka2Obiekty_>(x, Siatka2Obiekty_())
	);
	siatka.at(x).insert(
		pair<float const, Siatka1Obiekty_>(y, Siatka1Obiekty_())
	);
	siatka.at(x).at(y).insert(
		pair<float const, ZbiorOb3wStale_>(z, ZbiorOb3wStale_())
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
void SiatkaObiekty::czysc(
	) {
	siatka.clear();
}
void SiatkaObiekty::ustawWspolnyObiekt(
	Obiekt3w* const		ob
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
	MapaOb3wObiekty3w_&		kolizje
	) const {
	kolizje.clear();

	Siatka3Obiekty_::const_iterator it3;
	Siatka2Obiekty_::const_iterator it2;
	Siatka1Obiekty_::const_iterator it1;
	ZbiorOb3wStale_::const_iterator itA;
	ZbiorOb3wStale_::const_iterator itB;
	for(it3 = siatka.begin(); it3 != siatka.end(); ++it3) {
	for(it2 = it3->second.begin(); it2 != it3->second.end(); ++it2) {
	for(it1 = it2->second.begin(); it1 != it2->second.end(); ++it1) {
	for(itA = it1->second.begin(); itA != it1->second.end(); ++itA) {
	for(itB = it1->second.begin(); itB != it1->second.end(); ++itB) {
		if(itA == itB) {
			continue;
		}

		kolizje.insert(ParaOb3wObiekty3w_(
			*itA, ZbiorOb3wStale_()
		));
		kolizje.at(*itA).insert(*itB);
	}
	}
	}
	}
	}
}
SiatkaObiekty::StalyIteratorX SiatkaObiekty::wezKon(
	) const {
	return siatka.end();
}
bool SiatkaObiekty::wezObiekty(
	ZbiorOb3wStale_&		obiekty,
	float					x,
	float					y,
	float					z
	) const {
	obiekty.clear();

	if(siatka.count(x) == 0) {
		return false;
	}
	if(siatka.at(x).count(y) == 0) {
		return false;
	}
	if(siatka.at(x).at(y).count(z) == 0) {
		return false;
	}
	obiekty = siatka.at(x).at(y).at(z);
	return true;
}
SiatkaObiekty::StalyIteratorX SiatkaObiekty::wezPocz(
	) const {
	return siatka.begin();
}
UINT SiatkaObiekty::wezRozm(
	) const {
	return siatka.size();
}


