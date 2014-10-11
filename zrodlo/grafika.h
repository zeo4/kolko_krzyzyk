#pragma once
#include <globalne.h>
#include <stdint.h>
#include <wek.h>
#include <fizyka.h>
#include <uch.h>
// -------------------------------------------------------
class Grafika : protected Zadania, protected ZasGraf, protected Kamera, protected Obiekty, protected ParFiz {
public:
	void					wykonaj();
	void					rys_klatka();
	void					defrag(uint32_t*&);
	uint32_t				tworz_ob(uint32_t const&, uint32_t const&);
	void					usun_ob(uint32_t const&);
protected:
	void					rys_ob(uint32_t const&);
	void					aktual_co_ob(uint32_t const& _nr);
	void					uwzgl_wid();
	void					uwzgl_proj();
	void					tworz_mod(uint32_t const&);
	void					tworz_teks(uint32_t const&);
	char const*const		wez_teks_sciezka(uint32_t const&) const;
};
// -------------------------------------------------------


