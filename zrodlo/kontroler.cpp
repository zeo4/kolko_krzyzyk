#ifndef _GLOBALNE_
#define _GLOBALNE_
#include "globalne.cpp"
#endif

#ifndef _DEBUG_
#define _DEBUG_
#include "debug.cpp"
#endif

class Kontroler {
	HINSTANCE					uchAplikacji;
	IDirectInput8*				wejscie;
	IDirectInputDevice8*		klawiatura;
	IDirectInputDevice8*		mysz;
	BYTE						stanKlawiatura[256];
	DIMOUSESTATE				stanMysz;
	void						tworzWejscie() const;
	void						tworzKlawiatura();
	void						tworzMysz();
	void						ustawKlawiatura() const;
	void						ustawMysz() const;
	void						inicjalizuj();
public:
								Kontroler(HINSTANCE const);
	void						wezStanKlawiatura();
	void						wezStanMysz();
};
void Kontroler::tworzWejscie() const {
	wynik = DirectInput8Create(
		uchAplikacji,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&wejscie,
		NULL);
	SprawdzWynik(wynik, "Tworzenie wejscia kontrolerow.");
}
void Kontroler::tworzKlawiatura() {
	wynik = wejscie->CreateDevice(GUID_SysKeyboard, &klawiatura, NULL);
	SprawdzWynik(wynik, "Tworzenie klawiatury.");
}
void Kontroler::tworzMysz() {
	wynik = wejscie->CreateDevice(GUID_SysMouse, &mysz, NULL);
}
void Kontroler::ustawKlawiatura() const {
	wynik = klawiatura->SetDataFormat(&c_dfDIKeyboard);
	SprawdzWynik(wynik, "Ustawienie formatu danych klawiatury.");
	wynik = klawiatura->SetCooperativeLevel(
		uchOknoGl,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	SprawdzWynik(wynik, "Ustawienie wspolpracy klawiatury z oknem gry.");
}
void Kontroler::ustawMysz() const {
	wynik = mysz->SetDataFormat(&c_dfDIMouse);
	SprawdzWynik(wynik, "Ustawienie formatu danych myszy.");
	wynik = mysz->SetCooperativeLevel(
		uchOknoGl,
		DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	SprawdzWynik(wynik, "Ustawienie wspolpracy myszy z oknem gry.");
}
void Kontroler::inicjalizuj() {
	tworzWejscie();
	tworzKlawiatura();
	tworzMysz();
	ustawKlawiatura();
	ustawMysz();
}
Kontroler::Kontroler(
	HINSTANCE const uchwyt
	) : uchAplikacji(uchwyt), wejscie(NULL), klawiatura(NULL), mysz(NULL)
	{}
void Kontroler::wezStanKlawiatura() {
	klawiatura->Acquire();
	klawiatura->GetDeviceState(sizeof(klawiatura), &stanKlawiatura);
	klawiatura->Unacquire();
}
void Kontroler::wezStanMysz() {
	mysz->Acquire();
	mysz->GetDeviceState(sizeof(mysz), &stanMysz);
	mysz->Unacquire();
}


