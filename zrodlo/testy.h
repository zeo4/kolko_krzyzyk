#ifndef _TESTY_H_
#define _TESTY_H_

#include "globalne.h"
#include "debug.h"
#include <typeinfo>

class Test_Wsk {
	void		konstruktory();
	void		operatoryPrzypisania();
	void		operatoryPrzyrostu();
	void		operatoryUbytku();
	void		operatoryWyboruWskaznik();
	void		operatoryWyluskania();
public:
	void		uruchom();
};

#endif