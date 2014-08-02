#pragma once

#include "globalne.h"
#include "debug.h"

struct ZasobyGraf {
	ID3D10Blob*					bufBladSzad; // interfejs do pamięci gdzie siedzi skompilowany szader piksela
	ID3D11Buffer*				bufCoObiekt; // interfejs bufora aktualizowanego co obiekt
	ID3D11Texture2D*			bufGlebiaSzablon;
	ID3D10Blob*					bufSzadPiks; // interfejs do pamięci gdzie siedzi skompilowany szader piksela
	ID3D10Blob*					bufSzadWierz; // interfejs do pamięci gdzie siedzi skompilowany szader wierzchołka
	struct DaneCoObiekt {
		XMMATRIX		macSWP;
						//DaneCoObiekt();
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
									char const* const);
	void						wgrajSzadWierz(
									char const* const,
									char const* const);
	void						wiazCoObiekt() const;
	void						wiazRzutnie() const;
	void						wiazStanProbkowania() const;
	void						wiazStruktWe() const;
	void						wiazSzadPiks() const;
	void						wiazSzadWierz() const;
	void						wypelnijCoObiekt();
} extern zasoby;

