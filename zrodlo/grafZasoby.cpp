#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

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

class Obiekt3W {
	struct AtrybZmienne {
		XMFLOAT4X4		macPrzesun;
						AtrybZmienne();
	};
	ID3D11Buffer*					bufIndeksy;
	ID3D11Buffer*					bufWierz;
	deque<AtrybZmienne>				historia;
	vector<DWORD>					ind;
	ID3D11ShaderResourceView*		widokTekstura;
	vector<Wierzcholek>				wierz;
	void							tworzBufIndeksy();
	void							tworzBufWierz();
	bool							wezKolizjaPromienTrojkat(
										XMVECTOR* const,
										XMVECTOR const,
										XMVECTOR const,
										UINT const
									) const;
	void							wypelnijBufIndeksy();
	void							wypelnijBufWierz();
public:
									Obiekt3W();
									~Obiekt3W();
	void							przesun(XMVECTOR const);
	void							przesunHistoria();
	void							tworz();
	void							ustawPrzesun(XMVECTOR const);
	void							usunSwiatPkt(XMVECTOR* const) const;
	void							usunSwiatWektor(XMVECTOR* const) const;
	UINT							wezIloscIndeksy() const;
	bool							wezKolizjaPromienModel(
										map<float const, UINT const>* const,
										XMVECTOR const,
										XMVECTOR const
									) const;
	void							wezNormalnaTrojkat(XMVECTOR* const) const;
	void							wezPlaszczTrojkat(XMVECTOR* const) const;
	void							wezPrzesunAkt(XMMATRIX* const) const;
	void							wgrajIndeksy(
										DWORD const *const,
										UINT const);
	void							wgrajTeksture(string);
	void							wgrajWierzcholki(
										Wierzcholek const *const,
										UINT const);
	void							wiazIndeksy() const;
	void							wiazTeksture() const;
	void							wiazWierz() const;
};
Obiekt3W::AtrybZmienne::AtrybZmienne() {
	XMStoreFloat4x4(&macPrzesun, XMMatrixIdentity());
}
void Obiekt3W::tworzBufIndeksy() {
	tworzBufor<DWORD>(
		D3D11_BIND_INDEX_BUFFER,
		ind.size(),
		bufIndeksy
	);
}
void Obiekt3W::tworzBufWierz() {
	tworzBufor<Wierzcholek>(
		D3D11_BIND_VERTEX_BUFFER,
		wierz.size(),
		bufWierz
	);
}
bool Obiekt3W::wezKolizjaPromienTrojkat(
	XMVECTOR* const		pktKol,
	XMVECTOR const		pocz,
	XMVECTOR const		kier,
	UINT const			nrWierzPierwszy
	) const {
	if(nrWierzPierwszy+2 > wierz.size()-1) {
		logi.pisz("!", "sprawdzWektorTrojkat(): Za malo wierzcholkow.");
		*pktKol = XMVectorSet(+0.0f, +0.0f, +0.0f, +0.0f);
		return false;
	}
	//------------------WZORY-------------------------
	// pkt promienia r(t) = poz + t*kier
	// w1 = wierz1 - wierz0, w2 = wierz2 - wierz0
	// pkt trojkata T(u,v) = wierz0 + u*w1 + v*w2, dla u >= 0, v >= 0, u+v <= 1
	// m = poz - wierz0
	// t = w2 * (m x w1) / w1 * (kier x w2)
	// u = m * (kier x w2) / w1 * (kier x w2)
	// v = kier * (m x w1) / w1 * (kier x w2)
	//------------------------------------------------
	XMVECTOR wierz0 = XMLoadFloat3(&wierz[nrWierzPierwszy].poz);
	XMVECTOR wierz1 = XMLoadFloat3(&wierz[nrWierzPierwszy+1].poz);
	XMVECTOR wierz2 = XMLoadFloat3(&wierz[nrWierzPierwszy+2].poz);
	XMVECTOR w1 = wierz1 - wierz0;
	XMVECTOR w2 = wierz2 - wierz0;
	XMVECTOR m = pocz - wierz0;
	XMVECTOR kxw2 = XMVector3Cross(kier, w2);
	XMVECTOR U = XMVector3Dot(m, kxw2) / XMVector3Dot(w1, kxw2);
	XMVECTOR V = XMVector3Dot(kier, XMVector3Cross(m, w1)) / XMVector3Dot(w1, kxw2);
	float u = XMVectorGetX(U);
	float v = XMVectorGetX(V);

	// jeśli kolizja
	if(u >= 0 && v >=0 && u+v <= 1) {
		// zwróć punkt kolizji
		XMVECTOR T = XMVector3Dot(w2, XMVector3Cross(m, w1)) / XMVector3Dot(w1, kxw2);
		float t = XMVectorGetX(T);
		*pktKol = pocz + t * kier;
		return true;
	} else {
		*pktKol = XMVectorSet(+0.0f, +0.0f, +0.0f, +0.0f);
		return false;
	}
}
void Obiekt3W::wypelnijBufIndeksy() {
	zasoby.render->UpdateSubresource(bufIndeksy, 0, NULL, &ind[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor indeksow.");
}
void Obiekt3W::wypelnijBufWierz() {
	zasoby.render->UpdateSubresource(bufWierz, 0, NULL, &wierz[0], 0, 0);
	logi.pisz("OK", "Wypelnij bufor wierzcholkow.");
}
Obiekt3W::Obiekt3W() : bufIndeksy(NULL), bufWierz(NULL), historia(3, AtrybZmienne()), widokTekstura(NULL)
	{}
Obiekt3W::~Obiekt3W() {
	logi.piszStart("--->", "Niszcz obiekt 3W.");
	if(bufIndeksy != NULL) {
		bufIndeksy->Release();
		bufIndeksy = NULL;
		logi.pisz("OK", "Zwolnij bufor indeksow.");
	}
	if(bufWierz != NULL) {
		bufWierz->Release();
		bufWierz = NULL;
		logi.pisz("OK", "Zwolnij bufor wierzcholkow.");
	}
	if(widokTekstura != NULL) {
		widokTekstura->Release();
		widokTekstura = NULL;
		logi.pisz("OK", "Zwolnij widok tekstury.");
	}
	logi.piszStop("<---", "Niszcz obiekt 3W.");
}
void Obiekt3W::przesun(
	XMVECTOR const		w // wektor przesuniecia
	) {
	AtrybZmienne* const a = &historia[historia.size()-1];
	XMStoreFloat4x4(
		&a->macPrzesun,
		XMMatrixTranslationFromVector(w) * XMLoadFloat4x4(&a->macPrzesun)
	);
}
void Obiekt3W::przesunHistoria() {
	historia.pop_front();
	// dodaj do końca kopię ostatniego
	historia.push_back(*(--historia.end()));
}
void Obiekt3W::tworz() {
	logi.piszStart("--->", "Tworz obiekt 3W.");
	tworzBufWierz();
	wypelnijBufWierz();
	tworzBufIndeksy();
	wypelnijBufIndeksy();
	historia.pop_front();
	historia.push_back(historia[historia.size()-1]);
	historia.pop_front();
	historia.push_back(historia[historia.size()-1]);
	logi.piszStop("<---", "Tworz obiekt 3W.");
}
void Obiekt3W::ustawPrzesun(
	XMVECTOR const		wektor
	) {
	AtrybZmienne* const a = &historia[historia.size()-1];
	XMStoreFloat4x4(&a->macPrzesun, XMMatrixTranslationFromVector(wektor));
}
void Obiekt3W::usunSwiatPkt(
	XMVECTOR* const		pkt3W
	) const {
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX m;
	wezPrzesunAkt(&m);
	XMMATRIX mOdwrot = XMMatrixInverse(&w, m);
	// czwarty parametr wektora nie istotny, XMVector3TransformCoord() załatwia obliczenia z nim związane
	*pkt3W = XMVector3TransformCoord(*pkt3W, mOdwrot);
}
void Obiekt3W::usunSwiatWektor(
	XMVECTOR* const		wektor3W
	) const {
	// wartość nie ważna, konieczny dla XMMatrixInverse()
	XMVECTOR w;

	XMMATRIX m;
	wezPrzesunAkt(&m);
	XMMATRIX mOdwrot = XMMatrixInverse(&w, m);
	// czwarty parametr wektora nie istotny, XMVector3TransformNormal() załatwia obliczenia z nim związane
	*wektor3W = XMVector3TransformNormal(*wektor3W, mOdwrot);
}
UINT Obiekt3W::wezIloscIndeksy() const {
	return ind.size();
}
bool Obiekt3W::wezKolizjaPromienModel(
	map<float const, UINT const>* const		trojkatyKol,
	XMVECTOR const							pocz,
	XMVECTOR const							kier
	) const {
	trojkatyKol->clear();
	// weź punkty kolizji promienia z modelem, ułożone od najbliższych początku promienia do najdalszych początku promienia
	float odl;
	XMVECTOR w;
	for(int i = 0; i < wierz.size()-2; i++) {
		if(wezKolizjaPromienTrojkat(&w, pocz, kier, i)) {
			// licz odległość punktu od początku promienia
			w = w - pocz;
			w = XMVectorPow(w, XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f));
			odl = XMVectorGetX(w) + XMVectorGetY(w) + XMVectorGetZ(w);
			// zapisz odległość i numer trójkąta
			trojkatyKol->insert(pair<float const, UINT const>(odl, i));
		}
	}

	// jeśli brak kolizji promienia z obiektem
	if(trojkatyKol->size() == 0) {
		return false;
	} else {
		return true;
	}
}
void Obiekt3W::wezNormalnaTrojkat(
	XMVECTOR* const		n
	) const {
	XMVECTOR wierz1, wierz2, wierz3;
	wierz1 = XMLoadFloat3(&wierz[0].poz);
	wierz2 = XMLoadFloat3(&wierz[1].poz);
	wierz3 = XMLoadFloat3(&wierz[2].poz);
	XMVECTOR wekt1 = wierz2 - wierz1;
	XMVECTOR wekt2 = wierz3 - wierz1;
	*n = XMVector3Cross(wekt1, wekt2);
	*n = XMVector3Normalize(*n);
}
void Obiekt3W::wezPlaszczTrojkat(
	XMVECTOR* const		p
	) const {
	wezNormalnaTrojkat(p);
	float A = XMVectorGetX(*p);
	float B = XMVectorGetY(*p);
	float C = XMVectorGetZ(*p);
	XMVECTOR w = XMLoadFloat3(&wierz[0].poz);
	float x = XMVectorGetX(w);
	float y = XMVectorGetY(w);
	float z = XMVectorGetZ(w);
	float D = -(A*x + B*y + C*z);
	*p = XMVectorSet(A, B, C, D);
}
void Obiekt3W::wezPrzesunAkt(
	XMMATRIX* const		m
	) const {
	AtrybZmienne const* const a = &historia[historia.size()-2];
	*m = XMLoadFloat4x4(&a->macPrzesun);
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
		&widokTekstura,
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
void Obiekt3W::wiazIndeksy() const {
	if(bufIndeksy == NULL) {
		logi.pisz("UWAGA", "Nie powiazano. Bufor indeksow jest pusty.");
	} else {
		zasoby.render->IASetIndexBuffer(bufIndeksy, DXGI_FORMAT_R32_UINT, 0);
	}
}
void Obiekt3W::wiazTeksture() const {
	zasoby.render->PSSetShaderResources(0, 1, &widokTekstura);
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
typedef map<UINT const, Obiekt3W* const>		MapaObiekty3W_;



