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

#ifndef _FIZ_
#define _FIZ_
#include "fiz.cpp"
#endif

#ifndef _WEJ_
#define _WEJ_
#include "wejscie.cpp"
#endif

class Logika {
	UINT			adrObKursor;
	UINT			adrObWybrany;
	Fizyka*			fizyka;
	Wejscie*		wejscie;
	void			uwzglWejscie();
public:
					Logika();
	void			dodajObiektSwiat(Obiekt3W* const);
	void			laczFizyka(Fizyka* const);
	void			laczWejscie(Wejscie* const);
	void			tworzKolejnaKlatka();
	
};
void Logika::uwzglWejscie() {
	BYTE stanKlawiatura[256];
	DIMOUSESTATE stanMysz;
	wejscie->wez(stanKlawiatura, &stanMysz);

	if(stanKlawiatura[DIK_W] & 0x80) {
		fizyka->przesunObiekt(adrObWybrany, XMVectorSet(+0.0f, +0.002f, +0.0f, +0.0f));
	}
	if(stanKlawiatura[DIK_S] & 0x80) {
		fizyka->przesunObiekt(adrObWybrany, XMVectorSet(+0.0f, -0.002f, +0.0f, +0.0f));
	}
	if(stanKlawiatura[DIK_A] & 0x80) {
		fizyka->przesunObiekt(adrObWybrany, XMVectorSet(-0.002f, +0.0f, +0.0f, +0.0f));
	}
	if(stanKlawiatura[DIK_D] & 0x80) {
		fizyka->przesunObiekt(adrObWybrany, XMVectorSet(+0.002f, +0.0f, +0.0f, +0.0f));
	}
	if(stanMysz.lX != 0) {
		float p = stanMysz.lX * 0.001f;
		fizyka->przesunObiekt(adrObKursor, XMVectorSet(p, +0.0f, +0.0f, +0.0f));
	}
	if(stanMysz.lY != 0) {
		float p = -stanMysz.lY * 0.001f;
		fizyka->przesunObiekt(adrObKursor, XMVectorSet(+0.0f, p, +0.0f, +0.0f));
	}
	if(stanMysz.rgbButtons[0] & 0x80){
		UINT adr;
		fizyka->wez(&adr, adrObKursor);
		adrObWybrany = adr;
	}
}
Logika::Logika() : adrObKursor(NULL), adrObWybrany(NULL), fizyka(NULL), wejscie(NULL)
	{}
void Logika::dodajObiektSwiat(
	Obiekt3W* const		ob
	) {
	// test
	if(adrObKursor == NULL)
	adrObKursor = (UINT)ob;
	adrObWybrany = (UINT)ob;

	fizyka->dodajObiektSwiat(ob);
}
void Logika::laczFizyka(
	Fizyka* const		f
	) {
	fizyka = f;
}
void Logika::laczWejscie(
	Wejscie* const		w
	) {
	wejscie = w;
}
void Logika::tworzKolejnaKlatka() {
	uwzglWejscie();
	fizyka->tworzKolejnaKlatka();
}



