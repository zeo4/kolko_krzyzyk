struct A {float val;};
RWStructuredBuffer<A> test : register(u1);

float4 main(float4 _pix_loc : SV_Position) : SV_Target {
	float a = test[0].val/10;
	return float4(a, a, a, a);
}

