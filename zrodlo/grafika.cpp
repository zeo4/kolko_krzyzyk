#ifndef _GRAFIKA_C_
#define _GRAFIKA_C_

#include "grafika.h"

IGrafika::~IGrafika() {}

void GrafikaObiekt3WPodstawa::aktualizujCoObiekt() const {
	XMMATRIX mSwiat = XMLoadFloat4x4(&obiekt->macPrzesun);
	XMMATRIX mWidok = XMLoadFloat4x4(obiekt->macWidok);
	XMMATRIX mProjekcja = XMLoadFloat4x4(obiekt->macProjekcja);
	// macierz SWP musi być transponowana przed wysłaniem do szadera
	zasoby.daneCoObiekt.macSWP = XMMatrixTranspose(mSwiat * mWidok * mProjekcja);
}
void GrafikaObiekt3WPodstawa::wiaz() const {
	wiazWierzcholki();
	wiazIndeksy();
	wiazTekstura();
}
void GrafikaObiekt3WPodstawa::wiazIndeksy() const {
	zasoby.render->IASetIndexBuffer(obiekt->bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
}
void GrafikaObiekt3WPodstawa::wiazTekstura() const {
	zasoby.render->PSSetShaderResources(0, 1, &obiekt->widokTekstura);
}
void GrafikaObiekt3WPodstawa::wiazWierzcholki() const {
	UINT rozmiar = sizeof(Wierzcholek);
	UINT przesuniecie = 0;
	zasoby.render->IASetVertexBuffers(
		0, 1, &obiekt->bufWierz, &rozmiar, &przesuniecie
	);
}
GrafikaObiekt3WPodstawa::GrafikaObiekt3WPodstawa(
	Obiekt3W* const		ob
	) : obiekt(ob)
	{}
GrafikaObiekt3WPodstawa::~GrafikaObiekt3WPodstawa() {}
void GrafikaObiekt3WPodstawa::rysuj() const {
	aktualizujCoObiekt();
	zasoby.wypelnijCoObiekt();
	zasoby.wiazCoObiekt();
	wiaz();
	zasoby.render->DrawIndexed(obiekt->ind.size(), 0, 0);
}

GrafikaObiektZbiorPodstawa::GrafikaObiektZbiorPodstawa(
	ObiektZbior* const		ob
	) : obiekt(ob)
	{}
GrafikaObiektZbiorPodstawa::~GrafikaObiektZbiorPodstawa() {}
void GrafikaObiektZbiorPodstawa::rysuj() const {
	ListaObiekty::const_iterator it;
	for(it = obiekt->podobiekty.begin(); it != obiekt->podobiekty.end(); ++it) {
		(*it)->graf->rysuj();
	}
}

#endif