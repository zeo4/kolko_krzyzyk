#pragma once

#include "logika.h"
#include "fizyka.h"

//void Logika::uwzglWejscie() {
//	BYTE stanKlawiatura[256];
//	DIMOUSESTATE stanMysz;
//	wejscie.wez(stanKlawiatura, &stanMysz);
//
//	if(obiektWybrany != NULL) {
//		if(stanKlawiatura[DIK_W] & 0x80) {
//			obiektWybrany->wezFiz()->zadajRuch(XMVectorSet(+0.0f, +0.015f, +0.0f, +0.0f), 0, 0, 0);
//		}
//		if(stanKlawiatura[DIK_S] & 0x80) {
//			obiektWybrany->wezFiz()->zadajRuch(XMVectorSet(+0.0f, -0.015f, +0.0f, +0.0f), 0, 0, 0);
//		}
//		if(stanKlawiatura[DIK_A] & 0x80) {
//			obiektWybrany->wezFiz()->zadajRuch(XMVectorSet(-0.015f, +0.0f, +0.0f, +0.0f), 0, 0, 0);
//		}
//		if(stanKlawiatura[DIK_D] & 0x80) {
//			obiektWybrany->wezFiz()->zadajRuch(XMVectorSet(+0.015f, +0.0f, +0.0f, +0.0f), 0, 0, 0);
//		}
//	}
//	if(obiektKursor != NULL) {
//		if(stanMysz.lX != 0) {
//			float p = stanMysz.lX * 0.001f;
//			obiektKursor->wezFiz()->zadajRuch(XMVectorSet(p, +0.0f, +0.0f, +0.0f), 0, 0, 0);
//		}
//		if(stanMysz.lY != 0) {
//			float p = -stanMysz.lY * 0.001f;
//			obiektKursor->wezFiz()->zadajRuch(XMVectorSet(+0.0f, p, +0.0f, +0.0f), 0, 0, 0);
//		}
//		if(stanMysz.rgbButtons[0] & 0x80) {
//			swiat.wezObPromien(&obiektWybrany, obiektKursor);
//		}
//	}
//}
Logika::Logika(HINSTANCE const uchwyt) /*: wejscie(uchwyt)*/
	{}
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
void Logika::inic() {
	swiat.inic();
}
void Logika::tworzKolejnaKlatka() {
	//uwzglWejscie();
	//swiat.wykonajFizyka();
	swiat.rysuj();
}

