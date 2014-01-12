#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

template<
	// Tworzy bufor o podanych parametrach.
	// Przypisuje stworzony bufor do podanego interfejsu.
	class TYP // typ danych bufora
	>
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

struct ZasobyGraf {
	IDXGISwapChain*				lancWym; // interfejs łańcucha wymiany
	ID3D11Device*				karta; // interfejs karty graficznej
	ID3D11DeviceContext*		render; // interfejs renderowania
	ID3D11RenderTargetView*		widBufTyl; // interfejs widoku bufora tylnego
	ID3D10Blob*					bufSzadWierz; // interfejs do pamięci gdzie siedzi skompilowany szader wierzchołka
	ID3D10Blob*					bufSzadPiks; // interfejs do pamięci gdzie siedzi skompilowany szader piksela
	ID3D10Blob*					bufBladSzad; // interfejs do pamięci gdzie siedzi skompilowany szader piksela
	ID3D11VertexShader*			szadWierz; // interfejs szadera wierzchołka
	ID3D11PixelShader*			szadPiks; // interfejs szadera piksela
	ID3D11InputLayout*			struktWe; // interfejs struktury wejścia
	D3D11_VIEWPORT				rzutnia;
	ID3D11Buffer*				bufCoObiekt; // interfejs bufora aktualizowanego co obiekt
	struct DaneCoObiekt {
		XMMATRIX		macSWP;
						DaneCoObiekt();
	}		daneCoObiekt; // struktura i dane bufora aktualizowanego co obiekt
	ID3D11SamplerState*			stanProbkowania;
	ID3D11Texture2D*			buforGlebiaSzablon;
	ID3D11DepthStencilView*		widokGlebiaSzablon;
								ZasobyGraf();
								~ZasobyGraf();
	char const* const			wezBladSzad();
	void						wgrajSzadWierz(
									char const* const,
									char const* const);
	void						tworzSzadWierz();
	void						wiazSzadWierz() const;
	void						wgrajSzadPiks(
									char const* const,
									char const* const);
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
ZasobyGraf::ZasobyGraf() : lancWym(NULL), karta(NULL), render(NULL), widBufTyl(NULL), bufSzadWierz(NULL), bufSzadPiks(NULL), bufBladSzad(NULL), szadWierz(NULL), szadPiks(NULL), struktWe(NULL), bufCoObiekt(NULL), stanProbkowania(NULL), buforGlebiaSzablon(NULL), widokGlebiaSzablon(NULL)
	{}
ZasobyGraf::~ZasobyGraf() {
	logi.piszStart("START", "Niszcz zasoby graficzne.");
	if(widokGlebiaSzablon != NULL) {
		widokGlebiaSzablon->Release();
		widokGlebiaSzablon = NULL;
		logi.pisz("OK", "Niszczenie widoku bufora glebi i szablonu.");
	}
	if(buforGlebiaSzablon != NULL) {
		buforGlebiaSzablon->Release();
		buforGlebiaSzablon = NULL;
		logi.pisz("OK", "Niszczenie bufora glebi i szablonu.");
	}
	if(stanProbkowania != NULL) {
		stanProbkowania->Release();
		stanProbkowania = NULL;
		logi.pisz("OK", "Niszczenie stanu probkowania.");
	}
	if(bufCoObiekt != NULL) {
		bufCoObiekt->Release();
		bufCoObiekt = NULL;
		logi.pisz("OK", "Niszczenie bufora aktualizowanego co obiekt.");
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
	if(bufBladSzad != NULL) {
		bufBladSzad->Release();
		bufBladSzad = NULL;
		logi.pisz("OK", "Niszczenie bufora bledow szadera.");
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
	if(widBufTyl != NULL) {
		widBufTyl->Release();
		widBufTyl = NULL;
		logi.pisz("OK", "Niszczenie widoku bufora tylnego.");
	}
	if(render != NULL) {
		render->Release();
		render = NULL;
		logi.pisz("OK", "Niszczenie interfejsu renderowania.");
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

	wynik = karta->CreateTexture2D(&opisGlebiaSzablon, NULL, &buforGlebiaSzablon);
	SprawdzWynik(wynik, "Tworzenie bufora glebi i szablonu.");

	wynik = karta->CreateDepthStencilView(buforGlebiaSzablon, NULL, &widokGlebiaSzablon);
	SprawdzWynik(wynik, "Tworzenie widoku glebi i szablonu.");
}
ZasobyGraf::DaneCoObiekt::DaneCoObiekt() : macSWP(XMMATRIX(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f))
		{}

struct Wierzcholek {
	XMFLOAT3		poz;
	XMFLOAT2		pozTekstury;
					Wierzcholek(float, float, float);
					Wierzcholek(float, float, float, float, float);
};
Wierzcholek::Wierzcholek(
	float		x,
	float		y,
	float		z
	) : poz(x, y, z)
	{}
Wierzcholek::Wierzcholek(
	float		x, // kształt
	float		y, // kształt
	float		z, // kształt
	float		u, // tekstura
	float		v // tekstura
	) : poz(x, y, z), pozTekstury(u, v)
	{}

class Ekran {
	vector<float>		wspKlik;
public:
						Ekran();
	void				lapWspKlik();
	void				usunEkranWsp();
	vector<float>		wezWspKlik() const;
};
Ekran::Ekran() : wspKlik(2)
	{}
void Ekran::lapWspKlik() {
	POINT wsp;
	GetCursorPos(&wsp);
	ScreenToClient(uchOknoGl, &wsp);
	wspKlik[0] = wsp.x;
	wspKlik[1] = wsp.y;
}
void Ekran::usunEkranWsp() {
	wspKlik[0] = 2.0f * wspKlik[0] / float(szerRend) - 1.0f;
	wspKlik[1] = -(2.0f * wspKlik[1] / float(wysRend) - 1.0f);
}
vector<float> Ekran::wezWspKlik() const {
	return wspKlik;
}

class Projektor {
	float			katProjekcja;
	float			odlBlizszaPlaszcz;
	float			odlDalszaPlaszcz;
	XMFLOAT4X4		macProjekcja;
public:
	void			ustawKat(float const);
	void			ustawBlizszaPlaszcz(float const);
	void			ustawDalszaPlaszcz(float const);
	void			tworzProjekcja();
	XMMATRIX		wezProjekcja() const;
	void			usunProjekcjaZ1(float const, float const, XMVECTOR&) const;
};
void Projektor::ustawKat(
	float const		kat
	) {
	katProjekcja = kat;
}
void Projektor::ustawBlizszaPlaszcz(
	float const		odl
	) {
	odlBlizszaPlaszcz = odl;
}
void Projektor::ustawDalszaPlaszcz(
	float const		odl
	) {
	odlDalszaPlaszcz = odl;
}
void Projektor::tworzProjekcja() {
	XMMATRIX mac = XMMatrixPerspectiveFovLH(
		katProjekcja*3.14f/180,
		float(szerRend)/wysRend,
		odlBlizszaPlaszcz,
		odlDalszaPlaszcz);
	XMStoreFloat4x4(&macProjekcja, mac);
}
XMMATRIX Projektor::wezProjekcja() const {
	return XMLoadFloat4x4(&macProjekcja);
}
void Projektor::usunProjekcjaZ1(
	float const x,
	float const y,
	XMVECTOR& pkt3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macProjekcja);

	// współrzędne 3W przy założeniu, że z = 1 (w ten sposób usuwamy projekcję)
	pkt3W = XMVectorSetX(pkt3W, x / mac._11);
	pkt3W = XMVectorSetY(pkt3W, y / mac._22);
	pkt3W = XMVectorSetZ(pkt3W, 1.0f);
}

class Kamera {
	XMFLOAT4		pozKamery;
	XMFLOAT4		celKamery;
	XMFLOAT4		goraKamery;
	XMFLOAT4X4		macWidok;
public:
	void			ustawPozycja(
							float const,
							float const,
							float const);
	void			ustawCel(
							float const,
							float const,
							float const);
	void			ustawGora(
							float const,
							float const,
							float const);
	void			tworzWidok();
	XMMATRIX		wezWidok() const;
	void			usunWidokPkt(XMVECTOR&) const;
	void			usunWidokWektor(XMVECTOR&) const;
};
void Kamera::ustawPozycja(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat4(&pozKamery, XMVectorSet(x, y, z, 0.0f));
}
void Kamera::ustawCel(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat4(&celKamery, XMVectorSet(x, y, z, 0.0f));
}
void Kamera::ustawGora(
	float const		x,
	float const		y,
	float const		z
	) {
	// czwarty parametr XMVectorSet() nie używany
	XMStoreFloat4(&goraKamery, XMVectorSet(x, y, z, 0.0f));
}
void Kamera::tworzWidok() {
	XMVECTOR w1 = XMLoadFloat4(&pozKamery);
	XMVECTOR w2 = XMLoadFloat4(&celKamery);
	XMVECTOR w3 = XMLoadFloat4(&goraKamery);

	XMStoreFloat4x4(&macWidok, XMMatrixLookAtLH(w1, w2, w3));
	logi.pisz("OK", "Ustaw widok kamery sceny.");
}
XMMATRIX Kamera::wezWidok() const {
	return XMLoadFloat4x4(&macWidok);
}
void Kamera::usunWidokPkt(
	XMVECTOR& pkt3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	pkt3W = XMVector3TransformCoord(pkt3W, macOdwrot);
}
void Kamera::usunWidokWektor(
	XMVECTOR& wektor3W
	) const {
	XMMATRIX mac = XMLoadFloat4x4(&macWidok);
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX macOdwrot = XMMatrixInverse(&w, mac);
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	wektor3W = XMVector3TransformNormal(wektor3W, macOdwrot);
}

class Obiekt3W {
	ID3D11Buffer*					bufIndeksy;
	ID3D11Buffer*					bufWierz;
	vector<DWORD>					ind;
	XMFLOAT4X4						macPrzesun; // pozycja obiektu w świecie
	ID3D11ShaderResourceView*		widokTekstury;
	vector<Wierzcholek>				wierz;
	void							tworzBufIndeksy();
	void							tworzBufWierz();
	void							wypelnijBufIndeksy();
	void							wypelnijBufWierz();
public:
									Obiekt3W();
									~Obiekt3W();
	void							przesun(XMVECTOR const);
	void							tworz();
	void							ustawPrzesun(XMVECTOR const);
	void							usunSwiatPkt(XMVECTOR&) const;
	void							usunSwiatWektor(XMVECTOR&) const;
	XMMATRIX						wezPrzesun() const;
	XMMATRIX						wezSwiat() const;
	void							wgrajIndeksy(
										DWORD const *const,
										UINT const);
	void							wgrajTeksture(string);
	void							wgrajWierzcholki(
										Wierzcholek const *const,
										UINT const);
	void							wgrajWierzIndeksy(
										Wierzcholek const *const,
										UINT const,
										DWORD const *const,
										UINT const);
	void							wiazIndeksy() const;
	void							wiazTeksture() const;
	void							wiazWierz() const;
};
void Obiekt3W::tworzBufIndeksy() {
	tworzBufor<DWORD>(
		D3D11_BIND_INDEX_BUFFER,
		ind.size(),
		bufIndeksy);
}
void Obiekt3W::tworzBufWierz() {
	tworzBufor<Wierzcholek>(
		D3D11_BIND_VERTEX_BUFFER,
		wierz.size(),
		bufWierz);
}
void Obiekt3W::wypelnijBufIndeksy() {
	zasoby.render->UpdateSubresource(bufIndeksy, 0, NULL, &ind[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor indeksow.");
}
void Obiekt3W::wypelnijBufWierz() {
	zasoby.render->UpdateSubresource(bufWierz, 0, NULL, &wierz[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor wierzcholkow.");
}
Obiekt3W::Obiekt3W() : bufIndeksy(NULL), bufWierz(NULL), widokTekstury(NULL)
	{}
Obiekt3W::~Obiekt3W() {
	logi.piszStart("START", "Niszcz obiekt sceny.");
	if(widokTekstury != NULL) {
		widokTekstury->Release();
		widokTekstury = NULL;
		logi.pisz("OK", "Zwolnij widok tekstury obiektu.");
	}
	if(bufWierz != NULL) {
		bufWierz->Release();
		bufWierz = NULL;
		logi.pisz("OK", "Zwolnij bufor wierzcholkow obiektu.");
	}
	if(bufIndeksy != NULL) {
		bufIndeksy->Release();
		bufIndeksy = NULL;
		logi.pisz("OK", "Zwolnij bufor indeksow obiektu.");
	}
	logi.piszStop("STOP", "Niszcz obiekt sceny.");
}
void Obiekt3W::przesun(
	XMVECTOR const		wektor
	) {
	XMStoreFloat4x4(
		&macPrzesun,
		XMMatrixTranslationFromVector(wektor) * XMLoadFloat4x4(&macPrzesun));
}
void Obiekt3W::tworz() {
	logi.piszStart("START", "Tworz obiekt sceny.");
	tworzBufWierz();
	wypelnijBufWierz();
	tworzBufIndeksy();
	wypelnijBufIndeksy();
	logi.piszStop("STOP", "Tworz obiekt sceny.");
}
void Obiekt3W::ustawPrzesun(
	XMVECTOR const		wektor
	) {
	XMStoreFloat4x4(&macPrzesun, XMMatrixTranslationFromVector(wektor));
}
void Obiekt3W::usunSwiatPkt(
	XMVECTOR& pkt3W
	) const {
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR wektorRob;

	XMMATRIX macOdwrotSwiat = XMMatrixInverse(&wektorRob, wezSwiat());
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	pkt3W = XMVector3TransformCoord(pkt3W, macOdwrotSwiat);
}
void Obiekt3W::usunSwiatWektor(
	XMVECTOR& wektor3W
	) const {
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR wektorRob;

	XMMATRIX macOdwrotSwiat = XMMatrixInverse(&wektorRob, wezSwiat());
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	wektor3W = XMVector3TransformNormal(wektor3W, macOdwrotSwiat);
}
XMMATRIX Obiekt3W::wezPrzesun() const {
	return XMLoadFloat4x4(&macPrzesun);
}
XMMATRIX Obiekt3W::wezSwiat() const {
	return XMLoadFloat4x4(&macPrzesun);
}
void Obiekt3W::wgrajIndeksy(
	DWORD const *const		indeksy,
	UINT const				ilIndeksy
	) {
	ind.assign(indeksy, indeksy+ilIndeksy);
	logi.pisz("OK", "Wgraj indeksy.");
}
void Obiekt3W::wgrajTeksture(
	string sciezka
	) {
	wynik = D3DX11CreateShaderResourceViewFromFile(
		zasoby.karta,
		sciezka.c_str(),
		NULL,
		NULL,
		&widokTekstury,
		NULL);
	SprawdzWynik(wynik, "Wgranie tekstury.");
}
void Obiekt3W::wgrajWierzcholki(
	Wierzcholek const *const		wierzcholki,
	UINT const						ilWierz
	) {
	wierz.assign(wierzcholki, wierzcholki+ilWierz);
	logi.pisz("OK", "Wgraj wierzcholki.");
}
void Obiekt3W::wgrajWierzIndeksy(
	Wierzcholek const *const		wierzcholki,
	UINT const						ilWierz,
	DWORD const *const				indeksy,
	UINT const						ilIndeksy
	) {
	wgrajWierzcholki(wierzcholki, ilWierz);
	wgrajIndeksy(indeksy, ilIndeksy);
}
void Obiekt3W::wiazIndeksy() const {
	if(bufIndeksy == NULL) {
		logi.pisz("UWAGA", "Nie powiazano. Bufor indeksow jest pusty.");
	} else {
		zasoby.render->IASetIndexBuffer(bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
	}
}
void Obiekt3W::wiazTeksture() const {
	zasoby.render->PSSetShaderResources(0, 1, &widokTekstury);
	logi.pisz("OK", "Wiazanie tekstury.");
}
void Obiekt3W::wiazWierz() const {
	if(bufWierz == NULL) {
		logi.pisz("UWAGA", "Nie powiazano. Bufor wierzcholkow jest pusty.");
	} else {
		UINT rozmWierz = sizeof(Wierzcholek);
		UINT przesunBufWierz = 0;
		zasoby.render->IASetVertexBuffers(
			0,
			1,
			&bufWierz,
			&rozmWierz,
			&przesunBufWierz);
	}
}
typedef list<Obiekt3W* const>		ListaOb_;

