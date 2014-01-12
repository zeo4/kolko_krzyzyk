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

#ifndef _GRAFZDARZENIA_
#define _GRAFZDARZENIA_
#include "grafZdarzenia.cpp"
#endif

#ifndef _GRAF_
#define _GRAF_
#include "graf.cpp"
#endif

#ifndef _FIZZDARZENIA_
#define _FIZZDARZENIA_
#include "fizZdarzenia.cpp"
#endif

#ifndef _FIZ_
#define _FIZ_
#include "fiz.cpp"
#endif

class Kontrolery {
	IDirectInputDevice8*		klawiatura;
	IDirectInputDevice8*		mysz;
	Obiekt3W*					obiekt;
	BYTE						stanKlawiatura[256];
	DIMOUSESTATE				stanMysz;
	HINSTANCE					uchAplikacji;
	IDirectInput8*				wejscie;
	ListaZdarzFiz_*				zdarzeniaFiz;
	void						inicjalizuj();
	void						tworzKlawiatura();
	void						tworzMysz();
	void						tworzWejscie() const;
	void						ustawKlawiatura() const;
	void						ustawMysz() const;
	void						wezStanKlawiatura();
	void						wezStanMysz();
public:
								Kontrolery(HINSTANCE const);
								~Kontrolery();
	void						przypnijListaFiz(ListaZdarzFiz_* const);
	void						tworzZdarzenia();
	void						ustawObiekt(Obiekt3W* const);
};
void Kontrolery::inicjalizuj() {
	tworzWejscie();
	tworzKlawiatura();
	tworzMysz();
	ustawKlawiatura();
	ustawMysz();
}
void Kontrolery::tworzKlawiatura() {
	wynik = wejscie->CreateDevice(GUID_SysKeyboard, &klawiatura, NULL);
	SprawdzWynik(wynik, "Tworzenie klawiatury.");
}
void Kontrolery::tworzMysz() {
	wynik = wejscie->CreateDevice(GUID_SysMouse, &mysz, NULL);
	SprawdzWynik(wynik, "Tworzenie myszy");
}
void Kontrolery::tworzWejscie() const {
	wynik = DirectInput8Create(
		uchAplikacji,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&wejscie,
		NULL);
	SprawdzWynik(wynik, "Tworzenie wejscia kontrolerow.");
}
void Kontrolery::ustawKlawiatura() const {
	wynik = klawiatura->SetDataFormat(&c_dfDIKeyboard);
	SprawdzWynik(wynik, "Ustawienie formatu danych klawiatury.");
	wynik = klawiatura->SetCooperativeLevel(
		uchOknoGl,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	SprawdzWynik(wynik, "Ustawienie wspolpracy klawiatury z oknem gry.");
	wynik = klawiatura->Acquire();
	SprawdzWynik(wynik, "Rezerwacja klawiatury.");
}
void Kontrolery::ustawMysz() const {
	wynik = mysz->SetDataFormat(&c_dfDIMouse);
	SprawdzWynik(wynik, "Ustawienie formatu danych myszy.");
	wynik = mysz->SetCooperativeLevel(
		uchOknoGl,
		DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	SprawdzWynik(wynik, "Ustawienie wspolpracy myszy z oknem gry.");
	wynik = mysz->Acquire();
	SprawdzWynik(wynik, "Rezerwacja myszy.");
}
void Kontrolery::wezStanKlawiatura() {
	klawiatura->GetDeviceState(sizeof(stanKlawiatura), (LPVOID)&stanKlawiatura);
}
void Kontrolery::wezStanMysz() {
	mysz->GetDeviceState(sizeof(stanMysz), &stanMysz);
}
Kontrolery::Kontrolery(
	HINSTANCE const		uchwyt
	) : klawiatura(NULL), mysz(NULL), obiekt(NULL), uchAplikacji(uchwyt), wejscie(NULL)
	{
	inicjalizuj();
}
Kontrolery::~Kontrolery() {
	logi.piszStart("START", "Niszczenie kontrolerow.");
	if(mysz != NULL) {
		mysz->Unacquire();
		mysz->Release();
		logi.pisz("OK", "Niszczenie myszy.");
	}
	if(klawiatura != NULL) {
		klawiatura->Unacquire();
		klawiatura->Release();
		logi.pisz("OK", "Niszczenie klawiatury.");
	}
	if(wejscie != NULL) {
		wejscie->Release();
		logi.pisz("OK", "Niszczenie wejscia.");
	}
	logi.piszStop("STOP", "Niszczenie kontrolerow.");
}
void Kontrolery::przypnijListaFiz(
	ListaZdarzFiz_* const		lista
	) {
	zdarzeniaFiz = lista;
}
void Kontrolery::tworzZdarzenia() {
	wezStanKlawiatura();
	wezStanMysz();

	if(stanKlawiatura[DIK_A] & 0x80) {
		ZdarzRuchOb* const zdarzenie = new ZdarzRuchOb;
		zdarzenie->obiekt = obiekt;
		zdarzenie->wektorRuch = XMVectorSet(-0.002f, +0.0f, +0.0f, 0.0f);
		zdarzeniaFiz->push_back(zdarzenie);
	}
	if(stanKlawiatura[DIK_D] & 0x80) {
		ZdarzRuchOb* const zdarzenie = new ZdarzRuchOb;
		zdarzenie->obiekt = obiekt;
		zdarzenie->wektorRuch = XMVectorSet(+0.002f, +0.0f, +0.0f, 0.0f);
		zdarzeniaFiz->push_back(zdarzenie);
	}
	if(stanKlawiatura[DIK_W] & 0x80) {
		ZdarzRuchOb* const zdarzenie = new ZdarzRuchOb;
		zdarzenie->obiekt = obiekt;
		zdarzenie->wektorRuch = XMVectorSet(+0.0f, +0.002f, +0.0f, 0.0f);
		zdarzeniaFiz->push_back(zdarzenie);
	}
	if(stanKlawiatura[DIK_S] & 0x80) {
		ZdarzRuchOb* const zdarzenie = new ZdarzRuchOb;
		zdarzenie->obiekt = obiekt;
		zdarzenie->wektorRuch = XMVectorSet(+0.0f, -0.002f, +0.0f, 0.0f);
		zdarzeniaFiz->push_back(zdarzenie);
	}
}
void Kontrolery::ustawObiekt(
	Obiekt3W* const		ob
	) {
	obiekt = ob;
}



