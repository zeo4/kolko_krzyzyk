#ifndef _ZASOBYGRAF_H_
#define _ZASOBYGRAF_H_

#include "globalne.h"
#include "debug.h"

template<class TYP>
void tworzBufor(UINT const, UINT const, ID3D11Buffer*&);

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
	char const* const			wezBladSzad();
	void						wgrajSzadWierz(
									char const* const,
									char const* const
								);
	void						tworzSzadWierz();
	void						wiazSzadWierz() const;
	void						wgrajSzadPiks(
									char const* const,
									char const* const
								);
	void						tworzSzadPiks();
	void						wiazSzadPiks() const;
	void						tworzStruktWe();
	void						wiazStruktWe() const;
	void						tworzRzutnie();
	void						wiazRzutnie() const;
	void						tworzCoObiekt();
	void						wypelnijCoObiekt();
	void						wiazCoObiekt() const;
	void						tworzStanProbkowania();
	void						wiazStanProbkowania() const;
	void						tworzGlebiaSzablon();
} zasoby;

#endif