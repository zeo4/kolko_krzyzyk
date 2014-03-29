#pragma once

#include "obiektyGra.h"

float Tekst::rozmPiks = 0.01;
UINT Tekst::szerTekstura = 416;
Obiekt3W* Tekst::tworzLitera(
	char const		litera
	) {
	float piksTekstura = 1.0f/szerTekstura;
	Wierzcholek wierzcholki[] = {
		Wierzcholek(
			+0.0f,
			+0.0f,
			+0.0f,
			mapaTekstura.at(litera)[0] * piksTekstura,
			+0.0f
		),
		Wierzcholek(
			mapaTekstura.at(litera)[1] * rozmPiks,
			+0.0f,
			+0.0f,
			(mapaTekstura.at(litera)[0] + mapaTekstura.at(litera)[1]) * piksTekstura,
			+0.0f
		),
		Wierzcholek(
			+0.0f,
			-mapaTekstura.at(litera)[2] * rozmPiks,
			+0.0f,
			mapaTekstura.at(litera)[0] * piksTekstura,
			+1.0f
		),
		Wierzcholek(
			mapaTekstura.at(litera)[1] * rozmPiks,
			-mapaTekstura.at(litera)[2] * rozmPiks,
			+0.0f,
			(mapaTekstura.at(litera)[0] + mapaTekstura.at(litera)[1]) * piksTekstura,
			+1.0f
		)
	};
	DWORD indeksy[] = {
		0, 1, 2,
		2, 1, 3
	};

	return new Obiekt3W(
		wierzcholki, 4,
		indeksy, 6,
		"tekstura\\znaki tekstowe.jpg"
	);
}
Tekst::Tekst(
	) : literyGraf(new ObiektZbior)
	{
	float *a = new float[3];
	float *b = new float[3];
	float *c = new float[3];
	float *d = new float[3];
	float *e = new float[3];
	float *f = new float[3];
	float *g = new float[3];
	float *h = new float[3];
	float *i = new float[3];
	float *j = new float[3];
	float *k = new float[3];
	float *l = new float[3];
	float *m = new float[3];
	float *n = new float[3];
	float *o = new float[3];
	float *p = new float[3];
	float *q = new float[3];
	float *r = new float[3];
	float *s = new float[3];
	float *t = new float[3];
	float *u = new float[3];
	float *v = new float[3];
	float *w = new float[3];
	float *x = new float[3];
	float *y = new float[3];
	float *z = new float[3];
	a[0] = 0; a[1] = 8; a[2] = 14;
	b[0] = a[0] + a[1]; b[1] = 8; b[2] = 14;
	c[0] = b[0] + b[1]; c[1] = 8; c[2] = 14;
	d[0] = c[0] + c[1]; d[1] = 8; d[2] = 14;
	e[0] = d[0] + d[1]; e[1] = 8; e[2] = 14;
	f[0] = e[0] + e[1]; f[1] = 8; f[2] = 14;
	g[0] = f[0] + f[1]; g[1] = 8; g[2] = 14;
	h[0] = g[0] + g[1]; h[1] = 8; h[2] = 14;
	i[0] = h[0] + h[1]; i[1] = 8; i[2] = 14;
	j[0] = i[0] + i[1]; j[1] = 8; j[2] = 14;
	k[0] = j[0] + j[1]; k[1] = 8; k[2] = 14;
	l[0] = k[0] + k[1]; l[1] = 8; l[2] = 14;
	m[0] = l[0] + l[1]; m[1] = 8; m[2] = 14;
	n[0] = m[0] + m[1]; n[1] = 8; n[2] = 14;
	o[0] = n[0] + n[1]; o[1] = 8; o[2] = 14;
	p[0] = o[0] + o[1]; p[1] = 8; p[2] = 14;
	q[0] = p[0] + p[1]; q[1] = 8; q[2] = 14;
	r[0] = q[0] + q[1]; r[1] = 8; r[2] = 14;
	s[0] = r[0] + r[1]; s[1] = 8; s[2] = 14;
	t[0] = s[0] + s[1]; t[1] = 8; t[2] = 14;
	u[0] = t[0] + t[1]; u[1] = 8; u[2] = 14;
	v[0] = u[0] + u[1]; v[1] = 8; v[2] = 14;
	w[0] = v[0] + v[1]; w[1] = 8; w[2] = 14;
	x[0] = w[0] + w[1]; x[1] = 8; x[2] = 14;
	y[0] = x[0] + x[1]; y[1] = 8; y[2] = 14;
	z[0] = y[0] + y[1]; z[1] = 8; z[2] = 14;
	mapaTekstura.insert(ParaLiteraPole_('a', a));
	mapaTekstura.insert(ParaLiteraPole_('b', b));
	mapaTekstura.insert(ParaLiteraPole_('c', c));
	mapaTekstura.insert(ParaLiteraPole_('d', d));
	mapaTekstura.insert(ParaLiteraPole_('e', e));
	mapaTekstura.insert(ParaLiteraPole_('f', f));
	mapaTekstura.insert(ParaLiteraPole_('g', g));
	mapaTekstura.insert(ParaLiteraPole_('h', h));
	mapaTekstura.insert(ParaLiteraPole_('i', i));
	mapaTekstura.insert(ParaLiteraPole_('j', j));
	mapaTekstura.insert(ParaLiteraPole_('k', k));
	mapaTekstura.insert(ParaLiteraPole_('l', l));
	mapaTekstura.insert(ParaLiteraPole_('m', m));
	mapaTekstura.insert(ParaLiteraPole_('n', n));
	mapaTekstura.insert(ParaLiteraPole_('o', o));
	mapaTekstura.insert(ParaLiteraPole_('p', p));
	mapaTekstura.insert(ParaLiteraPole_('q', q));
	mapaTekstura.insert(ParaLiteraPole_('r', r));
	mapaTekstura.insert(ParaLiteraPole_('s', s));
	mapaTekstura.insert(ParaLiteraPole_('t', t));
	mapaTekstura.insert(ParaLiteraPole_('u', u));
	mapaTekstura.insert(ParaLiteraPole_('v', v));
	mapaTekstura.insert(ParaLiteraPole_('w', w));
	mapaTekstura.insert(ParaLiteraPole_('x', x));
	mapaTekstura.insert(ParaLiteraPole_('y', y));
	mapaTekstura.insert(ParaLiteraPole_('z', z));
}
void Tekst::pisz(
	string const		str
	) {
	Obiekt3W* ob;
	float poz = +0.0f;
	for(int i = 0; i < str.size(); ++i) {
		ob = tworzLitera(str.at(i));
		ob->wykonajRuch(XMVectorSet(poz, +0.0f, +0.0f, +0.0f));
		literyGraf->dodaj(ob);
		poz += mapaTekstura.at(str.at(i))[1] * rozmPiks;
	}
	literyGraf->wykonajRuch(XMVectorSet(+0.0f, +0.0f, +1.0f, +0.0f));
}
IObiekt* const Tekst::wezObiektGraf(
	) const {
	return literyGraf;
}


