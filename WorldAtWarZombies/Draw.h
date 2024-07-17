#pragma once

class Draw
{
public:
	static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	static void DrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color, IDirect3DDevice9* dev);
};