cbuffer bufCoKlatka : register(b0) {
	//float4x4		macSWP[1024];
};

struct WyjscieSW {
	float4		poz : SV_POSITION;
	float2		pozTekstury : TEXCOORD;
};

Texture2D tekstura;
SamplerState stanProbkowania;

WyjscieSW SW(float4 _pozWe : POZYCJA, float2 _wspTekstury : TEKSTURA, float4x4 _mac_swp : SWIAT) {
	WyjscieSW _wyj;
	_wyj.poz = mul(_pozWe, _mac_swp);
	_wyj.pozTekstury = _wspTekstury;
	return _wyj;
}

float4 SP(WyjscieSW _we) : SV_TARGET {
	return tekstura.Sample(stanProbkowania, _we.pozTekstury);
}