#ifndef _ZASOBYGRAF_C_
#define _ZASOBYGRAF_C_

#include "zasobyGraf.h"

template<class TYP>
void tworzBufor(
	UINT const			flgLaczenie,
	UINT const			ilEl, // ilość elementów danych bufora
	ID3D11Buffer*&		bufor // interfejs bufora
	) {
	D3D11_BUFFER_DESC opisBuf;
	ZeroMemory(&opisBuf, sizeof(opisBuf));
	opisBuf.ByteWidth = sizeof(TYP) * ilEl;
	opisBuf.Usage = D3D11_USAGE_DEFAULT;
	opisBuf.BindFlags = flgLaczenie;
	opisBuf.CPUAccessFlags = 0;
	opisBuf.MiscFlags = 0;

	wynik = zasoby.karta->CreateBuffer(&opisBuf, NULL, &bufor);
	SprawdzWynik(wynik, (string("Tworz bufor z laczeniem ")+to_string(flgLaczenie)+string(".")).c_str());
}

ZasobyGraf::ZasobyGraf() : bufBladSzad(NULL), bufCoObiekt(NULL), bufGlebiaSzablon(NULL), bufSzadPiks(NULL), bufSzadWierz(NULL), karta(NULL), lancWym(NULL), render(NULL), stanProbkowania(NULL), struktWe(NULL), szadPiks(NULL), szadWierz(NULL), widBufTyl(NULL), widGlebiaSzablon(NULL)
	{}
ZasobyGraf::~ZasobyGraf() {
	logi.piszStart("START", "Niszcz zasoby graficzne.");
	if(bufBladSzad != NULL) {
		bufBladSzad->Release();
		bufBladSzad = NULL;
		logi.pisz("OK", "Niszczenie bufora bledow szadera.");
	}
	if(bufCoObiekt != NULL) {
		bufCoObiekt->Release();
		bufCoObiekt = NULL;
		logi.pisz("OK", "Niszczenie bufora aktualizowanego co obiekt.");
	}
	if(bufGlebiaSzablon != NULL) {
		bufGlebiaSzablon->Release();
		bufGlebiaSzablon = NULL;
		logi.pisz("OK", "Niszczenie bufora glebi i szablonu.");
	}
	if(bufSzadPiks != NULL) {
		bufSzadPiks->Release();
		bufSzadPiks = NULL;
		logi.pisz("OK", "Niszczenie bufora szadera pikseli.");
	}
	if(bufSzadWierz != NULL) {
		bufSzadWierz->Release();
		bufSzadWierz = NULL;
		logi.pisz("OK", "Niszczenie bufora szadera wierzcholkow.");
	}
	if(karta != NULL) {
		karta->Release();
		karta = NULL;
		logi.pisz("OK", "Niszczenie interfejsu karty graficznej.");
	}
	if(lancWym != NULL) {
		lancWym->Release();
		lancWym = NULL;
		logi.pisz("OK", "Niszczenie lancucha wymiany.");
	}
	if(render != NULL) {
		render->Release();
		render = NULL;
		logi.pisz("OK", "Niszczenie interfejsu renderowania.");
	}
	if(stanProbkowania != NULL) {
		stanProbkowania->Release();
		stanProbkowania = NULL;
		logi.pisz("OK", "Niszczenie stanu probkowania.");
	}
	if(struktWe != NULL) {
		struktWe->Release();
		struktWe = NULL;
		logi.pisz("OK", "Niszczenie struktury wejsciowej.");
	}
	if(szadPiks != NULL) {
		szadPiks->Release();
		szadPiks = NULL;
		logi.pisz("OK", "Niszczenie szadera pikseli.");
	}
	if(szadWierz != NULL) {
		szadWierz->Release();
		szadWierz = NULL;
		logi.pisz("OK", "Niszczenie szadera wierzcholkow.");
	}
	if(widBufTyl != NULL) {
		widBufTyl->Release();
		widBufTyl = NULL;
		logi.pisz("OK", "Niszczenie widoku bufora tylnego.");
	}
	if(widGlebiaSzablon != NULL) {
		widGlebiaSzablon->Release();
		widGlebiaSzablon = NULL;
		logi.pisz("OK", "Niszczenie widoku bufora glebi i szablonu.");
	}
	logi.piszStop("STOP", "Niszcz zasoby graficzne.");
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
void ZasobyGraf::wgrajSzadWierz(
	char const* const sciezka,
	char const* const nazwaFunkcji
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
void ZasobyGraf::tworzSzadWierz() {
	wynik = karta->CreateVertexShader(
		bufSzadWierz->GetBufferPointer(),
		bufSzadWierz->GetBufferSize(),
		NULL,
		&szadWierz);
	SprawdzWynik(wynik, "Tworz szader wierzcholkow.");
}
void ZasobyGraf::wiazSzadWierz() const {
	if(szadWierz == NULL) {
		logi.pisz("UWAGA", "Nie powiazano. Szader wierzcholkow jest pusty.");
	} else {
		zasoby.render->VSSetShader(szadWierz, 0, 0);
	}
}
void ZasobyGraf::wgrajSzadPiks(
	char const* const sciezka,
	char const* const nazwaFunkcji
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
void ZasobyGraf::tworzSzadPiks() {
	wynik = karta->CreatePixelShader(
		bufSzadPiks->GetBufferPointer(),
		bufSzadPiks->GetBufferSize(),
		NULL,
		&szadPiks);
	SprawdzWynik(wynik, "Tworz szader pikseli.");
}
void ZasobyGraf::wiazSzadPiks() const {
	if(szadPiks == NULL) {
		logi.pisz("UWAGA", "Nie powiazano. Szader pikseli jest pusty.");
	} else {
		zasoby.render->PSSetShader(szadPiks, 0, 0);
	}
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
void ZasobyGraf::wiazStruktWe() const {
	render->IASetInputLayout(struktWe);
	logi.pisz("OK", "Wiaz strukture wejscia.");
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
void ZasobyGraf::wiazRzutnie() const {
	render->RSSetViewports(1, &rzutnia);
	logi.pisz("OK", "Wiazanie rzutni.");
}
void ZasobyGraf::tworzCoObiekt() {
	// bufor już istnieje
	if(bufCoObiekt != NULL) {
		logi.pisz("BLAD", "Tworzenie juz istniejacego bufora aktualizowanego co obiekt.");
	} else {
		tworzBufor<DaneCoObiekt>(
			D3D11_BIND_CONSTANT_BUFFER,
			1,
			bufCoObiekt);
		logi.pisz("OK", "Tworzenie bufora aktualizowanego co obiekt.");
	}
}
void ZasobyGraf::wypelnijCoObiekt() {
	if(bufCoObiekt == NULL) {
		logi.pisz("BLAD", "Wypelnianie niestworzonego bufora aktualizowanego co obiekt.");
		return;
	}
	zasoby.render->UpdateSubresource(bufCoObiekt, 0, NULL, &daneCoObiekt, 0, 0);
}
void ZasobyGraf::wiazCoObiekt() const {
	if(bufCoObiekt == NULL) {
		logi.pisz("BLAD", "Wiazanie niestworzonego bufora aktualizowanego co obiekt.");
	} else {
		zasoby.render->VSSetConstantBuffers(0, 1, &bufCoObiekt);
	}
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
void ZasobyGraf::wiazStanProbkowania() const {
	render->PSSetSamplers(0, 1, &stanProbkowania);
	logi.pisz("OK", "Wiazanie stanu probkowania.");
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
ZasobyGraf::DaneCoObiekt::DaneCoObiekt() : macSWP(XMMATRIX(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		{}

#endif