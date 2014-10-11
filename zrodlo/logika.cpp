#pragma once
#include <logika.h>
// -------------------------------------------------------
void Logika::inicScena() {
	swiat.tworz_ob(MOD_TROJKAT, TEKS_TROJKAT);
	dane_gra.uch_wybr = 0x80000000;
}
void Logika::obsluz_wej(MSG const& _wiad) {
	if(_wiad.message == WM_LBUTTONDOWN) {
		dane_gra.uch_wybr = swiat.fiz->wyb_ob();
	}
	if(_wiad.message == WM_KEYDOWN) {
		if(_wiad.wParam == 0x41) {
			// lewy
			ZadUstawOb z = {USTAW_OB, 0, XMFLOAT3(-0.5f, 0.0f, 0.5f)};
			zad.wstaw_kon((uint8_t*)&z, sizeof(z));
		}
		if(_wiad.wParam == 0x44) {
			// prawy
			ZadUstawOb z = {USTAW_OB, 0, XMFLOAT3(0.5f, 0.0f, 0.5f)};
			zad.wstaw_kon((uint8_t*)&z, sizeof(z));
		}
		if(_wiad.wParam == 0x53) {
			// dół
			ZadUstawOb z = {USTAW_OB, 0, XMFLOAT3(0.0f, -0.5f, 0.5f)};
			zad.wstaw_kon((uint8_t*)&z, sizeof(z));
		}
		if(_wiad.wParam == 0x57) {
			// góra
			ZadUstawOb z = {USTAW_OB, 0, XMFLOAT3(0.0f, 0.5f, 0.5f)};
			zad.wstaw_kon((uint8_t*)&z, sizeof(z));
		}
	}
}
void Logika::rys_klatka() {
	swiat.rys_klatka();
}
// -------------------------------------------------------

//void Logika::inicScena() {
//	try{
//	obiektKursor = swiat.tworzObiektKursor();
//	obiektWybrany = swiat.tworzObiektRycerz();
//	swiat.tworzObiektSmok();
//	Tekst t;
//	//t.pisz("fps: 60, to i tak duzo.");
//	t.pisz("fpsabcd:dsfafadsfsdfdsf");
//	swiat.dodaj(t.wezObiektGraf());
//	Obiekt3w::test = t.wezObiektGraf();
//	}
//	catch(Wyjatek wyj){
//	ObslugaWyjatek(wyj);
//	}
//}



