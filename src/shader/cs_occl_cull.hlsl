Texture2D<float> ds : register(t0);
Buffer<float3> bbox : register(c0);

struct f4x4 {
	float4x4		mtx;
};
StructuredBuffer<f4x4> wvp : register(u2);

RWBuffer<bool> is_occluder : register(u0);
cbuffer scr_size : register(b0) {
	uint		width;
	uint		height;
}

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	uint _bbox_no = _gr.x * 8;

	// compute bounding rectangle
	float3 _scr_vert = mul(bbox[_bbox_no], wvp[_gr.x].mtx);
	float _left_f = _scr_vert.x;
	float _right_f = _left_f;
	float _top_f = _scr_vert.y;
	float _bottom_f = _top_f;
	float _z = _scr_vert.z;
	for(uint _i = 1; _i < 8; ++_i) {
		_scr_vert = mul(bbox[_bbox_no + _i], wvp[_gr.x].mtx);
		if(_scr_vert.x < _left_f) _left_f = _scr_vert.x;
		else if(_scr_vert.x > _right_f) _right_f = _scr_vert.x;
		if(_scr_vert.y < _bottom_f) _bottom_f = _scr_vert.y;
		else if(_scr_vert.y > _top_f) _top_f = _scr_vert.y;
		if(_scr_vert.z < _z) _z = _scr_vert.z;
	}

	// bounding rectangle to pixels
	uint _left_i = (_left_f + 1) / 2 * width;
	uint _right_i = (_right_f + 1) / 2 * width;
	uint _top_i = (1 - _top_f) / 2 * height;
	uint _bottom_i = (1 - _bottom_f) / 2 * height;

	// compute mip level
	//check what log2(int) returns;
	uint _mip;
	uint _mip_x = log2(_right_i - _left_i + 1);
	uint _mip_y = log2(_top_i - _bottom_i + 1);
	if(_mip_x > _mip_y) _mip = _mip_x;
	else _mip = _mip_y;

	// depth test
}

//struct BufferStruct { uint4 color; };
//RWStructuredBuffer<BufferStruct> g_OutBuff;

//[numthreads(4, 4, 1)]
//void main( uint3 threadIDInGroup : SV_GroupThreadID, uint3 groupID : SV_GroupID ) {
//	float4 color = 255 * float4(
//		(float)threadIDInGroup.x / THREAD_GROUP_SIZE_X,
//		(float)threadIDInGroup.y / THREAD_GROUP_SIZE_Y,
//		0, 1
//	);
//	int buffIndex = (groupID.y * THREAD_GROUP_SIZE_Y + threadIDInGroup.y) * THREAD_GROUPS_X * THREAD_GROUP_SIZE_X + (groupID.x * THREAD_GROUP_SIZE_X + threadIDInGroup.x);
//	g_OutBuff[buffIndex].color = color;
//}


