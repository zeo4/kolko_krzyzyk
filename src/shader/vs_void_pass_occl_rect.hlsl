Buffer<float4> rect : register(t0);

float4 main(uint _inst_id : SV_InstanceID, uint _vert_id : SV_VertexID) : SV_Position {
	return rect[_inst_id * 4 + _vert_id];
}
