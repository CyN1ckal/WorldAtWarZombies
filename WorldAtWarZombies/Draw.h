#pragma once

class Draw
{
public:
	static bool GetScreenDimensions(IDirect3DDevice9* dev, Vector2* a);


	static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	static void DrawTriangle(D3DCOLOR color, IDirect3DDevice9* dev);
	static void DrawLine(float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color, IDirect3DDevice9* dev);
	static bool DrawHealthBar(IDirect3DDevice9* dev, Vector2 ScreenDimensions);
	static bool DrawZombieCount(IDirect3DDevice9* dev, Vector2 ScreenDimensions);

	static ID3DXFont* pFont[1];

	static LPDIRECT3DVERTEXBUFFER9 v_buffer;
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CustomVertex {
	float x, y, z;
	DWORD color;
};