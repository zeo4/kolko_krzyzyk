#pragma once

#include "globalne.h"
#include "debug.h"
#include "zasobyGraf.h"

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

