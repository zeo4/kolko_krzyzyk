#ifndef _TESTY_C_
#define _TESTY_C_

#include "testy.h"

void Test_Wsk::konstruktory() {
	logi.piszStart("--->", "Konstruktory.");
	
	// konstruktor domyślny
	Wsk<int> w4;
	if(w4.wskaznik != NULL) {
		logi.pisz("!", "Konstruktor domyslny.");
	}
	// konstruktor ze wskaznika do wartosci
	int i5 = 5;
	Wsk<int> w5(&i5);
	if(w5.wskaznik != &i5) {
		logi.pisz("!", "Konstruktor ze wskaznika do wartosci");
	}
	// konstruktor kopiujący z wartości
	int i1 = 1;
	Wsk<int> w1 = &i1;
	if(w1.wskaznik != &i1) {
		logi.pisz("!", "Konstruktor kopiujacy z wartosci.");
	}
	// konstruktor kopiujący ze wskaźnika do wartosci
	int i2 = 2;
	int* wi2 = &i2;
	Wsk<int> w2 = wi2;
	if(w2.wskaznik != wi2) {
		logi.pisz("!", "Konstruktor kopiujacy ze wskaznika do wartosci.");
	}
	// konstruktor kopiujący z Wsk<>
	int i3 = 3;
	Wsk<int> w3a = &i3;
	Wsk<int> w3b = w3a;
	if(w3b.wskaznik != w3a.wskaznik) {
		logi.pisz("!", "Konstruktor kopiujacy z Wsk<>.");
	}

	logi.piszStop("<---", "Konstruktory.");
}
void Test_Wsk::operatoryPrzypisania() {
	logi.piszStart("--->", "Operatory przypisania.");
	
	// przypisanie z wartosci
	int i1 = 1;
	Wsk<int> w1;
	w1 = &i1;
	if(w1.wskaznik != &i1) {
		logi.pisz("!", "Przypisanie z wartosci.");
	}
	// przypisanie ze wskaźnika do wartosci
	int i2 = 2;
	int* wi2 = &i2;
	Wsk<int> w2;
	w2 = wi2;
	if(w2.wskaznik != wi2) {
		logi.pisz("!", "Przypisanie ze wskaznika do wartosci.");
	}
	// przypisanie z Wsk<>
	int i3 = 3;
	Wsk<int> w3a = &i3;
	Wsk<int> w3b;
	w3b = w3a;
	if(w3b.wskaznik != w3a.wskaznik) {
		logi.pisz("!", "Przypisanie z Wsk<>.");
	}

	logi.piszStop("<---", "Operatory przypisania.");
}
void Test_Wsk::operatoryPrzyrostu() {
	logi.piszStart("--->", "Operatory przyrostu.");

	// przyrost wcześniejszy
	Wsk<int> w1; // tutaj ustawiony na NULL
	if((UINT)(++w1).wskaznik != 4 || (UINT)w1.wskaznik != 4) {
		logi.pisz("!", "Przyrost wczesniejszy.");
	}
	// przyrost późniejszy
	Wsk<int> w2; // tutaj ustawiony na NULL
	if((UINT)(w2++).wskaznik != 0 || (UINT)w2.wskaznik != 4) {
		logi.pisz("!", "Przyrost pozniejszy.");
	}

	logi.piszStop("<---", "Operatory przyrostu.");
}
void Test_Wsk::operatoryUbytku() {
	logi.piszStart("--->", "Operatory ubytku.");

	// ubytek wcześniejszy
	Wsk<int> w1; // tutaj ustawiony na NULL
	if((int)(--w1).wskaznik != -4 || (int)w1.wskaznik != -4) {
		logi.pisz("!", "Ubytek wczesniejszy.");
	}
	// ubytek późniejszy
	Wsk<int> w2; // tutaj ustawiony na NULL
	if((int)(w2--).wskaznik != 0 || (int)w2.wskaznik != -4) {
		logi.pisz("!", "Ubytek pozniejszy.");
	}

	logi.piszStop("<---", "Operatory ubytku.");
}
void Test_Wsk::operatoryWyboruWskaznik() {
	logi.piszStart("--->", "Operatory wyboru przez wskaznik.");
	
	class A {
	public:
		int el;
	};

	// wybór elementu przez wskaźnik
	A a1;
	a1.el = 1;
	Wsk<A> w1 = &a1;
	if(w1->el != 1) {
		logi.pisz("!", "Wybor elementu przez wskaznik.");
	}
	// wybór elementu przez stały wskaźnik
	A a2;
	a2.el = 2;
	Wsk<A> const w2 = &a2;
	if(w2->el != 2) {
		logi.pisz("!", "Wybor elementu przez staly wskaznik.");
	}
	// zapis do elementu przez wskaźnik
	A a3;
	a3.el = 3;
	Wsk<A> w3 = &a3;
	w3->el = 33;
	if(a3.el != 33) {
		logi.pisz("!", "Zapis do elementu przez wskaznik.");
	}

	logi.piszStop("<---", "Operatory wyboru przez wskaznik.");
}
void Test_Wsk::operatoryWyluskania() {
	logi.piszStart("--->", "Operatory wyluskania.");

	// wyluskanie referencji do wartości
	int i1 = 1;
	Wsk<int> w1(&i1);
	if(*w1 != 1) {
		logi.pisz("!", "Wyluskanie referencji do wartosci.");
	}
	// przypisanie nowej wartości do wyłuskanej referencji
	int i2 = 2;
	Wsk<int> w2(&i2);
	*w2 = 22;
	if(*w2 != 22) {
		logi.pisz("!", "Przypisanie nowej wartosci do wyluskanej referencji.");
	}

	logi.piszStop("<---", "Operatory wyluskania.");
}
void Test_Wsk::uruchom() {
	logi.piszStart("--->", "Test_Wsk.");

	konstruktory();
	operatoryWyluskania();
	operatoryPrzypisania();
	operatoryWyboruWskaznik();
	operatoryPrzyrostu();
	operatoryUbytku();
	
	logi.piszStop("<---", "Test_Wsk.");
}

#endif