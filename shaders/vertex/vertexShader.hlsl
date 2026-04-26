cbuffer RenderableConstants : register(b0)
{
	row_major float4x4 projectedModelViewMatrix;
};

struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
	PSInput output;

	float4 localPosition = float4(input.position, 1.0f);

	output.position = mul(localPosition, projectedModelViewMatrix);
	output.color = input.color;

	return output;
}