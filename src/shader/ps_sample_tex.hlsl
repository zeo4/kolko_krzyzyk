struct Prim {
	float4		vert : SV_Position;
	float2		tex_coord : TEX_COORD;
};

Texture2D<float> tex : register(t0);
SamplerState sampl_state;

float4 main(Prim _pix) : SV_Target {
	return tex.Sample(sampl_state, _pix.tex_coord);
}
