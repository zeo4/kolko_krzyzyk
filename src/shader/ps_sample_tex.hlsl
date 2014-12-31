struct Prim {
	float4		vert_coord : SV_POSITION;
	float2		tex_coord : TTTTT;
};

Texture2D tex : register(t0);
SamplerState sampl_state;

float4 main(Prim _pix) : SV_TARGET {
	// return tex.Sample(sampl_state, _pix.tex_coord);
	float4 _r = tex.Sample(sampl_state, _pix.tex_coord);
	if(_r.x > 0 || _r.y > 0 || _r.z > 0 || _r.w > 0) return float4(1.0f, 1.0f, 1.0f, 0.0f);
	else return float4(0.2f, 0.2f, 0.2f, 0.0f);
}
