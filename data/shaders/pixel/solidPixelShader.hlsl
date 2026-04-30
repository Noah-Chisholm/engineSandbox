cbuffer RenderableConstants : register(b0)
{
	row_major float4x4 projectedModelViewMatrix;
	float4 materialBaseColor;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PSMain(PSInput input) : SV_TARGET
{
    return materialBaseColor;
}