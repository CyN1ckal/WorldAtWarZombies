#pragma once

class Draw
{
public:
	static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	static void DrawTriangle(int x1, int y1, int x2, int y2, int width, D3DCOLOR color, IDirect3DDevice9* dev);

	static LPDIRECT3DVERTEXBUFFER9 v_buffer;
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CustomVertex {
	float x, y, z;
	DWORD color;
};