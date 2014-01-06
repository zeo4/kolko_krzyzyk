cbuffer bufCoObiekt {
	float4x4		macSWP;
};

struct WyjscieSW {
	float4		poz : SV_POSITION;
	float2		pozTekstury : TEXCOORD;
};

Texture2D tekstura;
SamplerState stanProbkowania;

WyjscieSW SW(float4 pozWe : POZYCJA, float2 wspTekstury : TEKSTURA) {
	WyjscieSW wyj;
	wyj.poz = mul(pozWe, macSWP);
	wyj.pozTekstury = wspTekstury;
	return wyj;
}

float4 SP(WyjscieSW we) : SV_TARGET {
	return tekstura.Sample(stanProbkowania, we.pozTekstury);
}