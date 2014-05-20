#pragma once

#include "drzewo8.h"
#include "obiekty.h"
#include "fizyka.h"

Drzewo8::Drzewo8(XMFLOAT3 const srodek, float const promien) : sr(srodek), r(promien) {}
void Drzewo8::dodaj(Obiekt3w* const ob) {
	if(galezie.size() == 0) {
		liscie.insert(ob);
		if(liscie.size() > 1 && r > 0.5f) {
			przepiszDoGalezie();
		}
	} else {
		dodajDoGalezie(ob);
	}
}
void Drzewo8::dodajDoGalezie(Obiekt3w* const ob) {
	byte obszary = liczObszary(ob);
	for(int i = 0; i < 8; ++i) {
		if(obszary & (1 << i)) {
			galezie.at(i)->dodaj(ob);
		}
	}
}
void Drzewo8::dopiszSasiedzi(MapaSasiedzi_* const mapaSasiedzi, ZbiorLiscie_* const zbiorLiscie = NULL) const {
	if(galezie.size() > 0) {
		if(liscie.size() > 0) {
			if(zbiorLiscie == NULL) {
				ZbiorLiscie_ zl;
				zl.insert(&liscie);
				for(int i = 0; i < 8; ++i) {
					galezie.at(i)->dopiszSasiedzi(mapaSasiedzi, &zl);
				}
			} else {
				zbiorLiscie->insert(&liscie);
				for(int i = 0; i < 8; ++i) {
					galezie.at(i)->dopiszSasiedzi(mapaSasiedzi, zbiorLiscie);
				}
				zbiorLiscie->erase(&liscie);
			}
		} else {
			for(int i = 0; i < 8; ++i) {
				galezie.at(i)->dopiszSasiedzi(mapaSasiedzi, zbiorLiscie);
			}
		}
	} else {
		if(zbiorLiscie == NULL) {
			Liscie_::const_iterator itA, itB;
			for(itA = liscie.begin(); itA != liscie.end(); ++itA) {
			for(itB = liscie.begin(); itB != liscie.end(); ++itB) {
				if(itA == itB) {
					continue;
				}

				if(mapaSasiedzi->count(*itA) == 0) {
					mapaSasiedzi->insert(ParaSasiedzi_(*itA, Sasiedzi_()));
				}
				mapaSasiedzi->at(*itA).insert(*itB);
			}
			}
		} else {
			Liscie_::const_iterator itL1, itL2;
			ZbiorLiscie_::const_iterator itZL1, itZL2;
			for(itZL1 = zbiorLiscie->begin(); itZL1 != zbiorLiscie->end(); ++itZL1) {
			for(itL1 = (*itZL1)->begin(); itL1 != (*itZL1)->end(); ++itL1) {
			for(itZL2 = zbiorLiscie->begin(); itZL2 != zbiorLiscie->end(); ++itZL2) {
			for(itL2 = (*itZL2)->begin(); itL2 != (*itZL2)->end(); ++itL2) {
				if(itL1 == itL2) {
					continue;
				}

				if(mapaSasiedzi->count(*itL1) == 0) {
					mapaSasiedzi->insert(ParaSasiedzi_(*itL1, Sasiedzi_()));
				}
				mapaSasiedzi->at(*itL1).insert(*itL2);
			}
			}
			}
			}

			for(itL1 = liscie.begin(); itL1 != liscie.end(); ++itL1) {
			for(itL2 = liscie.begin(); itL2 != liscie.end(); ++itL2) {
				if(itL1 == itL2) {
					continue;
				}

				if(mapaSasiedzi->count(*itL1) == 0) {
					mapaSasiedzi->insert(ParaSasiedzi_(*itL1, Sasiedzi_()));
				}
				mapaSasiedzi->at(*itL1).insert(*itL2);
			}
			}

			for(itZL1 = zbiorLiscie->begin(); itZL1 != zbiorLiscie->end(); ++itZL1) {
			for(itL1 = (*itZL1)->begin(); itL1 != (*itZL1)->end(); ++itL1) {
			for(itL2 = liscie.begin(); itL2 != liscie.end(); ++itL2) {
				if(*itL1 == *itL2) {
					continue;
				}

				if(mapaSasiedzi->count(*itL1) == 0) {
					mapaSasiedzi->insert(ParaSasiedzi_(*itL1, Sasiedzi_()));
				}
				mapaSasiedzi->at(*itL1).insert(*itL2);
			}
			}
			}

			for(itL1 = liscie.begin(); itL1 != liscie.end(); ++itL1) {
			for(itZL1 = zbiorLiscie->begin(); itZL1 != zbiorLiscie->end(); ++itZL1) {
			for(itL2 = (*itZL1)->begin(); itL2 != (*itZL1)->end(); ++itL2) {
				if(*itL1 == *itL2) {
					continue;
				}

				if(mapaSasiedzi->count(*itL1) == 0) {
					mapaSasiedzi->insert(ParaSasiedzi_(*itL1, Sasiedzi_()));
				}
				mapaSasiedzi->at(*itL1).insert(*itL2);
			}
			}
			}
		}
	}
}
byte Drzewo8::liczObszary(Obiekt3w* const ob) const {
	byte obszary = ~0;
	XMVECTOR sr0, sr1, sr2, odl1, odl2;
	float prom;
	ob->wezFiz()->wezBrylaGraniczna(&sr1, &sr2, &prom);
	sr0 = XMLoadFloat3(&sr);
	odl1 = XMVectorAbs(sr0 - sr1);
	odl2 = XMVectorAbs(sr0 - sr2);

	if(XMVectorGetX(odl1) > r && XMVectorGetX(odl2) > r) {
		if(XMVectorGetX(sr1) < sr.x && XMVectorGetX(sr2) < sr.x) {
			obszary = obszary & 85;
		} else if(XMVectorGetX(sr1) > sr.x && XMVectorGetX(sr2) > sr.x) {
			obszary = obszary & 170;
		}
	}
	if(XMVectorGetY(odl1) > r && XMVectorGetY(odl2) > r) {
		if(XMVectorGetY(sr1) < sr.y && XMVectorGetY(sr2) < sr.y) {
			obszary = obszary & 51;
		} else if(XMVectorGetY(sr1) > sr.y && XMVectorGetY(sr2) > sr.y) {
			obszary = obszary & 204;
		}
	}
	if(XMVectorGetZ(odl1) > r && XMVectorGetZ(odl2) > r) {
		if(XMVectorGetZ(sr1) < sr.z && XMVectorGetZ(sr2) < sr.z) {
			obszary = obszary & 15;
		} else if(XMVectorGetZ(sr1) > sr.z && XMVectorGetZ(sr2) > sr.z) {
			obszary = obszary & 240;
		}
	}

	return obszary;
}
void Drzewo8::przepiszDoGalezie() {
	// twórz gałęzie
	tworzGalezie();

	// przepisz z liści do gałęzi
	Liscie_::const_iterator it;
	for(it = liscie.begin(); it != liscie.end(); it) {
		dodajDoGalezie(*it);
		liscie.erase(it++);
	}
}
void Drzewo8::tworzGalezie() {
	XMFLOAT3 SR;
	float R = r / 2;
	galezie = Galezie_(8, NULL);
	SR.x = sr.x - R;
	SR.y = sr.y - R;
	SR.z = sr.z - R;
	galezie.at(0) = new Drzewo8(SR, R);
	SR.x += r;
	galezie.at(1) = new Drzewo8(SR, R);
	SR.y += r;
	galezie.at(3) = new Drzewo8(SR, R);
	SR.x -= r;
	galezie.at(2) = new Drzewo8(SR, R);
	SR.z += r;
	galezie.at(6) = new Drzewo8(SR, R);
	SR.x += r;
	galezie.at(7) = new Drzewo8(SR, R);
	SR.y -= r;
	galezie.at(5) = new Drzewo8(SR, R);
	SR.x -= r;
	galezie.at(4) = new Drzewo8(SR, R);
}
void Drzewo8::wezSasiedzi(MapaSasiedzi_* const mapaSasiedzi) const {
	mapaSasiedzi->clear();
	dopiszSasiedzi(mapaSasiedzi);
}

//UINT Drzewo8::test = 0;
//
//Drzewo8::Drzewo8(float const xMin, float const yMin, float const zMin, float const xMaks, float const yMaks, float const zMaks) : zakresMaks(xMaks, yMaks, zMaks), zakresMin(xMin, yMin, zMin)
//	{}
//void Drzewo8::dodaj(Obiekt3w* const ob) {
//	if(galezie.size() == 0) {
//		liscie.insert(ob);
//		if(liscie.size() > 1 && zakresMaks.x - zakresMin.x > 0.5f) {
//			przepiszObiektyDzieci();
//		}
//	} else {
//		dodajDziecko(ob);
//	}
//}
//void Drzewo8::dodajDziecko(Obiekt3w* const ob) {
//	// wez rogi obiektu
//	WektorRogi_ rogi;
//	wezRogiObiekt(ob, &rogi);
//
//	// gdy obiekt za duży
//	//XMVECTOR w0, w1;
//	//w0 = XMLoadFloat3(&rogi[0]);
//	//w1 = XMLoadFloat3(&rogi[1]);
//	//if(XMVectorGetX(XMVector3LengthEst(w1 - w0)) > (zakresMaks.x - zakresMin.x) / 2) {
//	//	liscie.insert(ob);
//	//	return;
//	//	//jak nieusuwac obiektu z liscia?
//	//}
//
//	// licz obszary zajmowane przez obiekt
//	byte obszaryObiekt = 0;
//	XMFLOAT3 min, maks;
//	for(int i = 0; i < rogi.size(); i += 2) {
//		min = rogi[i];
//		maks = rogi[i+1];
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(min));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(XMFLOAT3(min.x, min.y, maks.z)));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(XMFLOAT3(min.x, maks.y, min.z)));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(XMFLOAT3(min.x, maks.y, maks.z)));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(XMFLOAT3(maks.x, min.y, min.z)));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(XMFLOAT3(maks.x, min.y, maks.z)));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(XMFLOAT3(maks.x, maks.y, min.z)));
//		obszaryObiekt = obszaryObiekt | (1 << liczNr(maks));
//	}
//
//	// dodaj obiekt do gałęzi dla zajmowanych obszarów
//	byte przes;
//	for(int i = 0; i < 8; ++i) {
//		przes = 1 << i;
//		if((obszaryObiekt & przes) == przes) {
//			galezie[i].dodaj(ob);
//		}
//	}
//}
//void Drzewo8::dopiszSasiedzi(MapaSasiedzi_* const sasiedzi) const {
//	if(galezie.size() == 0) {
//		ZbiorOb3w_::const_iterator itA, itB;
//		for(itA = liscie.begin(); itA != liscie.end(); ++itA) {
//		for(itB = liscie.begin(); itB != liscie.end(); ++itB) {
//			if(itA == itB) {
//				continue;
//			}
//
//			if(sasiedzi->count(*itA) == 0) {
//				sasiedzi->insert(ParaSasiedzi_(*itA, ZbiorSasiedzi_()));
//			}
//			sasiedzi->at(*itA).insert(*itB);
//		}
//		}
//	} else {
//		for(int i = 0; i < galezie.size(); ++i) {
//			galezie[i].dopiszSasiedzi(sasiedzi);
//		}
//	}
//}
//byte Drzewo8::liczNr(XMFLOAT3 pkt) {
//	pkt = liczPozLokal(pkt);
//	byte nr = 7;
//
//	if(pkt.x < 0) {
//		nr = nr & 3;
//	}
//	if(pkt.y < 0) {
//		nr = nr & 5;
//	}
//	if(pkt.z < 0) {
//		nr = nr & 6;
//	}
//
//	return nr;
//}
//XMFLOAT3 Drzewo8::liczPozLokal(XMFLOAT3 poz) const {
//	poz.x -= (zakresMin.x + zakresMaks.x) / 2;
//	poz.y -= (zakresMin.y + zakresMaks.y) / 2;
//	poz.z -= (zakresMin.z + zakresMaks.z) / 2;
//	return poz;
//}
//void Drzewo8::przepiszObiektyDzieci() {
//	tworzDzieci();
//
//	ZbiorOb3w_::const_iterator it;
//	for(it = liscie.begin(); it != liscie.end(); it) {
//		dodajDziecko(*it);
//		liscie.erase(it++);
//	}
//
//	//liscie.clear();
//}
//void Drzewo8::tworzDzieci() {
//	test += 8;
//	galezie.clear();
//
//	float odl = (zakresMaks.x - zakresMin.x) / 2;
//	
//	for(float x = zakresMin.x; x < zakresMaks.x; x += odl) {
//	for(float y = zakresMin.y; y < zakresMaks.y; y += odl) {
//	for(float z = zakresMin.z; z < zakresMaks.z; z += odl) {
//		galezie.push_back(Drzewo8(x, y, z, x + odl, y + odl, z + odl));
//	}
//	}
//	}
//}
//void Drzewo8::wezSasiedzi(MapaSasiedzi_* const sasiedzi) const {
//	sasiedzi->clear();
//	dopiszSasiedzi(sasiedzi);
//}
//void Drzewo8::wezRogiKula(XMVECTOR const sr, float const r, vector<XMFLOAT3>* const rogi) const {
//	rogi->clear();
//
//	XMFLOAT3 rog;
//	XMVECTOR R = XMVectorSet(r, r, r, 0.0f);
//	XMStoreFloat3(&rog, sr - R);
//	rogi->push_back(rog);
//	XMStoreFloat3(&rog, sr + R);
//	rogi->push_back(rog);
//}
//void Drzewo8::wezRogiObiekt(Obiekt3w const* const ob, WektorRogi_* const rogi) const {
//	rogi->clear();
//
//	XMVECTOR sr1, sr2;
//	float r;
//	vector<XMFLOAT3> rogiRob;
//	ob->wezFiz()->wezBrylaGraniczna(&sr1, &sr2, &r);
//
//	// przypisz rogi z pierwszej kuli
//	wezRogiKula(sr1, r, &rogiRob);
//	rogi->insert(rogi->end(), rogiRob.begin(), rogiRob.end());
//	wezRogiKula(sr2, r, &rogiRob);
//	rogi->insert(rogi->end(), rogiRob.begin(), rogiRob.end());
//}




