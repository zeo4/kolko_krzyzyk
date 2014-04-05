#pragma once

#include "grafika.h"

IGrafika::~IGrafika() {}

void Grafika3WPodstawa::aktualizujCoObiekt() const {
	XMMATRIX mSwiat = XMLoadFloat4x4(&obiekt->macPrzesun);
	XMMATRIX mWidok = XMLoadFloat4x4(&obiekt->macWidok);
	XMMATRIX mProjekcja = XMLoadFloat4x4(&obiekt->macProjekcja);
	// macierz SWP musi być transponowana przed wysłaniem do szadera
	zasoby.daneCoObiekt.macSWP = XMMatrixTranspose(mSwiat * mWidok * mProjekcja);
}
void Grafika3WPodstawa::aktualizujPoz(
	) {
	// prędkość
	XMStoreFloat4x4(
		&obiekt->macPrzesun,
		XMMatrixTranslationFromVector(XMLoadFloat3(&obiekt->v)) * XMLoadFloat4x4(&obiekt->macPrzesun)
	);
	obiekt->v = XMFLOAT3(0,0,0);
}
void Grafika3WPodstawa::wiaz() const {
	wiazWierzcholki();
	wiazIndeksy();
	wiazTekstura();
}
void Grafika3WPodstawa::wiazIndeksy() const {
	zasoby.render->IASetIndexBuffer(obiekt->bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
}
void Grafika3WPodstawa::wiazTekstura() const {
	zasoby.render->PSSetShaderResources(0, 1, &obiekt->widokTekstura);
}
void Grafika3WPodstawa::wiazWierzcholki() const {
	UINT rozmiar = sizeof(Wierzcholek);
	UINT przesuniecie = 0;
	zasoby.render->IASetVertexBuffers(
		0, 1, &obiekt->bufWierz, &rozmiar, &przesuniecie
	);
}

Grafika3WPodstawa::Grafika3WPodstawa(
	Obiekt3W* const		ob
	) : obiekt(ob)
	{}
Grafika3WPodstawa::~Grafika3WPodstawa() {}
void Grafika3WPodstawa::rysuj(
	) {
	aktualizujPoz();
	aktualizujCoObiekt();
	zasoby.wypelnijCoObiekt();
	zasoby.wiazCoObiekt();
	wiaz();
	zasoby.render->DrawIndexed(obiekt->ind.size(), 0, 0);
}

GrafikaZbiorPodstawa::GrafikaZbiorPodstawa(
	ObiektZbior* const		ob
	) : obiekt(ob)
	{}
GrafikaZbiorPodstawa::~GrafikaZbiorPodstawa() {}
void GrafikaZbiorPodstawa::rysuj(
	) {
	ListaObiekty::const_iterator it;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->graf->rysuj();
	}
}

