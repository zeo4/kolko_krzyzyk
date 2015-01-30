float4 main(float4 _pix : SV_Position) : SV_TARGET {
	return float4(_pix.z, _pix.z, _pix.z, 1.0f);
}
