#pragma once
#include <global.h>
#include <debugs.h>
#include <logic.h>
#include <DXGIDebug.h>
// -------------------------------------------------------
HINSTANCE uchAp;
// -------------------------------------------------------
LRESULT CALLBACK ProcOknoGl(HWND _uch_okno, UINT wiad, WPARAM paramW, LPARAM paramL) {
	// indywidualna obsługa wiadomości
	switch(wiad){
		// gdy wciśnięty klawisz
		case WM_KEYDOWN:
			// gdy escape
			if(paramW == VK_ESCAPE){
				// niszcz okno
				DestroyWindow(uch_okno);
			}
			return 0;
		// gdy okno zniszczone
		case WM_DESTROY:
			// zakończ aplikację
			PostQuitMessage(0);
			return 0;
	}

	// domyślna obsługa wiadomości
	return DefWindowProc(_uch_okno, wiad, paramW, paramL);
}
// -------------------------------------------------------
void PetlaWiad() {
	// struktura wiadomości
	MSG wiad;

	Logika logika;
	logika.init_scene();

	while(1){
		// gdy wiadomość w kolejce
		if(PeekMessage(&wiad, 0, 0, 0, PM_REMOVE)){
			// wyjscie z programu
			if(wiad.message == WM_QUIT) {
				break;
			}
			logika.handle_input(wiad);
			// przerób na wiadomość tekstową
			TranslateMessage(&wiad);
			// ślij wiadomość
			DispatchMessage(&wiad);
		// gdy brak wiadomości w kolejce
		}else{
			// rysuj scenę
			logika.exe();
		}
	}
}
// -------------------------------------------------------
int WINAPI WinMain(HINSTANCE uchAplikacji, HINSTANCE uchPoprzAplikacji, PSTR liniaKomend, int opcjaWysw) {
	// test
	//IDXGIDebug::ReportLiveObjects();

	uchAp = uchAplikacji;

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
	uch_okno = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		"KolkoKrzyzyk",
		"Kolko Krzyzyk",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		GraphDev::scr_size.width,
		GraphDev::scr_size.height,
		0,
		0,
		uchAplikacji,
		0);
	if(uch_okno == 0){
		MessageBox(0, "Tworzenie okna glownego", 0, MB_OK|MB_ICONERROR);
		return 0;
	}

	// pokaż i odśwież okno
	ShowWindow(uch_okno, opcjaWysw);
	UpdateWindow(uch_okno);

	// pętla wiadomości
	PetlaWiad();

	return 1;
}
// -------------------------------------------------------

// input layout zgodny z parametrami wejściowymi szadera wierzchołków - sprawdzić
// template tworzBufor bez danych inicjalizujących
// wypelnijBufWierz(): potestować wypełnianie bufora (skąd UpdateSubresource ma wiedzieć ile skopiować z danych źródłowych)
// Tworzyć kontroler solidnie, szczególnie z uwzględnieniem uchwytu aplikacji. Teraz jest porozrzucany w głównych funkcjach.
// Obslużyć sytuację: Wskaźnik do wskaźnika do wskaźnika ... Tak aby nie można było zmieniać ob2 w: const Wsk<ob1> -> zwykły wsk_ob2 -> ob2 <- to już można zmieniać.
// Obiekty 3W bazują na wierzchołkach utworzonych poprzez tablicę o stałej ilości elementów. Usuwanie takiej tablicy nie wymaga uwagi. Ale co gdy będzie używana tablica wierzchołków stworzona przez new?
// Przy dodaniu macierzy obrotu do klasy obiektu 3W, uwzględnić to w: pobieraniu bryły granicznej, pobieraniu obszaru siatki.
// Kiedy już nowa fizyka zacznie działać pousuwać niepotrzebne funkcje.
// Po dopisaniu zakańczającej klasy FizykiZbioru i FizykiObiektu, sprawdzić czy dodając tylko ją jako przyjaciela klas obiektów, wystarczy.
// Poprawić tworzenie obiektów tak aby to nie świat tylko logika gry tworzyła poszczególne obiekty, które potem sa "podpinane" do świata.
// Napisać dekstruktory do wszystkich klas fizyk i grafik.
// Dopracować wybieranie promieniem tak aby zawsze nie wybierał po modelach i to dodatkowo wszystkich obiektów.
// Kolizje oparte na założeniu że prędkość obiektu (obiekt->v) to jest prędkość per klatka. Nie koniecznie tak musi być później. Gdyby, to dzielić prędkość przez ilosc klatek i potem używać takiej prędkości.
// Przy kolizji zapobiegać "przestrzeleniu" obiektów.
// Cała inicjalizacja do logów.
// Napisać kolizje modeli tak aby się po sobie zsuwały. Wykorzystać siły (F).
// Dorobić przy kolizjach wpływ od kolidujących obiektów i ich rodziców. Nie uwzględnia parametrów rodzica przy sprawdzaniu kolizjii.
// Nie sprawdzać kolizji obiektów z tego samego zbioru, lub przynajmniej rozróżniać czy sprawdzać czy nie w zależności od rodzaju zbioru (kolizyjny, niekolizyjny) albo rodzaju elementów w zbiorze (kolizyjne, niekolizyjne)
// Kolizja liczona na podstawie prostej przesunięcia brył (nie jest liczona po łuku przesunięcia). Dorobić.
// Dopisać kolizje dla szybko poruszających się obiektów.
// W procesie tworzenia klatki najpierw usuwać obiekty, potem wykonywać na nich operacje (tak że gdy wykonywane są na usuniętych, to te wykonają odpowiednią czynność związaną z usunięciem danego obiektu), a dopiero potem dodawać nowe obiekty.
// Tworzyć tekstury w jednej tablicy tekstur a nie każdą osobno.
// Wektory: Za każdym razem kiedy jest zmiana, trzeba jawnie aktualizować nagłówek obszaru pamięci. Wykombinować jak nie aktualizować tego jawnie.
// Usunąć pobieranie wielkości bufora prosto z karty graficznej przy aktualizacji bufora. Transfer GPU -> CPU jest wolny. Niech rozmiar będzie trzymany po stronie CPU również.
// Fizyka: zmienić miejsce pobierania rozmiaru obszaru klienta okna, tak aby pobierany był rzadziej.
// ZasobyGraf: ustawić wszystkie możliwe metody na inline.

// Przetestować Wek2::wstaw_kon(Wek2) czy działa dobrze.
// Ukladac co jakis czas (np. 60 klatek) obiekty wedlug rozmieszczenia w drzewie kolizji, lub ukladac co klatke czesc obiektow wedlug drzewa, w kolejnej klatce kolejna czesc obiektow, itd.. Pomoze to zachowac lokalnosc obiektow przy wykrywaniu kolizji. ALE ROWNIEZ SORTOWANIE WEDLUG MESH/TEX PRZED WYRYSOWANIEM.






