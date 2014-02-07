#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

#ifndef _GRAFZASOBY_
#define _GRAFZASOBY_
#include "grafZasoby.cpp"
#endif

#ifndef _GRAF_
#define _GRAF_
#include "graf.cpp"
#endif

#ifndef _FIZ_
#define _FIZ_
#include "fiz.cpp"
#endif

#ifndef _WEJ_
#define _WEJ_
#include "wejscie.cpp"
#endif

#ifndef _GRA_
#define _GRA_
#include "gra.cpp"
#endif

#ifndef _TESTY_
#define _TESTY_
#include "testy.cpp"
#endif

// test
HINSTANCE		uchAp;

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
void PetlaWiad() {
	// struktura wiadomości
	MSG wiad;

	Gra gra(uchAp);
	gra.inic3W();
	gra.inicScena();

	while(1){
		// gdy wiadomość w kolejce
		if(PeekMessage(&wiad, 0, 0, 0, PM_REMOVE)){
			// wyjscie z programu
			if(wiad.message == WM_QUIT) {
				break;
			}
			// przerób na wiadomość tekstową
			TranslateMessage(&wiad);
			// ślij wiadomość
			DispatchMessage(&wiad);
		// gdy brak wiadomości w kolejce
		}else{
			// rysuj scenę
			gra.wyswietlScena();
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

	uchAp = uchAplikacji;

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

	// pętla wiadomości
	PetlaWiad();

	return 1;
}

// input layout zgodny z parametrami wejściowymi szadera wierzchołków - sprawdzić
// template tworzBufor bez danych inicjalizujących
// wypelnijBufWierz(): potestować wypełnianie bufora (skąd UpdateSubresource ma wiedzieć ile skopiować z danych źródłowych)
// Logi dla napisanych funkcji.
// Tworzyć kontroler solidnie, szczególnie z uwzględnieniem uchwytu aplikacji. Teraz jest porozrzucany w głównych funkcjach.
// Sprawdzić czy "map[klucz];" nadpisze linię "map[klucz] = wartość;". Jeśli tak to poprawić Logika::niszczObiektSwiat().
// Obslużyć sytuację: Wskaźnik do wskaźnika do wskaźnika ... Tak aby nie można było zmieniać ob2 w: const Wsk<ob1> -> zwykły wsk_ob2 -> ob2 <- to już można zmieniać.
// Obiekty 3W bazują na wierzchołkach utworzonych poprzez tablicę o stałej ilości elementów. Usuwanie takiej tablicy nie wymaga uwagi. Ale co gdy będzie używana tablica wierzchołków stworzona przez new?
// Wybieranie prawie działa. Dlaczego działają złe macierze z książki (pdf'a)?
// Projektor powinien być w kamerze.


