struct A {float val;};

//Buffer<float> bbox : register(c0);
RWStructuredBuffer<A> test1 : register(u0);
RWStructuredBuffer<A> test2 : register(u1);
RWBuffer<float> rect : register (u2);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
	float a = test1[0].val;
	a = test2[0].val +a-a;
	rect[0] = a;
	//uint _bbox_offset = _gr.x * 8;
	//uint _rect_offset = _gr.x * 16;

	// compute bounding rectangle
	//float _left_f = bbox[_bbox_offset].x;
	//float _right_f = _left_f;
	//float _top_f = bbox[_bbox_offset].y;
	//float _bottom_f = _top_f;
	//float _near_f = bbox[_bbox_offset].z;
	//for(uint _i = 1; _i < 8; ++_i) {
	//	if(bbox[_bbox_offset + _i].x < _left_f) _left_f = bbox[_bbox_offset + _i].x;
	//	else if(bbox[_bbox_offset + _i].x > _right_f) _right_f = bbox[_bbox_offset + _i].x;
	//	if(bbox[_bbox_offset + _i].y < _bottom_f) _bottom_f = bbox[_bbox_offset + _i].y;
	//	else if(bbox[_bbox_offset + _i].y > _top_f) _top_f = bbox[_bbox_offset + _i].y;
	//	if(bbox[_bbox_offset + _i].z < _near_f) _near_f = bbox[_bbox_offset + _i].z;
	//}
}




