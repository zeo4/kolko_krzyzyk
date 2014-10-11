﻿#pragma once

#include <globalne.h>
#include <wek.h>
#include <uch.h>
#include <zasoby.h>
#include <zadania.h>
class Grafika;
// -------------------------------------------------------
class Fizyka : protected Zadania, protected Kamera, protected Obiekty, protected ParFiz {
public:
	void				wyk_zad();
public:
	void				inic();
	void				wykonaj();
	void				tworz_ob();
	uint32_t			wyb_ob() const;
protected:
	void				uwzgl_v();
	void				uwzgl_poz();
	void				licz_prom_klik(XMVECTOR&, XMVECTOR&,
							uint32_t const&, uint32_t const&) const;
	float				licz_prom_ob(XMVECTOR const&, XMVECTOR const&,
							uint32_t const&) const;
	float				licz_prom_troj(XMVECTOR const&, XMVECTOR const&,
							CXMVECTOR const&, CXMVECTOR const&,
							CXMVECTOR const&) const;
	inline XMMATRIX		licz_swiat_odwr(uint32_t const&) const;
};
XMMATRIX Fizyka::licz_swiat_odwr(uint32_t const& _nr_ob) const {
	return XMMatrixInverse(
		&XMVectorSet(0,0,0,0),
		XMLoadFloat4x4(&par_fiz.mac_swiat[_nr_ob])
	);
}
// -------------------------------------------------------

// ---------------------------------------------
//class Fizyka {
//public:
//private:
//};
//// ---------------------------------------------
//
//class Obiekt3w;
//
//class IFizyka {
//	friend class Fizyka3WKolizyjny;
//	friend class Fizyka3WNiekolizyjny;
//	friend class ObiektZbior;
//	friend class FizykaZbior;
//protected:
//	IObiekt* const			obiekt;
//							IFizyka(IObiekt* const);
//	virtual					~IFizyka();
//	void					liczSwiatBezkol();
//	void					liczSwiatParam();
//	bool					sprawdzKulaKula(FXMVECTOR const, float const,
//								FXMVECTOR const, FXMVECTOR const, float const,
//								CXMVECTOR const, float* const) const;
//	bool					sprawdzKolizjaPudelkoPudelko(FXMVECTOR const,
//								FXMVECTOR const, FXMVECTOR const, CXMVECTOR const
//								) const;
//	bool					sprawdzKolizjaTrojkatTrojkat(FXMVECTOR const,
//								FXMVECTOR const, FXMVECTOR const, CXMVECTOR const,
//								CXMVECTOR const, CXMVECTOR const) const;
//	bool					sprawdzPromienKula(XMVECTOR const, XMVECTOR const,
//								XMVECTOR const, float const, float* const) const;
//	bool					wezKolizjaOdcinekTrojkat(float* const, FXMVECTOR const,
//								FXMVECTOR const, FXMVECTOR const, CXMVECTOR const,
//								CXMVECTOR const) const;
//	virtual void			wezKolizjePromien(MapaFloatObiekt_* const, XMVECTOR const,
//								XMVECTOR const) const = 0;
//	bool					wezKolizjaPromienTrojkat(float* const, FXMVECTOR const,
//								FXMVECTOR const, FXMVECTOR const, CXMVECTOR const,
//								CXMVECTOR const) const;
//public:
//	virtual void			liczCzasKolizja() = 0;
//	virtual void			liczSwiatyBezkol() = 0;
//	virtual void			liczSwiatyParam() = 0;
//	virtual void			wezObiekty3W(ZbiorOb3w_* const) const = 0;
//	XMVECTOR				wezPrzes() const;
//	void					wezPrzesunMacierz(XMFLOAT4X4 const, XMVECTOR* const
//								) const;
//	virtual void			zadajRuch(XMVECTOR const, float const, float const,
//								float const) = 0;
//};
//
//// #############################################
//
//class Fizyka3w : public IFizyka {
//	friend class Obiekt3w;
//protected:
//	Obiekt3w* const		obiekt;
//	void				usunSwiatPkt(XMVECTOR* const) const;
//	void				usunSwiatWektor(XMVECTOR* const) const;
//	void				wezKolizjePromien(MapaFloatObiekt_* const, XMVECTOR const,
//							XMVECTOR const) const;
//public:
//						Fizyka3w(Obiekt3w* const);
//						~Fizyka3w();
//	void				liczSwiatyBezkol();
//	void				liczSwiatyParam();
//	void				wezBrylaGraniczna(XMVECTOR* const, XMVECTOR* const,
//							float* const) const;
//	void				wezObiekty3W(ZbiorOb3w_* const) const;
//	void				zadajRuch(XMVECTOR const, float const, float const,
//							float const);
//};
//
//class Fizyka3wKoliz : public virtual Fizyka3w {
//public:
//						Fizyka3wKoliz();
//	virtual void		liczCzasKolizja();
//};
//
//class Fizyka3wNiekoliz : public virtual Fizyka3w {
//public:
//						Fizyka3wNiekoliz();
//	virtual void		liczCzasKolizja();
//};
//
//// ---------------------------------------------
//
//class FizykaLitera : public virtual Fizyka3w, public Fizyka3wNiekoliz {
//	friend class Obiekt3w;
//protected:
//	FizykaLitera(Obiekt3w* const);
//};
//
//class FizykaPostac : public virtual Fizyka3w, public Fizyka3wKoliz {
//	friend class Obiekt3w;
//protected:
//	FizykaPostac(Obiekt3w* const);
//};
//
//// #############################################
//
//class FizykaZbior : public IFizyka {
//	friend class ObiektZbior;
//protected:
//	ObiektZbior* const		obiekt;
//public:
//							FizykaZbior(ObiektZbior* const);
//	virtual					~FizykaZbior();
//	virtual void			liczCzasKolizja();
//	void					liczSwiatyBezkol();
//	void					liczSwiatyParam();
//	void					wezObiekty3W(ZbiorOb3w_* const) const;
//	void					zadajRuch(
//								XMVECTOR const, float const, float const, float const
//							);
//};
//
//class FizykaZbiorNiezalezny : public virtual FizykaZbior {
//};
//
//class FizykaZbiorZalezny : public virtual FizykaZbior {
//protected:
//	void		wezKolizjePromien(
//					MapaFloatObiekt_* const,
//					XMVECTOR const, XMVECTOR const
//				) const;
//public:
//				FizykaZbiorZalezny();
//};
//// ---------------------------------------------
//
//class FizykaTekst : public virtual FizykaZbior, public FizykaZbiorZalezny {
//	friend class ObiektZbior;
//protected:
//	FizykaTekst(ObiektZbior* const);
//public:
//};
//
//// #############################################
