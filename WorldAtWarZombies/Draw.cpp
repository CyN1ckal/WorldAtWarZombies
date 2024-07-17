#include "pch.h"


void Draw::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void Draw::DrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color, IDirect3DDevice9* dev)
{

}