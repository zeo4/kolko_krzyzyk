Buffer<bool> is_occluder : register(c0);
Buffer<float4> world : register(c1);

float4 main(float4 _in : VERT_COORD) : SV_POSITION {
	return float4(0, 0, 0, 0);
}


