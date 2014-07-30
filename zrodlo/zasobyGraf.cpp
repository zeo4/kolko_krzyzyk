#pragma once

#include "zasobyGraf.h"

ZasobyGraf::DaneCoObiekt::DaneCoObiekt() : macSWP(XMMATRIX(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		{}
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
void ZasobyGraf::tworzGlebiaSzablon() {
	D3D11_TEXTURE2D_DESC opisGlebiaSzablon;
	opisGlebiaSzablon.Width = szerRend;
	opisGlebiaSzablon.Height = wysRend;
	opisGlebiaSzablon.MipLevels = 1;
	opisGlebiaSzablon.ArraySize = 1;
	opisGlebiaSzablon.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	opisGlebiaSzablon.SampleDesc.Count = 1;
	opisGlebiaSzablon.SampleDesc.Quality = 0;
	opisGlebiaSzablon.Usage = D3D11_USAGE_DEFAULT;
	opisGlebiaSzablon.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	opisGlebiaSzablon.CPUAccessFlags = 0;
	opisGlebiaSzablon.MiscFlags = 0;

	wynik = karta->CreateTexture2D(&opisGlebiaSzablon, NULL, &bufGlebiaSzablon);
	SprawdzWynik(wynik, "Tworzenie bufora glebi i szablonu.");

	wynik = karta->CreateDepthStencilView(bufGlebiaSzablon, NULL, &widGlebiaSzablon);
	SprawdzWynik(wynik, "Tworzenie widoku glebi i szablonu.");
}
void ZasobyGraf::tworzRzutnie() {
	ZeroMemory(&rzutnia, sizeof(D3D11_VIEWPORT));
	rzutnia.TopLeftX = 0;
	rzutnia.TopLeftY = 0;
	rzutnia.Width = szerRend;
	rzutnia.Height = wysRend;
	rzutnia.MinDepth = 0.0f;
	rzutnia.MaxDepth = 1.0f;
	logi.pisz("OK", "Tworzenie rzutni.");
}
void ZasobyGraf::tworzStanProbkowania() {
	D3D11_SAMPLER_DESC opisProbkowania;
	ZeroMemory(&opisProbkowania, sizeof(opisProbkowania));
	opisProbkowania.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	opisProbkowania.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	opisProbkowania.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	opisProbkowania.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	opisProbkowania.ComparisonFunc = D3D11_COMPARISON_NEVER;
	opisProbkowania.MinLOD = 0;
	opisProbkowania.MaxLOD = D3D11_FLOAT32_MAX;

	wynik = karta->CreateSamplerState(&opisProbkowania, &stanProbkowania);
	SprawdzWynik(wynik, "Tworzenie stanu probkowania.");
}
void ZasobyGraf::tworzStruktWe() {
	D3D11_INPUT_ELEMENT_DESC struktWierz[] = {
		{"POZYCJA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEKSTURA", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT ilAtryb = ARRAYSIZE(struktWierz);
	wynik = karta->CreateInputLayout(
		struktWierz,
		ilAtryb,
		bufSzadWierz->GetBufferPointer(),
		bufSzadWierz->GetBufferSize(),
		&struktWe);
	SprawdzWynik(wynik, "Tworz strukture wejscia.");
}
void ZasobyGraf::tworzSzadPiks() {
	wynik = karta->CreatePixelShader(
		bufSzadPiks->GetBufferPointer(),
		bufSzadPiks->GetBufferSize(),
		NULL,
		&szadPiks);
	SprawdzWynik(wynik, "Tworz szader pikseli.");
}
void ZasobyGraf::tworzSzadWierz() {
	wynik = karta->CreateVertexShader(
		bufSzadWierz->GetBufferPointer(),
		bufSzadWierz->GetBufferSize(),
		NULL,
		&szadWierz);
	SprawdzWynik(wynik, "Tworz szader wierzcholkow.");
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
void ZasobyGraf::wgrajSzadPiks(char const* const sciezka, char const* const nazwaFunkcji
	) {
	wynik = D3DX11CompileFromFile(
		sciezka,
		0,
		0,
		nazwaFunkcji,
		"ps_4_0",
		0,
		0,
		0,
		&bufSzadPiks,
		&bufBladSzad,
		0);
	SprawdzWynik(
		wynik,
		string("Wgraj szader pikseli: ") + string(wezBladSzad()));
}
void ZasobyGraf::wgrajSzadWierz(char const* const sciezka, char const* const nazwaFunkcji
	) {
	wynik = D3DX11CompileFromFile(
		sciezka,
		0,
		0,
		nazwaFunkcji,
		"vs_4_0",
		0,
		0,
		0,
		&bufSzadWierz,
		&bufBladSzad,
		0);
	SprawdzWynik(
		wynik,
		string("Wgraj szader wierzcholkow: ") + string(wezBladSzad()));
}
void ZasobyGraf::wiazCoObiekt() const {
	if(bufCoObiekt == NULL) {
		logi.pisz("BLAD", "Wiazanie niestworzonego bufora aktualizowanego co obiekt.");
	} else {
		zasoby.render->VSSetConstantBuffers(0, 1, &bufCoObiekt);
	}
}
void ZasobyGraf::wiazRzutnie() const {
	render->RSSetViewports(1, &rzutnia);
	logi.pisz("OK", "Wiazanie rzutni.");
}
void ZasobyGraf::wiazStanProbkowania() const {
	render->PSSetSamplers(0, 1, &stanProbkowania);
	logi.pisz("OK", "Wiazanie stanu probkowania.");
}
void ZasobyGraf::wiazStruktWe() const {
	render->IASetInputLayout(struktWe);
	logi.pisz("OK", "Wiaz strukture wejscia.");
}
void ZasobyGraf::wiazSzadPiks() const {
	if(szadPiks == NULL) {
		logi.pisz("!", "ZasobyGraf::wiazSzadPiks(): Nie powiazano. Szader pikseli jest pusty.");
	} else {
		zasoby.render->PSSetShader(szadPiks, 0, 0);
	}
}
void ZasobyGraf::wiazSzadWierz() const {
	if(szadWierz == NULL) {
		logi.pisz("!", "ZasobyGraf::wiazSzadWierz(): Nie powiazano. Szader wierzcholkow jest pusty.");
	} else {
		zasoby.render->VSSetShader(szadWierz, 0, 0);
	}
}
ZasobyGraf zasoby;


