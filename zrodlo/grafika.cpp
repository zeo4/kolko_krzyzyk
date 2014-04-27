#pragma once

#include "grafika.h"

IGrafika::~IGrafika() {}

void Grafika3w::aktualizujCoObiekt(
	) const {
	XMMATRIX mSwiat = XMLoadFloat4x4(&obiekt->macSwiat);
	XMMATRIX mWidok = XMLoadFloat4x4(&obiekt->macWidok);
	XMMATRIX mProjekcja = XMLoadFloat4x4(&obiekt->macProjekcja);
	// macierz SWP musi być transponowana przed wysłaniem do szadera
	zasoby.daneCoObiekt.macSWP = XMMatrixTranspose(mSwiat * mWidok * mProjekcja);
}
void Grafika3w::wiaz(
	) const {
	wiazWierzcholki();
	wiazIndeksy();
	wiazTekstura();
}
void Grafika3w::wiazIndeksy(
	) const {
	zasoby.render->IASetIndexBuffer(obiekt->bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
}
void Grafika3w::wiazTekstura(
	) const {
	zasoby.render->PSSetShaderResources(0, 1, &obiekt->widokTekstura);
}
void Grafika3w::wiazWierzcholki(
	) const {
	UINT rozmiar = sizeof(Wierzcholek);
	UINT przesuniecie = 0;
	zasoby.render->IASetVertexBuffers(
		0, 1, &obiekt->bufWierz, &rozmiar, &przesuniecie
	);
}
Grafika3w::Grafika3w(
	Obiekt3w* const		ob
	) : obiekt(ob)
	{}
Grafika3w::~Grafika3w() {}
void Grafika3w::rysuj(
	) {
	aktualizujCoObiekt();
	zasoby.wypelnijCoObiekt();
	zasoby.wiazCoObiekt();
	wiaz();
	zasoby.render->DrawIndexed(obiekt->ind.size(), 0, 0);
}

GrafikaZbior::GrafikaZbior(
	ObiektZbior* const		ob
	) : obiekt(ob)
	{}
GrafikaZbior::~GrafikaZbior() {}
void GrafikaZbior::rysuj(
	) {
	ListaObiekty::const_iterator it;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->graf->rysuj();
	}
}

