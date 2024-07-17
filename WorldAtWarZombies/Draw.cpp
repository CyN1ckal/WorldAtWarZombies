#include "pch.h"

// Forward Declaration from static class
LPDIRECT3DVERTEXBUFFER9 Draw::v_buffer;

DWORD OriginalFVF = 0;

void Draw::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void Draw::DrawTriangle(int x1, int y1, int x2, int y2, int width, D3DCOLOR color, IDirect3DDevice9* dev)
{
    Sleep(50);

    if (dev->GetFVF(&OriginalFVF) != D3D_OK)
    {
        printf("Something went wrong!\n");
        return;
    }
   
    dev->GetStreamSource(0, 0, 0, 0);

    CustomVertex OurVertices[] =
    {
        {320.0f, 50.0f, 1.0f, D3DCOLOR_XRGB(0, 0, 255),},
        {520.0f, 400.0f, 1.0f, D3DCOLOR_XRGB(0, 255, 0),},
        {120.0f, 400.0f, 1.0f, D3DCOLOR_XRGB(255, 0, 0),},
    };

    dev->CreateVertexBuffer(3 * sizeof(CustomVertex), 0, CUSTOMFVF, D3DPOOL_MANAGED, &v_buffer, NULL);
    
    void* pVoid;
    v_buffer->Lock(0, 0, &pVoid, 0);

    memcpy(pVoid, OurVertices, sizeof(OurVertices));
    v_buffer->Unlock();

    dev->SetFVF(CUSTOMFVF);

    dev->SetStreamSource(0, v_buffer, 0, sizeof(CustomVertex));

    dev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    dev->SetFVF(OriginalFVF);
}
