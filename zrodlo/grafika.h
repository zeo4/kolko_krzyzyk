#pragma once
#include <globalne.h>
#include <stdint.h>
#include <wek.h>
#include <fizyka.h>
#include <uch.h>
// -------------------------------------------------------
class Grafika : protected Zadania, protected ZasGraf, protected Kamera, protected ParGraf, protected ParFiz {
public:
	void					wyk_zad();
protected:
	void					rys_klatka();
	void					defrag();
	void					tworz_mod(uint32_t const&);
	void					tworz_teks(uint32_t const&);
	char const*const		wez_teks_sciezka(uint32_t const&) const;
public:
	void					usun_ob(uint32_t const&);
};
// -------------------------------------------------------


