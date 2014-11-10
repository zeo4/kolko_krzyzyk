cbuffer bufCoKlatka : register(b0) {
	//float4x4		macSWP[1024];
};

struct WyjscieSW {
	float4		poz : SV_POSITION;
	float2		poz_teks : TTTTT;
};

Texture2D teks;
SamplerState stanProbkowania;

WyjscieSW SW(float4 _poz_we : POZYCJA, float2 _wsp_teks : TEKSTURA, float4x4 _mac_swp : SWIAT) {
	WyjscieSW _wyj;
	_wyj.poz = mul(_poz_we, _mac_swp);
	_wyj.poz_teks = _wsp_teks;
	return _wyj;
}

float4 SP_GL(WyjscieSW _we) : SV_TARGET {
	//return teks.Sample(stanProbkowania, _we.poz_teks);
	return float4(_we.poz.z, _we.poz.z, _we.poz.z, 0.0f);
}

void SP_GLEB(WyjscieSW _we) {}

