#pragma once

#include "globalne.h"
#include "debug.h"

template<class T>
void tworz_bufor(UINT const flgLaczenie, UINT const ilEl, ID3D11Buffer*& bufor) {
	D3D11_BUFFER_DESC opisBuf;
	ZeroMemory(&opisBuf, sizeof(opisBuf));
	opisBuf.ByteWidth = sizeof(T) * ilEl;
	opisBuf.Usage = D3D11_USAGE_DEFAULT;
	opisBuf.BindFlags = flgLaczenie;
	opisBuf.CPUAccessFlags = 0;
	opisBuf.MiscFlags = 0;

	wynik = zasoby.karta->CreateBuffer(&opisBuf, NULL, &bufor);
	SprawdzWynik(wynik, (string("Tworz bufor z laczeniem ")+to_string(flgLaczenie)+string(".")).c_str());
}

struct ZasobyGraf {
	ID3D10Blob*					bufBladSzad; // interfejs do pamięci gdzie siedzi skompilowany szader piksela
	ID3D11Buffer*				bufCoObiekt; // interfejs bufora aktualizowanego co obiekt
	ID3D11Texture2D*			bufGlebiaSzablon;
	ID3D10Blob*					bufSzadPiks; // interfejs do pamięci gdzie siedzi skompilowany szader piksela
	ID3D10Blob*					bufSzadWierz; // interfejs do pamięci gdzie siedzi skompilowany szader wierzchołka
	struct DaneCoObiekt {
		XMMATRIX		macSWP;
						DaneCoObiekt();
	}		daneCoObiekt; // struktura i dane bufora aktualizowanego co obiekt
	ID3D11Device*				karta; // interfejs karty graficznej
	IDXGISwapChain*				lancWym; // interfejs łańcucha wymiany
	ID3D11DeviceContext*		render; // interfejs renderowania
	D3D11_VIEWPORT				rzutnia;
	ID3D11SamplerState*			stanProbkowania;
	ID3D11InputLayout*			struktWe; // interfejs struktury wejścia
	ID3D11PixelShader*			szadPiks; // interfejs szadera piksela
	ID3D11VertexShader*			szadWierz; // interfejs szadera wierzchołka
	ID3D11RenderTargetView*		widBufTyl; // interfejs widoku bufora tylnego
	ID3D11DepthStencilView*		widGlebiaSzablon;
								ZasobyGraf();
								~ZasobyGraf();
	void						tworzCoObiekt();
	void						tworzGlebiaSzablon();
	void						tworzRzutnie();
	void						tworzStanProbkowania();
	void						tworzStruktWe();
	void						tworzSzadPiks();
	void						tworzSzadWierz();
	char const* const			wezBladSzad();
	void						wgrajSzadPiks(
									char const* const,
									char const* const
								);
	void						wgrajSzadWierz(
									char const* const,
									char const* const
								);
	void						wiazCoObiekt() const;
	void						wiazRzutnie() const;
	void						wiazStanProbkowania() const;
	void						wiazStruktWe() const;
	void						wiazSzadPiks() const;
	void						wiazSzadWierz() const;
	void						wypelnijCoObiekt();
} extern zasoby;

