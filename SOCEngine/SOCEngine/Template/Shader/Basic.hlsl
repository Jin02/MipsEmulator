
cbuffer Transform : register( b0 )
{
	matrix world;
	matrix view;
	matrix proj;
};

struct VS_INPUT
{
	float4 pos 		: POSITION;
};

struct PS_INPUT
{
	float4 pos 		: SV_POSITION;
};

 PS_INPUT VS ( VS_INPUT input )
{
	PS_INPUT ps;

	ps.pos = mul( input.pos, world );
	ps.pos = mul( ps.pos, view);
	ps.pos = mul( ps.pos, proj);

    return ps;
}

float4 PS( PS_INPUT input ) : SV_Target
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}