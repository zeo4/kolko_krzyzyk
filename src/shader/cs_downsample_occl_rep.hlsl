Texture2D<float> mip_previous : register(t0);
RWTexture2D<float> mip : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	uint _width, _height, _dummy;
	mip_previous.GetDimensions(0, _width, _height, _dummy);
	uint3 _coord = uint3(_gr.x, _gr.y, 0) * 2;

	float4 _texel = {0.0f, 0.0f, 0.0f, 0.0f};
	_texel.x = mip_previous.Load(_coord);
	if(_width > 1) {
		_texel.y = mip_previous.Load(_coord, uint2(1, 0));
		if(_height > 1) {
			_texel.z = mip_previous.Load(_coord, uint2(0, 1));
			_texel.w = mip_previous.Load(_coord, uint2(1, 1));
		}
	} else if(_height > 1) {
		_texel.z = mip_previous.Load(_coord, uint2(0, 1));
	}

	float a = max(max(_texel.x, _texel.y), max(_texel.z, _texel.w));
	mip[uint2(_gr.x, _gr.y)] = a;
}




