Buffer<float4> rect : register(c0);

float4 main(uint _inst_id : SV_InstanceID, uint _vert_id : SV_VertexID) : SV_Position {
	return rect[_inst_id * 5 + _vert_id];
}

