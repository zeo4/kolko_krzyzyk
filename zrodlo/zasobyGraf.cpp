#pragma once

#include "zasobyGraf.h"

ZasobyGraf::ZasobyGraf() : bufBladSzad(NULL), bufCoObiekt(NULL), bufGlebiaSzablon(NULL), bufSzadPiks(NULL), bufSzadWierz(NULL), karta(NULL), lancWym(NULL), render(NULL), stanProbkowania(NULL), struktWe(NULL), szadPiks(NULL), szadWierz(NULL), widBufTyl(NULL), widGlebiaSzablon(NULL)
	{}
ZasobyGraf::~ZasobyGraf() {
	if(bufBladSzad != NULL) {
		bufBladSzad->Release();
		bufBladSzad = NULL;
	}
	if(bufCoObiekt != NULL) {
		bufCoObiekt->Release();
		bufCoObiekt = NULL;
	}
	if(bufGlebiaSzablon != NULL) {
		bufGlebiaSzablon->Release();
		bufGlebiaSzablon = NULL;
	}
	if(bufSzadPiks != NULL) {
		bufSzadPiks->Release();
		bufSzadPiks = NULL;
	}
	if(bufSzadWierz != NULL) {
		bufSzadWierz->Release();
		bufSzadWierz = NULL;
	}
	if(karta != NULL) {
		karta->Release();
		karta = NULL;
	}
	if(lancWym != NULL) {
		lancWym->Release();
		lancWym = NULL;
	}
	if(render != NULL) {
		render->Release();
		render = NULL;
	}
	if(stanProbkowania != NULL) {
		stanProbkowania->Release();
		stanProbkowania = NULL;
	}
	if(struktWe != NULL) {
		struktWe->Release();
		struktWe = NULL;
	}
	if(szadPiks != NULL) {
		szadPiks->Release();
		szadPiks = NULL;
	}
	if(szadWierz != NULL) {
		szadWierz->Release();
		szadWierz = NULL;
	}
	if(widBufTyl != NULL) {
		widBufTyl->Release();
		widBufTyl = NULL;
	}
	if(widGlebiaSzablon != NULL) {
		widGlebiaSzablon->Release();
		widGlebiaSzablon = NULL;
	}
}
char const* const ZasobyGraf::wezBladSzad() {
	if(bufBladSzad != NULL){
		char const* tekstBlad = (char const*)(bufBladSzad->GetBufferPointer());
		bufBladSzad->Release();
		bufBladSzad = NULL;
		return tekstBlad;
	}
	return "";
}
ZasobyGraf zasoby;


