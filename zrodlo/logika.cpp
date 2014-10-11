#pragma once
#include <logika.h>
// -------------------------------------------------------
void Logika::inicScena() {
	swiat.tworz_ob(MOD_TROJKAT, TEKS_TROJKAT);
	gra_dane.uch_wybr = 0x80000000;
}
void Logika::obsluz_wej(MSG const& _wiad) {
	if(_wiad.message == WM_LBUTTONDOWN) {
		POINT _pkt;
		GetCursorPos(&_pkt);
		ScreenToClient(uch_okno, &_pkt);
		ZadWybOb z = {WYB_OB, _pkt.x, _pkt.y};
		zad.wstaw_kon((uint8_t*)&z, sizeof(z));
	}
	if(_wiad.message == WM_KEYDOWN) {
		if(gra_dane.uch_wybr != 0x80000000) {
			if(_wiad.wParam == 0x41) {
				// lewy
				ZadUstawOb z = {USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(-0.5f, 0.0f, 0.5f)};
				zad.wstaw_kon((uint8_t*)&z, sizeof(z));
			}
			if(_wiad.wParam == 0x44) {
				// prawy
				ZadUstawOb z = {USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(0.5f, 0.0f, 0.5f)};
				zad.wstaw_kon((uint8_t*)&z, sizeof(z));
			}
			if(_wiad.wParam == 0x53) {
				// dół
				ZadUstawOb z = {USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(0.0f, -0.5f, 0.5f)};
				zad.wstaw_kon((uint8_t*)&z, sizeof(z));
			}
			if(_wiad.wParam == 0x57) {
				// góra
				ZadUstawOb z = {USTAW_OB, gra_dane.uch_wybr, XMFLOAT3(0.0f, 0.5f, 0.5f)};
				zad.wstaw_kon((uint8_t*)&z, sizeof(z));
			}
		}
	}
}
void Logika::rys_klatka() {
	swiat.rys_klatka();
	wez_wyn();
	uint32_t* _mapa_wier = 0,* _mapa_el = 0;
	zad.defrag_licz(_mapa_wier, _mapa_el, 1);
	zad.defrag_wyk(_mapa_wier, _mapa_el);
	wyn.defrag_licz(_mapa_wier, _mapa_el, 1);
	wyn.defrag_wyk(_mapa_wier, _mapa_el);
	free(_mapa_wier);
	free(_mapa_el);
}
void Logika::wez_wyn() {
	for(uint32_t _i = 0; _i < wyn.wez_il_wier(); ++_i) {
		if(wyn.wez_wier(_i) == wyn.pusty) continue;

		switch(*(KodZad*)wyn[_i]) {
		case WYB_OB: {
			WynWybOb w = *(WynWybOb*)wyn[_i];
			gra_dane.uch_wybr = w.uch_ob;
			break;
		}
		}

		wyn.usun(_i);
	}
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



