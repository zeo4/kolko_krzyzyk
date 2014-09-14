#pragma once

#include<string>
#include<wek.h>
#include<fstream>
#include<sstream>

using std::string;
// -------------------------------------------------------
class TestPlik {
public:
								TestPlik();
								~TestPlik();
	inline static void			pomin();
	static std::ofstream		plik;
	static uint32_t				poz;
private:
	inline static void			otworz();
	static bool					otwarty;
};
TestPlik::TestPlik() {
	otworz();
}
TestPlik::~TestPlik() {
	if(otwarty) plik.close();
	otwarty = false;
}
void TestPlik::otworz() {
	if(!otwarty) {
		if(poz == 0) plik.open("logtest.txt", std::ios::trunc);
		else plik.open("logtest.txt", std::ios::ate | std::ios::in | std::ios::out);
		plik.seekp(poz);
		otwarty = true;
	}
}
void TestPlik::pomin() {
	plik.seekp(poz);
}
std::ofstream TestPlik::plik;
uint32_t TestPlik::poz = 0;
bool TestPlik::otwarty = false;
// -------------------------------------------------------
class TestPlikKoncz : public TestPlik {
protected:
	inline void		zglos();
};
void TestPlikKoncz::zglos() {
	plik.close();
	assert(0);
}
// -------------------------------------------------------
class TestPlikKont : public TestPlik {
protected:
	inline void		zglos();
};
void TestPlikKont::zglos() {
	poz = plik.tellp();
}
// -------------------------------------------------------
template<class A, class B, class P>
class TestLab : public P {
public:
					TestLab(A const&, B const&);
	inline void		operator==(std::ostream&);
	inline void		operator!=(std::ostream&);
	inline void		operator<(std::ostream&);
	inline void		operator<=(std::ostream&);
	inline void		operator>(std::ostream&);
	inline void		operator>=(std::ostream&);
private:
	A				wartA;
	B				wartB;
};
template<class A, class B, class P>
TestLab<A,B,P>::TestLab(A const& _a, B const& _b)
	: wartA(_a),
	wartB(_b) {}
template<class A, class B, class P>
void TestLab<A,B,P>::operator==(std::ostream&) {
	if(wartA != wartB) zglos();
	else pomin();
}
template<class A, class B, class P>
void TestLab<A,B,P>::operator!=(std::ostream&) {
	if(wartA == wartB) zglos();
	else pomin();
}
template<class A, class B, class P>
void TestLab<A,B,P>::operator<(std::ostream&) {
	if(wartA >= wartB) zglos();
	else pomin();
}
template<class A, class B, class P>
void TestLab<A,B,P>::operator<=(std::ostream&) {
	if(wartA > wartB) zglos();
	else pomin();
}
template<class A, class B, class P>
void TestLab<A,B,P>::operator>(std::ostream&) {
	if(wartA <= wartB) zglos();
	else pomin();
}
template<class A, class B, class P>
void TestLab<A,B,P>::operator>=(std::ostream&) {
	if(wartA < wartB) zglos();
	else pomin();
}
// -------------------------------------------------------
class TestUpewnij : public TestPlik {
public:
	template<class A, class B>
	inline TestLab<A,B,TestPlikKoncz>		tworz_lab(A const&, B const&) const;
};
template<class A, class B>
TestLab<A,B,TestPlikKoncz> TestUpewnij::tworz_lab(A const& _a, B const& _b) const {
	return TestLab<A,B,TestPlikKoncz>(_a, _b);
}
// -------------------------------------------------------
class TestZglos : public TestPlik {
public:
	template<class A, class B>
	inline TestLab<A,B,TestPlikKont>		tworz_lab(A const&, B const&) const;
};
template<class A, class B>
TestLab<A,B,TestPlikKont> TestZglos::tworz_lab(A const& _a, B const& _b) const {
	return TestLab<A,B,TestPlikKont>(_a, _b);
}
// -------------------------------------------------------
#define NAW __FILE__ << "(" << __LINE__ << "): "
#define UPEWNIJ_R(a, b) TestUpewnij().tworz_lab(a, b) == TestUpewnij().plik\
	<< NAW << "(" << a << "==" << b << ") "
#define UPEWNIJ_NR(a, b) TestUpewnij().tworz_lab(a, b) != TestUpewnij().plik\
	<< NAW << "(" << a << "!=" << b << ") "
#define UPEWNIJ_M(a, b) TestUpewnij().tworz_lab(a, b) < TestUpewnij().plik\
	<< NAW << "(" << a << "<" << b << ") "
#define UPEWNIJ_MR(a, b) TestUpewnij().tworz_lab(a, b) <= TestUpewnij().plik\
	<< NAW << "(" << a << "<=" << b << ") "
#define UPEWNIJ_W(a, b) TestUpewnij().tworz_lab(a, b) > TestUpewnij().plik\
	<< NAW << "(" << a << ">" << b << ") "
#define UPEWNIJ_WR(a, b) TestUpewnij().tworz_lab(a, b) >= TestUpewnij().plik\
	<< NAW << "(" << a << ">=" << b << ") "
#define ZGLOS_R(a, b) TestZglos().tworz_lab(a, b) == TestZglos().plik\
	<< NAW << "(" << a << "==" << b << ") "
#define ZGLOS_NR(a, b) TestZglos().tworz_lab(a, b) != TestZglos().plik\
	<< NAW << "(" << a << "!=" << b << ") "
#define ZGLOS_M(a, b) TestZglos().tworz_lab(a, b) < TestZglos().plik\
	<< NAW << "(" << a << "<" << b << ") "
#define ZGLOS_MR(a, b) TestZglos().tworz_lab(a, b) <= TestZglos().plik\
	<< NAW << "(" << a << "<=" << b << ") "
#define ZGLOS_W(a, b) TestZglos().tworz_lab(a, b) > TestZglos().plik\
	<< NAW << "(" << a << ">" << b << ") "
#define ZGLOS_WR(a, b) TestZglos().tworz_lab(a, b) >= TestZglos().plik\
	<< NAW << "(" << a << ">=" << b << ") "
// -------------------------------------------------------


