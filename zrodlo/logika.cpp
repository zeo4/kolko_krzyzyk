#pragma once

#include "logika.h"
#include "fizyka.h"

void Logika::uwzglWejscie() {
	BYTE stanKlawiatura[256];
	DIMOUSESTATE stanMysz;
	wejscie.wez(stanKlawiatura, &stanMysz);

	if(obiektWybrany != NULL) {
		if(stanKlawiatura[DIK_W] & 0x80) {
			obiektWybrany->wezFiz()->dodajPredkosc(XMVectorSet(+0.0f, +0.015f, +0.0f, +0.0f));
		}
		if(stanKlawiatura[DIK_S] & 0x80) {
			obiektWybrany->wezFiz()->dodajPredkosc(XMVectorSet(+0.0f, -0.015f, +0.0f, +0.0f));
		}
		if(stanKlawiatura[DIK_A] & 0x80) {
			obiektWybrany->wezFiz()->dodajPredkosc(XMVectorSet(-0.015f, +0.0f, +0.0f, +0.0f));
		}
		if(stanKlawiatura[DIK_D] & 0x80) {
			obiektWybrany->wezFiz()->dodajPredkosc(XMVectorSet(+0.015f, +0.0f, +0.0f, +0.0f));
		}
	}
	if(obiektKursor != NULL) {
		if(stanMysz.lX != 0) {
			float p = stanMysz.lX * 0.001f;
			obiektKursor->wezFiz()->dodajPredkosc(XMVectorSet(p, +0.0f, +0.0f, +0.0f));
		}
		if(stanMysz.lY != 0) {
			float p = -stanMysz.lY * 0.001f;
			obiektKursor->wezFiz()->dodajPredkosc(XMVectorSet(+0.0f, p, +0.0f, +0.0f));
		}
		if(stanMysz.rgbButtons[0] & 0x80) {
			swiat.wezObPromien(&obiektWybrany, obiektKursor);
		}
	}
}
Logika::Logika(
	HINSTANCE const		uchwyt
	) : obiektKursor(NULL), obiektWybrany(NULL), wejscie(uchwyt)
	{}
void Logika::inic3W() {
	logi.piszStart("--->", "Logika::inic3W().");
	try{
	// twórz opis bufora tylnego
	DXGI_MODE_DESC opisBufTylny;
	ZeroMemory(&opisBufTylny, sizeof(opisBufTylny));
	opisBufTylny.Width = szerRend;
	opisBufTylny.Height = wysRend;
	opisBufTylny.RefreshRate.Numerator = 60;
	opisBufTylny.RefreshRate.Denominator = 1;
	opisBufTylny.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	opisBufTylny.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	opisBufTylny.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// twórz opis łańcucha wymiany
	DXGI_SWAP_CHAIN_DESC opisLancWym;
	ZeroMemory(&opisLancWym, sizeof(opisLancWym));
	opisLancWym.BufferDesc = opisBufTylny;
	opisLancWym.SampleDesc.Count = 1;
	opisLancWym.SampleDesc.Quality = 0;
	opisLancWym.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	opisLancWym.BufferCount = 1;
	opisLancWym.OutputWindow = uchOknoGl;
	opisLancWym.Windowed = true;
	opisLancWym.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//twórz interfejsy karty graficznej, renderowania i łańcucha wymiany
	wynik = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&opisLancWym,
		&zasoby.lancWym,
		&zasoby.karta,
		NULL,
		&zasoby.render);
	SprawdzWynik(wynik, "Tworz interfejsy: karty, renderowania i lancucha.");

	//przypisz obiekt bufora tylnego do interfejsu
	ID3D11Texture2D* bufTyl;
	wynik = zasoby.lancWym->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bufTyl);
	SprawdzWynik(wynik, "Przypisz bufor tylny do interfejsu.");
		
	//twórz widok bufora tylnego
	wynik = zasoby.karta->CreateRenderTargetView(bufTyl, NULL, &zasoby.widBufTyl);
	SprawdzWynik(wynik, "Tworz widok bufora tylnego.");
		
	//zwolnij obiekt bufora tylnego
	bufTyl->Release();

	zasoby.tworzGlebiaSzablon();

	zasoby.render->OMSetRenderTargets(1, &zasoby.widBufTyl, zasoby.widGlebiaSzablon);
	}
	catch(Wyjatek wyj){
	ObslugaWyjatek(wyj);
	}
	logi.piszStart("<---", "Logika::inic3W().");
}
void Logika::inicScena() {
	try{
	obiektKursor = swiat.tworzObiektKursor();
	obiektWybrany = swiat.tworzObiektRycerz();
	swiat.tworzObiektSmok();
	Tekst t;
	t.pisz("fps: 60, to i tak duzo.");
	swiat.dodaj(t.wezObiektGraf());
	
	zasoby.wgrajSzadWierz("szader\\efekty.fx", "SW");
	zasoby.tworzSzadWierz();
	zasoby.wiazSzadWierz();
	zasoby.wgrajSzadPiks("szader\\efekty.fx", "SP");
	zasoby.tworzSzadPiks();
	zasoby.wiazSzadPiks();

	zasoby.tworzStruktWe();
	zasoby.wiazStruktWe();

	//ustaw topologię podstaw
	zasoby.render->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	zasoby.tworzRzutnie();
	zasoby.wiazRzutnie();

	zasoby.tworzCoObiekt();
	
	zasoby.tworzStanProbkowania();
	zasoby.wiazStanProbkowania();
	}
	catch(Wyjatek wyj){
	ObslugaWyjatek(wyj);
	}
}
void Logika::tworzKolejnaKlatka() {
	// czyść widok bufora tylnego
	const FLOAT kolor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	zasoby.render->ClearRenderTargetView(zasoby.widBufTyl, kolor);
	// czyść widok bufora glebi i szablonu
	zasoby.render->ClearDepthStencilView(zasoby.widGlebiaSzablon, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	zasoby.wiazSzadWierz();
	zasoby.wiazSzadPiks();

	uwzglWejscie();
	swiat.tworzKolejnaKlatka();

	zasoby.lancWym->Present(0, 0);
}

