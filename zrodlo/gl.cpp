#pragma once

#include "globalne.h"
#include "debug.h"
#include "logika.h"

#include <wek.h>
#include <uchwyty.h>

HINSTANCE uchAp;

LRESULT CALLBACK ProcOknoGl(HWND uchOkno, UINT wiad, WPARAM paramW, LPARAM paramL) {
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

	Logika logika(uchAp);
	logika.inic();

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
			logika.tworzKolejnaKlatka();
		}
	}
}
int WINAPI WinMain(HINSTANCE uchAplikacji, HINSTANCE uchPoprzAplikacji, PSTR liniaKomend, int opcjaWysw) {
	// test

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
// Tworzyć tekstury w jednej tablicy tekstury a nie każdą osobno.

// Dokończyć przebudowę Drzewo8 pod optymalizację.
// WektorSeg: Wpisywać do wolnych segmentów idąc od początku tablicy _seg.
// Obiekty3w::usun_wykonaj: źle usuwa.
// Obiekty3w::tworz_ob: czemu nie widzi domyślnego parametru.
// Usunąć "ZasobyGraf::tworz_model_teks" i podobne?





