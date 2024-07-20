#pragma once

class Draw
{
public:
	static bool WorldToScreen(const Vector3 pos, Vector2& screen, float matrix[16], const int windowWidth, const int windowHeight);

	static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	static void DrawTriangle(D3DCOLOR color, IDirect3DDevice9* dev);
	static void DrawLine(float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color, IDirect3DDevice9* dev);
	static bool DrawHealthBar(IDirect3DDevice9* dev);
	static bool DrawZombieCount(IDirect3DDevice9* dev);
	static bool DrawZombieTracers(IDirect3DDevice9* dev);
	static bool DrawTypeTracers(IDirect3DDevice9* dev, int eType);

	static ID3DXFont* pFont[1];

	static LPDIRECT3DVERTEXBUFFER9 v_buffer;
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CustomVertex {
	float x, y, z;
	DWORD color;
};