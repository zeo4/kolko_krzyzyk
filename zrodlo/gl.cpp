#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFIKA_
#define _GRAFIKA_
#include "grafika.cpp"
#endif

#ifndef _FIZYKA_
#define _FIZYKA_
#include "fizyka.cpp"
#endif

#ifndef _KONTROLER_
#define _KONTROLER_
#include "kontroler.cpp"
#endif

// test
ObiektScena figura1;
ObiektScena figura2;

LRESULT CALLBACK ProcOknoGl(
	HWND		uchOkno,
	UINT		wiad,
	WPARAM		paramW,
	LPARAM		paramL
	) {
	// indywidualna obsługa wiadomości
	switch(wiad){
		// gdy wciśnięty klawisz
		case WM_KEYDOWN:
			// gdy escape
			if(paramW == VK_ESCAPE){
				// niszcz okno
				DestroyWindow(uchOknoGl);
			}
			return 0;
		// gdy okno zniszczone
		case WM_DESTROY:
			// zakończ aplikację
			PostQuitMessage(0);
			return 0;
	}

	// domyślna obsługa wiadomości
	return DefWindowProc(uchOkno, wiad, paramW, paramL);
}
bool Inic3D() {
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

		zasoby.render->OMSetRenderTargets(1, &zasoby.widBufTyl, zasoby.widokGlebiaSzablon);
	}
	catch(Wyjatek wyj){
		ObslugaWyjatek(wyj);
		return false;
	}

	return true;
}
bool InicScena() {
	try{
		Wierzcholek w1[] = {
			Wierzcholek(-0.5f, -0.5f, +0.5f, +0.0f, +1.0f),
			Wierzcholek(+0.0f, +0.5f, +0.5f, +0.5f, +0.0f),
			Wierzcholek(+0.5f, -0.5f, +0.5f, +1.0f, +1.0f),
		};
		Wierzcholek w2[] = {
			Wierzcholek(-1.0f, -1.0f, +1.0f),
			Wierzcholek(-1.0f, +0.0f, +1.0f),
			Wierzcholek(+0.0f, -1.0f, +1.0f),
		};

		DWORD indeksyModel[] = {
			0, 1, 2
		};

		figura1.wgrajWierzcholki(w1, 3);
		figura1.wgrajIndeksy(indeksyModel, 3);
		figura1.tworz();
		figura1.dodDoRender();
		
		figura2.wgrajWierzcholki(w2, 3);
		figura2.wgrajIndeksy(indeksyModel, 3);
		figura2.tworz();
		figura2.dodDoRender();
		
		zasoby.wgrajSzadWierz("szader\\efekty.fx", "SW");
		zasoby.tworzSzadWierz();
		zasoby.wgrajSzadPiks("szader\\efekty.fx", "SP");
		zasoby.tworzSzadPiks();

		zasoby.tworzStruktWe();
		zasoby.wiazStruktWe();

		//ustaw topologię podstaw
		zasoby.render->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		logi.pisz("OK", "Ustaw topologie elementów podstawowych.");

		zasoby.tworzRzutnie();
		zasoby.wiazRzutnie();

		zasoby.tworzCoObiekt();

		zasoby.tworzStanProbkowania();
		zasoby.wiazStanProbkowania();

		figura1.wgrajTeksture("tekstura\\t1.jpg");
		figura1.wiazTeksture();
		figura1.ustawPrzesun(+0.0f, +0.0f, +1.0f);
		figura2.ustawPrzesun(+0.0f, +0.0f, +2.0f);

		return true;
	}
	catch(Wyjatek wyj){
		ObslugaWyjatek(wyj);
		return false;
	}
}
void RysujScena() {
	// czyść widok bufora tylnego
	const FLOAT kolor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	zasoby.render->ClearRenderTargetView(zasoby.widBufTyl, kolor);
	// czyść widok bufora glebi i szablonu
	zasoby.render->ClearDepthStencilView(zasoby.widokGlebiaSzablon, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	zasoby.wiazSzadWierz();
	zasoby.wiazSzadPiks();
	grafik.rysujObiekty();
	
	//przestaw bufory
	zasoby.lancWym->Present(0, 0);
}
void PetlaWiad() {
	// struktura wiadomości
	MSG wiad;

	// weź pierwszą wiadomość
	PeekMessage(&wiad, 0, 0, 0, PM_NOREMOVE);

	// dopóki nie wyjście z programu
	while(wiad.message != WM_QUIT){
		// gdy wiadomość w kolejce
		if(PeekMessage(&wiad, 0, 0, 0, PM_REMOVE)){
			// przerób na wiadomość tekstową
			TranslateMessage(&wiad);

			// ślij wiadomość
			DispatchMessage(&wiad);
		// gdy brak wiadomości w kolejce
		}else{
			// rysuj scenę
			RysujScena();
		}
	}
}
int WINAPI WinMain(
	HINSTANCE		uchAplikacji,
	HINSTANCE		uchPoprzAplikacji,
	PSTR			liniaKomend,
	int				opcjaWysw
	) {
	// test
	
	tikProgramStart = clock();

	// twórz klasę okna głównego
	WNDCLASSEX klasaOkno;
	klasaOkno.cbSize = sizeof(WNDCLASSEX);
	klasaOkno.style = CS_HREDRAW|CS_VREDRAW;
	klasaOkno.lpfnWndProc = ProcOknoGl;
	klasaOkno.cbClsExtra = 0;
	klasaOkno.cbWndExtra = 0;
	klasaOkno.hInstance = uchAplikacji;
	klasaOkno.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	klasaOkno.hCursor = ::LoadCursor(0, IDC_ARROW);
	klasaOkno.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	klasaOkno.lpszMenuName = NULL;
	klasaOkno.lpszClassName = "KolkoKrzyzyk";
	klasaOkno.hIconSm = NULL;

	// rejestruj klasę okna głównego
	if(!RegisterClassEx(&klasaOkno)){
		MessageBox(0, "Rejestracja klasy okna glownego", 0, MB_OK|MB_ICONERROR);
		return 0;
	}

	// stwórz okno główne
	uchOknoGl = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		"KolkoKrzyzyk",
		"Kolko Krzyzyk",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		szerRend,
		wysRend,
		0,
		0,
		uchAplikacji,
		0);
	if(uchOknoGl == 0){
		MessageBox(0, "Tworzenie okna glownego", 0, MB_OK|MB_ICONERROR);
		return 0;
	}

	// pokaż i odśwież okno
	ShowWindow(uchOknoGl, opcjaWysw);
	UpdateWindow(uchOknoGl);

	// inicjalizuj środowisko 3D
	Inic3D();
	InicScena();

	// pętla wiadomości
	PetlaWiad();

	return 1;
}

// input layout zgodny z parametrami wejściowymi szadera wierzchołków - sprawdzić
// template tworzBufor bez danych inicjalizujących
// wypelnijBufWierz(): potestować wypełnianie bufora (skąd UpdateSubresource ma wiedzieć ile skopiować z danych źródłowych)
// Logi dla napisanych funkcji.