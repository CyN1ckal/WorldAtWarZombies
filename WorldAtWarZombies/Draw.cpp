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

void Draw::DrawLine(float x1, float y1, float x2, float y2, float width, bool antialias, D3DCOLOR color, IDirect3DDevice9* dev)
{
    ID3DXLine* m_Line;

    D3DXCreateLine(dev, &m_Line);
    D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
    m_Line->SetWidth(width);
    if (antialias) m_Line->SetAntialias(1);
    m_Line->Begin();
    m_Line->Draw(line, 2, color);
    m_Line->End();
    m_Line->Release();
}