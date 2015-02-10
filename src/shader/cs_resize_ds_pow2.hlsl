cbuffer CB : register(b0) {
	float		width_ratio;
	float		height_ratio;
};
Texture2D<float> ds : register(t0);
RWTexture2D<float> occl_rep : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	uint3 _coord_ds = {_gr.x+1, _gr.y+1, 0.0f};
	uint2 _coord_occl_rep2 = {_coord_ds.x*width_ratio, _coord_ds.y*height_ratio};
	uint2 _coord_occl_rep3 = {(_coord_ds.x+1)*width_ratio, (_coord_ds.y+1)*height_ratio};

	if(_gr.x != 0 && _gr.y != 0) {
		uint2 _coord_occl_rep1 = {
			(_coord_ds.x-1)*width_ratio, (_coord_ds.y-1)*height_ratio
		};
		if(_coord_occl_rep1.x == _coord_occl_rep2.x
			|| _coord_occl_rep1.y == _coord_occl_rep2.y)
			return;
	}

	float4 _texel = {0.0f, 0.0f, 0.0f, 0.0f};
	_texel.x = ds.Load(_coord_ds);
	if(_coord_occl_rep2.x == _coord_occl_rep3.x) {
		_texel.y = ds.Load(_coord_ds, uint2(1, 0));
		if(_coord_occl_rep2.y == _coord_occl_rep3.y) {
			_texel.z = ds.Load(_coord_ds, uint2(0, 1));
			_texel.w = ds.Load(_coord_ds, uint2(1, 1));
		}
	} else if(_coord_occl_rep2.y == _coord_occl_rep3.y) {
		_texel.z = ds.Load(_coord_ds, uint2(0, 1));
	}
	occl_rep[_coord_occl_rep2] = max(max(_texel.x, _texel.y), max(_texel.z, _texel.w));
}




