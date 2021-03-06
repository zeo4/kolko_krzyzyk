﻿#pragma once
#include <physics.h>
// -------------------------------------------------------
void Physics::comp_ray_click(XMVECTOR& _pocz, XMVECTOR& _kier, uint32_t const& _x, uint32_t const& _y) const {
	// licz początek
	_pocz = XMLoadFloat3(&cam.pos);

	// licz kierunek
	RECT _kwadr;
	GetClientRect(uch_okno, &_kwadr);
	_kier = XMVectorSet(
		(float(2 * _x) / _kwadr.right - 1) / cam.mtx_proj._11,
		(- float(2 * _y) / _kwadr.bottom + 1) / cam.mtx_proj._22,
		1.0f,
		0
	);
}
float Physics::comp_ray_ob(XMVECTOR const& _pocz, XMVECTOR const& _kier, uint32_t const& _nr_ob) const {
	XMVECTOR _w0, _w1, _w2;
	uint32_t const _uch_mod = graph_par.occludee_mesh_hnd[_nr_ob];
	float _t = 1000.0f, _t1;
	for(uint32_t _i = 0; _i < graph_par.occludee_mesh.get_ind_row(_uch_mod).drug; _i += 3) {
		_w0 = XMLoadFloat3(graph_par.occludee_mesh.get_vert(_uch_mod) + graph_par.occludee_mesh.get_ind(_uch_mod)[_i]);
		_w1 = XMLoadFloat3(graph_par.occludee_mesh.get_vert(_uch_mod) + graph_par.occludee_mesh.get_ind(_uch_mod)[_i+1]);
		_w2 = XMLoadFloat3(graph_par.occludee_mesh.get_vert(_uch_mod) + graph_par.occludee_mesh.get_ind(_uch_mod)[_i+2]);
		_t1 = comp_ray_tri(_pocz, _kier, _w0, _w1, _w2);
		if(_t1 < _t) _t = _t1;
	}
	return _t;
}
float Physics::comp_ray_tri(XMVECTOR const& _pocz, XMVECTOR const& _kier, CXMVECTOR const& _w0, CXMVECTOR const& _w1, CXMVECTOR const& _w2) const {
	//------------------WZORY-------------------------
	// pkt promienia r(t) = pocz + t*kier
	// wekt1 = w1 - w0, wekt2 = w2 - w0
	// pkt trojkata T(u,v) = w0 + u*wekt1 + v*wekt2, dla u >= 0, v >= 0, u+v <= 1
	// m = pocz - w0
	// t = wekt2 * (m x wekt1) / wekt1 * (kier x wekt2)
	// u = m * (kier x wekt2) / wekt1 * (kier x wekt2)
	// v = kier * (m x wekt1) / wekt1 * (kier x wekt2)
	//------------------------------------------------
	XMVECTOR _wekt1 = _w1 - _w0;
	XMVECTOR _wekt2 = _w2 - _w0;
	XMVECTOR _m = _pocz - _w0;
	XMVECTOR _kxwekt2 = XMVector3Cross(_kier, _wekt2);
	XMVECTOR _mxwekt1 = XMVector3Cross(_m, _wekt1);
	XMVECTOR _U = XMVector3Dot(_m, _kxwekt2) / XMVector3Dot(_wekt1, _kxwekt2);
	XMVECTOR _V = XMVector3Dot(_kier, _mxwekt1) / XMVector3Dot(_wekt1, _kxwekt2);
	float _u = XMVectorGetX(_U);
	float _v = XMVectorGetX(_V);

	// jeśli kolizja
	if(_u >= 0 && _v >= 0 && _u + _v <= 1) {
		XMVECTOR _T = XMVector3Dot(_wekt2, _mxwekt1) / XMVector3Dot(_wekt1, _kxwekt2);
		return XMVectorGetX(_T);
	} else {
		return 1000.0f;
	}
}
uint32_t Physics::exe_ob(uint32_t const& _x, uint32_t const& _y) const {
	XMVECTOR _pocz, _pocz1, _kier, _kier1;
	comp_ray_click(_pocz, _kier, _x, _y);
	float _t = 1000.0f, _t1;
	uint32_t _uch_wybr = 0x80000000;
	for(uint32_t _uch_ob = 0; _uch_ob < graph_par.no.wez_poj(); ++_uch_ob) {
		if(graph_par.no.sprawdz_pusty(_uch_ob)) continue;
		_pocz1 = XMVector3TransformCoord(_pocz, XMMatrixInverse(
			&XMVectorSet(0,0,0,0), XMLoadFloat4x4(&phys_par.mtx_world[graph_par.no[_uch_ob]])
		));
		_kier1 = XMVector3TransformNormal(_kier, XMMatrixInverse(
			&XMVectorSet(0,0,0,0), XMLoadFloat4x4(&phys_par.mtx_world[graph_par.no[_uch_ob]])
		));
		_t1 = comp_ray_ob(_pocz1, _kier1, graph_par.no[_uch_ob]);
		if(_t1 < _t) {
			_t = _t1;
			_uch_wybr = _uch_ob;
		}
	}
	return _uch_wybr;
}
void Physics::exe_ob_pos(uint32_t const _i_zad) {
	TaskObPos _z = *(TaskObPos*)task[_i_zad];
	phys_par.pos[phys_par.no[_z.el]] = _z.pos;
	task.erase(_i_zad);
}
void Physics::exe_ob_v(uint32_t const _i_zad) {
	TaksObV _z = *(TaksObV*)task[_i_zad];
	phys_par.v[phys_par.no[_z.el]] = _z.v;
	task.erase(_i_zad);
}
void Physics::exe_ob_pick(uint32_t const _i_zad) {
	TaskObPick _z = *(TaskObPick*)task[_i_zad];
	ResultObPick _w;
	_w.kod_zad = OB_PICK;
	_w.uch_ob = exe_ob(_z.x, _z.y);
	result.push_back((uint8_t*)&_w, sizeof(_w));
	task.erase(_i_zad);
}
void Physics::exe_ob_create(uint32_t const _i_zad) {
	phys_par.no.wstaw(phys_par.pos.get_size());
	phys_par.pos.push_back(XMFLOAT3(0.0f, 0.0f, 0.5f));
	phys_par.v.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	phys_par.mtx_world.push_back(XMFLOAT4X4());
	XMStoreFloat4x4(
		&phys_par.mtx_world[phys_par.mtx_world.get_size()-1], XMMatrixIdentity()
	);
	XMFLOAT3 _t[] = {
		XMFLOAT3(-1.0f, -1.0f, -1.0f),
		XMFLOAT3(-1.0f, 1.0f, -1.0f),
		XMFLOAT3(1.0f, 1.0f, -1.0f),
		XMFLOAT3(1.0f, -1.0f, -1.0f),
		XMFLOAT3(-1.0f, -1.0f, 1.0f),
		XMFLOAT3(-1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, -1.0f, 1.0f),
	};
	phys_par.bound_box.push_back(_t, 8);
}
void Physics::exe_world_curr(uint32_t const _i_zad) {
	for(uint32_t _i = 0; _i < phys_par.mtx_world.get_size(); ++_i) {
		XMStoreFloat3(&phys_par.pos[_i], XMLoadFloat3(&phys_par.pos[_i]) + XMLoadFloat3(&phys_par.v[_i]));
		XMStoreFloat4x4(
			&phys_par.mtx_world[_i],
			XMMatrixTranslationFromVector(XMLoadFloat3(&phys_par.pos[_i]))
		);
	}
}
void Physics::exe_phys_defrag(uint32_t const _i_zad) {
	task.erase(_i_zad);
}
void Physics::exe_tasks() {
	if(task.get_col_size() > 1) {
		uint32_t* _mapa = 0;
		task.sort_comp(_mapa);
		task.sort_exe(_mapa);
		task.erase_dupl_comp(_mapa, FunHasz<uint8_t>(), FunHasz2<uint8_t>());
		task.sort_exe(_mapa);
		free(_mapa);
	}

	for(uint32_t _i = 0; _i < task.get_col_size(); ++_i) {
		if(task.get_row(_i) == task.empty) continue;

		switch(((Task*)task[_i])->code) {
		case OB_POS: exe_ob_pos(_i); break;
		case OB_V: exe_ob_v(_i); break;
		case OB_PICK: exe_ob_pick(_i); break;
		case OB_CREATE: exe_ob_create(_i); break;
		case WORLD_UPDATE: exe_world_curr(_i); break;
		case PHYS_DEFRAG: exe_phys_defrag(_i); break;
		}
	}
}
// -------------------------------------------------------

//IFizyka::IFizyka(IObiekt* const ob) : obiekt (ob)
//	{}
//IFizyka::~IFizyka() {}
//void IFizyka::liczSwiatBezkol() {
//	// licz macierz obrotu
//	XMVECTOR kx, ky, kz, k;
//	kx = XMQuaternionRotationNormal(
//		XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
//		obiekt->obrot.x + obiekt->omega.x * obiekt->tKolizja
//	);
//	ky = XMQuaternionRotationNormal(
//		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
//		obiekt->obrot.y + obiekt->omega.y * obiekt->tKolizja
//	);
//	kz = XMQuaternionRotationNormal(
//		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
//		obiekt->obrot.z + obiekt->omega.z * obiekt->tKolizja
//	);
//	k = XMQuaternionMultiply(kz, XMQuaternionMultiply(kx,ky));
//	XMMATRIX macObrot = XMMatrixRotationQuaternion(k);
//
//	// licz macierz przesunięcia
//	XMMATRIX macPrzes = XMMatrixTranslationFromVector(
//		XMLoadFloat3(&obiekt->przes) + XMLoadFloat3(&obiekt->v) * obiekt->tKolizja
//	);
//
//	// licz macierz świata
//	if(obiekt->rodzic != NULL) {
//		XMMATRIX macSwiatRodzic = XMLoadFloat4x4(&obiekt->rodzic->macSwiatBezkol);
//		XMStoreFloat4x4(&obiekt->macSwiatBezkol, macObrot * macPrzes * macSwiatRodzic);
//	} else {
//		XMStoreFloat4x4(&obiekt->macSwiatBezkol, macObrot * macPrzes);
//	}
//}
//void IFizyka::liczSwiatParam() {
//	// aktualizuj parametry
//	obiekt->tKolizja -= 0.1f; // żeby nie lądował za bryłą graniczną
//	obiekt->obrot.x += obiekt->omega.x * obiekt->tKolizja;
//	obiekt->obrot.y += obiekt->omega.y * obiekt->tKolizja;
//	obiekt->obrot.z += obiekt->omega.z * obiekt->tKolizja;
//	obiekt->omega = XMFLOAT3(0,0,0);
//	obiekt->przes.x += obiekt->v.x * obiekt->tKolizja;
//	obiekt->przes.y += obiekt->v.y * obiekt->tKolizja;
//	obiekt->przes.z += obiekt->v.z * obiekt->tKolizja;
//	obiekt->v = XMFLOAT3(0,0,0);
//	liczSwiatBezkol();
//	obiekt->macSwiat = obiekt->macSwiatBezkol;
//	obiekt->tKolizja = 1.0f;
//}
//bool IFizyka::sprawdzKulaKula(FXMVECTOR const sr1, float const r1, FXMVECTOR const przes1, FXMVECTOR const sr2, float const r2, CXMVECTOR const przes2, float* const t) const {
//	if(r1 < 0 || r2 < 0) {
//		logi.pisz("!", "IFizyka::sprawdzKolizjaKulaKula(): Jeden z promieni mniejszy od zera.");
//		return false;
//	}
//
//	float R = r1 + r2;
//	if(sprawdzPromienKula(sr1, przes1, sr2, R, t)) {
//		return true;
//	} else {
//		return false;
//	}
//}
//bool IFizyka::sprawdzPromienKula(XMVECTOR const pocz, XMVECTOR const kier, XMVECTOR const sr, float const r, float* const t) const {
//	// pkt = pocz + kier * t; <--- promien
//	// (pkt - sr) . (pkt - sr) = r^2; <--- sfera, . - iloczyn skalarny
//	// kier.kier*t^2 + 2*kier.(pocz-sr)*t + (pocz-sr).(pocz-sr)-r^2 = 0;
//
//	// licz t
//	float a = XMVectorGetX(XMVector3Dot(kier, kier));
//	float b = 2 * XMVectorGetX(XMVector3Dot(kier, pocz - sr));
//	float c = XMVectorGetX(XMVector3Dot(pocz - sr, pocz - sr)) - pow(r,2);
//	float delta = pow(b,2) - 4*a*c;
//	if(delta < 0) {
//		return false;
//	} else if (delta == 0) {
//		*t = -b / (2*a);
//	} else if (delta > 0) {
//		*t = (-b-sqrt(delta)) / (2*a);
//	}
//	return true;
//}
//bool IFizyka::sprawdzKolizjaPudelkoPudelko(FXMVECTOR const pktMin1, FXMVECTOR const pktMaks1, FXMVECTOR const pktMin2, CXMVECTOR const pktMaks2) const {
//	XMFLOAT3 roznica1, roznica2;
//	XMStoreFloat3(&roznica1, pktMaks2 - pktMin1);
//	XMStoreFloat3(&roznica2, pktMaks1 - pktMin2);
//	if(roznica1.x > 0 && roznica2.x > 0 && roznica1.y > 0 && roznica2.y > 0 && roznica1.z > 0 && roznica2.z > 0) {
//		return true;
//	}
//	return false;
//}
//bool IFizyka::sprawdzKolizjaTrojkatTrojkat(FXMVECTOR const wA1, FXMVECTOR const wA2, FXMVECTOR const wA3, CXMVECTOR const wB1, CXMVECTOR const wB2, CXMVECTOR const wB3) const {
//	float f1;
//	if(wezKolizjaOdcinekTrojkat(&f1, wA1, wA2, wB1, wB2, wB3)) {
//		return true;
//	}
//	if(wezKolizjaOdcinekTrojkat(&f1, wA1, wA3, wB1, wB2, wB3)) {
//		return true;
//	}
//	if(wezKolizjaOdcinekTrojkat(&f1, wA2, wA3, wB1, wB2, wB3)) {
//		return true;
//	}
//	return false;
//}
//bool IFizyka::wezKolizjaOdcinekTrojkat(float* const odlKolizja, FXMVECTOR const pocz, FXMVECTOR const kon, FXMVECTOR const w0, CXMVECTOR const w1, CXMVECTOR const w2) const {
//	/*XMVECTOR kier = kon - pocz;
//	if(wezKolizjaPromienTrojkat(odlKolizja, pocz, kier, w0, w1, w2)) {
//		XMVECTOR p1 = XMVectorAbs(*pktKol);
//		XMVECTOR p2 = XMVectorAbs(kon);
//		XMVECTOR roznica = p2 - p1;
//		if(XMVectorGetX(roznica) < 0 || XMVectorGetY(roznica) < 0 || XMVectorGetZ(roznica) < 0) {
//			*pktKol = XMVectorSet(+0.0f, +0.0f, +0.0f, +0.0f);
//			return false;
//		} else {
//			return true;
//		}
//	} else {
//		return false;
//	}*/
//	return false;
//}
//bool IFizyka::wezKolizjaPromienTrojkat(float* const t, FXMVECTOR const pocz, FXMVECTOR const kier, FXMVECTOR const w0, CXMVECTOR const w1, CXMVECTOR const w2) const {
//	//------------------WZORY-------------------------
//	// pkt promienia r(t) = pocz + t*kier
//	// wekt1 = w1 - w0, wekt2 = w2 - w0
//	// pkt trojkata T(u,v) = w0 + u*wekt1 + v*wekt2, dla u >= 0, v >= 0, u+v <= 1
//	// m = pocz - w0
//	// t = wekt2 * (m x wekt1) / wekt1 * (kier x wekt2)
//	// u = m * (kier x wekt2) / wekt1 * (kier x wekt2)
//	// v = kier * (m x wekt1) / wekt1 * (kier x wekt2)
//	//------------------------------------------------
//	XMVECTOR wekt1 = w1 - w0;
//	XMVECTOR wekt2 = w2 - w0;
//	XMVECTOR m = pocz - w0;
//	XMVECTOR kxwekt2 = XMVector3Cross(kier, wekt2);
//	XMVECTOR mxwekt1 = XMVector3Cross(m, wekt1);
//	XMVECTOR U = XMVector3Dot(m, kxwekt2) / XMVector3Dot(wekt1, kxwekt2);
//	XMVECTOR V = XMVector3Dot(kier, mxwekt1) / XMVector3Dot(wekt1, kxwekt2);
//	float u = XMVectorGetX(U);
//	float v = XMVectorGetX(V);
//
//	// jeśli kolizja
//	if(u >= 0 && v >=0 && u+v <= 1) {
//		// zwróć punkt kolizji
//		XMVECTOR T = XMVector3Dot(wekt2, mxwekt1) / XMVector3Dot(wekt1, kxwekt2);
//		*t = XMVectorGetX(T);
//		return true;
//	} else {
//		return false;
//	}
//}
//XMVECTOR IFizyka::wezPrzes() const {
//	XMVECTOR przes;
//	XMMatrixDecompose(
//		&XMVectorSet(0,0,0,0), &XMVectorSet(0,0,0,0), &przes, XMLoadFloat4x4(&obiekt->macSwiat)
//	);
//	return przes;
//}
//void IFizyka::wezPrzesunMacierz(XMFLOAT4X4 const mac, XMVECTOR* const przes) const {
//	*przes = XMVectorSet(mac._41, mac._42, mac._43, 0.0f);
//}
//
//// #############################################
//
//void Fizyka3w::usunSwiatPkt(XMVECTOR* const pkt) const {
//	XMMATRIX mOdwrot = XMMatrixInverse(
//		&XMVectorSet(0,0,0,0), XMLoadFloat4x4(&obiekt->macSwiat)
//	);
//	*pkt = XMVector3TransformCoord(*pkt, mOdwrot);
//}
//void Fizyka3w::usunSwiatWektor(XMVECTOR* const w) const {
//	XMMATRIX mOdwrot = XMMatrixInverse(
//		&XMVectorSet(0,0,0,0), XMLoadFloat4x4(&obiekt->macSwiat)
//	);
//	*w = XMVector3TransformNormal(*w, mOdwrot);
//}
//void Fizyka3w::wezKolizjePromien(MapaFloatObiekt_* const odlKolizje, XMVECTOR const pocz, XMVECTOR const kier) const {
//	odlKolizje->clear();
//
//	// weź pary: odległość (od początku promienia) punku kolizjii - obiekt, ułożone rosnąco według odległości (istnieją odległości ujemne)
//	float odl;
//	for(int i = 0; i < obiekt->ind.size()-2; i += 3) {
//		if(wezKolizjaPromienTrojkat(
//			&odl, pocz, kier,
//			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i)).pos),
//			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i+1)).pos),
//			XMLoadFloat3(&obiekt->wierz.at(obiekt->ind.at(i+2)).pos)
//		)) {
//			// zapisz parę: odległość - obiekt
//			odlKolizje->insert(ParaFloatObiekt_(odl, obiekt));
//		}
//	}
//}
//Fizyka3w::Fizyka3w(Obiekt3w* const ob) : IFizyka(ob), obiekt(ob)
//	{}
//Fizyka3w::~Fizyka3w() {}
//void Fizyka3w::liczSwiatyBezkol() {
//	liczSwiatBezkol();
//}
//void Fizyka3w::wezBrylaGraniczna(XMVECTOR* const sr1, XMVECTOR* const sr2, float* const r) const {
//	wezPrzesunMacierz(obiekt->macSwiat, sr1);
//	wezPrzesunMacierz(obiekt->macSwiatBezkol, sr2);
//	*r = 1.0f;
//}
//void Fizyka3w::wezObiekty3W(ZbiorOb3w_* const obiekty) const {
//	obiekty->clear();
//	obiekty->insert(obiekt);
//}
//void Fizyka3w::liczSwiatyParam() {
//	liczSwiatParam();
//}
//void Fizyka3w::zadajRuch(XMVECTOR const przes, float const obrotX, float const obrotY, float const obrotZ) {
//	XMFLOAT3 przesRob;
//	XMStoreFloat3(&przesRob, przes);
//	obiekt->v.x += przesRob.x;
//	obiekt->v.y += przesRob.y;
//	obiekt->v.z += przesRob.z;
//	obiekt->omega.x += obrotX;
//	obiekt->omega.y += obrotY;
//	obiekt->omega.z += obrotZ;
//}
//
//Fizyka3wKoliz::Fizyka3wKoliz() : Fizyka3w(NULL)
//	{}
//void Fizyka3wKoliz::liczCzasKolizja() {
//	if(obiekt->sasiedzi->size() == 0) {
//		return;
//	}
//	if(obiekt->sasiedzi->count(obiekt) == 0) {
//		return;
//	}
//
//	// licz czas kolizji obiektu
//	XMVECTOR pozPrzed, pozPo, kier, pozRob;
//	float r, rRob, t;
//	Sasiedzi_::const_iterator it;
//	wezBrylaGraniczna(&pozPrzed, &pozPo, &r);
//	kier = pozPo - pozPrzed;
//	for(it = obiekt->sasiedzi->at(obiekt).begin(); it != obiekt->sasiedzi->at(obiekt).end(); ++it) {
//		(*it)->fiz->wezBrylaGraniczna(&pozRob, &pozRob, &rRob);
//		if(sprawdzKulaKula(pozPrzed, r, kier, pozRob, rRob, XMVectorSet(0,0,0,0), &t)) {
//			if(t >= 0.0f) {
//				if(t < obiekt->tKolizja) {
//					obiekt->tKolizja = t;
//				}
//			}
//		}
//	}
//}
//
//Fizyka3wNiekoliz::Fizyka3wNiekoliz() : Fizyka3w(NULL)
//	{}
//void Fizyka3wNiekoliz::liczCzasKolizja() {
//	obiekt->tKolizja = 1.0f;
//}
//
//// ---------------------------------------------
//
//FizykaLitera::FizykaLitera(Obiekt3w* const ob) : Fizyka3w(ob)
//	{}
//
//FizykaPostac::FizykaPostac(Obiekt3w* const ob) : Fizyka3w(ob)
//	{}
//
//// #############################################
//
//FizykaZbior::FizykaZbior(ObiektZbior* const ob) : IFizyka(ob), obiekt(ob)
//	{}
//FizykaZbior::~FizykaZbior() {}
//void FizykaZbior::liczCzasKolizja() {
//	ListaObiekty::const_iterator it;
//	obiekt->tKolizja = 1.0f;
//	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
//		(*it)->fiz->liczCzasKolizja();
//		if((*it)->tKolizja < obiekt->tKolizja) {
//			obiekt->tKolizja = (*it)->tKolizja;
//		}
//	}
//}
//void FizykaZbior::liczSwiatyBezkol() {
//	liczSwiatBezkol();
//
//	ListaObiekty::const_iterator it;
//	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
//		(*it)->wezFiz()->liczSwiatyBezkol();
//	}
//}
//void FizykaZbior::wezObiekty3W(ZbiorOb3w_* const obiekty) const {
//	obiekty->clear();
//
//	ListaObiekty::const_iterator it;
//	ZbiorOb3w_ obiektyRob;
//	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
//		(*it)->fiz->wezObiekty3W(&obiektyRob);
//		obiekty->insert(obiektyRob.begin(), obiektyRob.end());
//	}
//}
//void FizykaZbior::liczSwiatyParam() {
//	// wykonaj ruch obiektu
//	liczSwiatParam();
//	
//	// wykonaj ruch podobiektów
//	ListaObiekty::const_iterator it;
//	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
//		(*it)->fiz->liczSwiatyParam();
//	}
//}
//void FizykaZbior::zadajRuch(XMVECTOR const przes, float const obrotX, float const obrotY, float const obrotZ) {
//	XMFLOAT3 przesRob;
//	XMStoreFloat3(&przesRob, przes);
//	obiekt->v.x += przesRob.x;
//	obiekt->v.y += przesRob.y;
//	obiekt->v.z += przesRob.z;
//	obiekt->omega.x += obrotX;
//	obiekt->omega.y += obrotY;
//	obiekt->omega.z += obrotZ;
//}
//
//void FizykaZbiorZalezny::wezKolizjePromien(MapaFloatObiekt_* const odlKolizje, XMVECTOR const pocz, XMVECTOR const kier) const {
//	odlKolizje->clear();
//	ListaObiekty::const_iterator it;
//	MapaFloatObiekt_ odlKol;
//	MapaFloatObiekt_::iterator itKol;
//	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
//		(*it)->wezKolizjePromien(&odlKol, pocz, kier);
//
//		// zamień wszystkie obiekty w mapie na aktualny obiekt
//		for(itKol = odlKol.begin(); itKol != odlKol.end(); ++itKol) {
//			itKol->second = obiekt;
//		}
//
//		odlKolizje->insert(odlKol.begin(), odlKol.end());
//	}
//}
//FizykaZbiorZalezny::FizykaZbiorZalezny() : FizykaZbior(NULL)
//	{}
//
//// ---------------------------------------------
//
//FizykaTekst::FizykaTekst(ObiektZbior* const ob) : FizykaZbior(ob)
//	{}
//
//// #############################################

