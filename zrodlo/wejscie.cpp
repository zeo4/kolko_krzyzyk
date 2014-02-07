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

class Wejscie {
	IDirectInputDevice8*		klawiatura;
	IDirectInputDevice8*		mysz;
	HINSTANCE					uchAplikacji;
	IDirectInput8*				wejscie;
	void						inicjalizuj();
	void						tworzKlawiatura();
	void						tworzMysz();
	void						tworzWejscie() const;
	void						ustawKlawiatura() const;
	void						ustawMysz() const;
public:
								Wejscie(HINSTANCE const);
								~Wejscie();
	void						wez(BYTE* const, DIMOUSESTATE* const);
};
void Wejscie::inicjalizuj() {
	tworzWejscie();
	tworzKlawiatura();
	tworzMysz();
	ustawKlawiatura();
	ustawMysz();
}
void Wejscie::tworzKlawiatura() {
	wynik = wejscie->CreateDevice(GUID_SysKeyboard, &klawiatura, NULL);
	SprawdzWynik(wynik, "Tworzenie klawiatury.");
}
void Wejscie::tworzMysz() {
	wynik = wejscie->CreateDevice(GUID_SysMouse, &mysz, NULL);
	SprawdzWynik(wynik, "Tworzenie myszy");
}
void Wejscie::tworzWejscie() const {
	wynik = DirectInput8Create(
		uchAplikacji,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&wejscie,
		NULL);
	SprawdzWynik(wynik, "Tworzenie wejscia kontrolerow.");
}
void Wejscie::ustawKlawiatura() const {
	wynik = klawiatura->SetDataFormat(&c_dfDIKeyboard);
	SprawdzWynik(wynik, "Ustawienie formatu danych klawiatury.");
	wynik = klawiatura->SetCooperativeLevel(
		uchOknoGl,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	SprawdzWynik(wynik, "Ustawienie wspolpracy klawiatury z oknem gry.");
	wynik = klawiatura->Acquire();
	SprawdzWynik(wynik, "Rezerwacja klawiatury.");
}
void Wejscie::ustawMysz() const {
	wynik = mysz->SetDataFormat(&c_dfDIMouse);
	SprawdzWynik(wynik, "Ustawienie formatu danych myszy.");
	wynik = mysz->SetCooperativeLevel(
		uchOknoGl,
		DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	SprawdzWynik(wynik, "Ustawienie wspolpracy myszy z oknem gry.");
	wynik = mysz->Acquire();
	SprawdzWynik(wynik, "Rezerwacja myszy.");
}
Wejscie::Wejscie(
	HINSTANCE const		uchwyt
	) : klawiatura(NULL), mysz(NULL), uchAplikacji(uchwyt), wejscie(NULL)
	{
	inicjalizuj();
}
Wejscie::~Wejscie() {
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
void Wejscie::wez(
	BYTE* const				stanKlawiatura,
	DIMOUSESTATE* const		stanMysz
	) {
	klawiatura->GetDeviceState(sizeof(BYTE)*256, (LPVOID)stanKlawiatura);
	mysz->GetDeviceState(sizeof(DIMOUSESTATE), stanMysz);
}


