struct A {float val;};

Buffer<float> bbox : register(c0);
RWStructuredBuffer<A> test1 : register(u2);
RWStructuredBuffer<A> test2 : register(u0);
RWBuffer<float> rect : register (u1);

[numthreads(1, 1, 1)]
void main(uint3 _gr : SV_GroupID) {
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
	float a = test1[1].val + bbox[0]-bbox[0];
	//float b = test1[_gr.x + 1];
	//float c = test1[_gr.x + 2];
	a = test2[1].val +a-a;
	//b = test2[_gr.x + 1] +b-b;
	//c = test2[_gr.x + 2] +c-c;
	rect[0] = a;
	//rect[_gr.x + 1] = b;
	//rect[_gr.x + 2] = c;
	//test1[_gr.x] = +a-a;
	//test1[_gr.x + 1] = +b-b;
	//test1[_gr.x + 2] = +c-c;
}




