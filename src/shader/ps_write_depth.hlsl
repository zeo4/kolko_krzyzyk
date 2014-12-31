struct Primitive {
	float4		vert_coord : SV_POSITION;
};

float4 main(Primitive _pix) : SV_TARGET {
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
