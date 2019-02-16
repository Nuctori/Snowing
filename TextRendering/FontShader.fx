#include <FontRenderer.hlsli>

// ����ʹ��Ĭ�ϵľ���VS��GS������ʹ�õ�PS
VertexShader SpriteVS = CompileShader(vs_4_0, SpriteVSFunc());
GeometryShader SpriteGS = CompileShader(gs_4_0, SpriteGSFunc());
PixelShader FontPS = CompileShader(ps_4_0, FontPSFunc());

technique11 FontRendering
{
	pass P0
	{
		// ���û��ģʽ
		SetBlendState(NormalAlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		// ���ö�����ɫ����������ɫ���ͼ�����ɫ��
		SetVertexShader(SpriteVS);
		SetPixelShader(FontPS);
		SetGeometryShader(SpriteGS);
	}
};
