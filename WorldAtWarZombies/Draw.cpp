#include "pch.h"
IDirect3DTexture9* Primitive = NULL;
bool PerfDrawInit = false;
/*
        brief: Copy and pasted world to screen... Need to do this by myself
   still. But  it takes 3d coordinates and makes them into 2d screen coordinates
   for esp drawing. Basically just a bunch of matrix math.
*/
bool Draw::WorldToScreen(const Vector3 pos, Vector2& screen, float matrix[16],
                         const int windowWidth, const int windowHeight) {
  Vector4 clipCoords = {};
  clipCoords.x =
      pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
  clipCoords.y =
      pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
  clipCoords.z =
      pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
  clipCoords.w =
      pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

  if (clipCoords.w < 0.1f) return false;

  Vector3 NDC = {};
  NDC.x = clipCoords.x / clipCoords.w;
  NDC.y = clipCoords.y / clipCoords.w;
  NDC.z = clipCoords.z / clipCoords.w;

  screen.x = ((float)windowWidth / static_cast<float>(2) * NDC.x) +
             (NDC.x + (float)windowWidth / static_cast<float>(2));
  screen.y = -((float)windowHeight / static_cast<float>(2) * NDC.y) +
             (NDC.y + (float)windowHeight / static_cast<float>(2));
  return true;
}

/*
        brief: Clears a rectangular area of your monitor with 1 single color
*/
void Draw::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color,
                          IDirect3DDevice9* dev) {
  D3DRECT BarRect = {x, y, x + w, y + h};
  dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

/*
        brief: My attempt at drawing a triangle with vertices. It sorta works,
   but then gets like drawn over or something? Need to figure it out a bit more.
*/
LPDIRECT3DVERTEXBUFFER9 Draw::v_buffer;
void Draw::DrawTriangle(IDirect3DDevice9* dev) {
  CUSTOMVERTEX OurVertices[] = {
      {
          400.0f,
          62.5f,
          0.5f,
          1.0f,
          D3DCOLOR_XRGB(0, 0, 255),
      },
      {
          650.0f,
          500.0f,
          0.5f,
          1.0f,
          D3DCOLOR_XRGB(0, 255, 0),
      },
      {
          150.0f,
          500.0f,
          0.5f,
          1.0f,
          D3DCOLOR_XRGB(255, 0, 0),
      },
  };

  // create a vertex buffer interface called v_buffer
  dev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, CUSTOMFVF,
                          D3DPOOL_MANAGED, &v_buffer, NULL);

  VOID* pVoid;  // a void pointer

  // lock v_buffer and load the vertices into it
  v_buffer->Lock(0, 0, (void**)&pVoid, 0);
  memcpy(pVoid, OurVertices, sizeof(OurVertices));
  v_buffer->Unlock();
}

/*
        brief: Draw a line between 2 sets of screen coordinates
*/
void Draw::DrawLine(float x1, float y1, float x2, float y2, float width,
                    bool antialias, D3DCOLOR color, IDirect3DDevice9* dev) {
  ID3DXLine* m_Line;

  D3DXCreateLine(dev, &m_Line);
  D3DXVECTOR2 line[] = {D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2)};
  m_Line->SetWidth(width);
  if (antialias) m_Line->SetAntialias(1);
  m_Line->Begin();
  m_Line->Draw(line, 2, color);
  m_Line->End();
  m_Line->Release();
}

/*
        brief: Draw a colored healthbar for your local player
*/
bool Draw::DrawHealthBar(IDirect3DDevice9* dev) {
  if (!Hack::Local_Player->Time || Hack::Local_Player->Flags == 8 ||
      Hack::Local_Player->Flags == 2)
    return false;

  Vector2 ScreenDimensions = {Hack::RefDef->Width, Hack::RefDef->Height};

  Vector2 CenterScreen = {ScreenDimensions.x / 2, ScreenDimensions.y / 2};

  float PercentageMaxHealth =
      static_cast<float>(Hack::Local_Player->CurrentHealth) /
      static_cast<float>(Hack::Local_Player->MaxHealth);
  int HealthBarWidth = 300;
  int HealthBarHeight = 24;
  int HealthBarOffsetFromBottom = 24;

  D3DRECT BackgroundRect = {
      CenterScreen.x - HealthBarWidth / 2,
      ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight,
      CenterScreen.x + HealthBarWidth / 2,
      ScreenDimensions.y - HealthBarOffsetFromBottom};
  dev->Clear(1, &BackgroundRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET,
             D3DCOLOR_ARGB(255, 125, 125, 125), 0, 0);

  int HealthInPixels = floor(PercentageMaxHealth * HealthBarWidth);

  D3DRECT HealtRect = {
      CenterScreen.x - HealthBarWidth / 2,
      ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight,
      CenterScreen.x - HealthBarWidth / 2 + HealthInPixels,
      ScreenDimensions.y - HealthBarOffsetFromBottom};
  dev->Clear(1, &HealtRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET,
             D3DCOLOR_ARGB(255, 0, 255, 0), 0, 0);

  RECT rect;
  SetRect(&rect, CenterScreen.x - HealthBarWidth / 2,
          ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight,
          CenterScreen.x + HealthBarWidth / 2,
          ScreenDimensions.y - HealthBarOffsetFromBottom);
  std::string HealthBarString =
      std::format("{} / {}", Hack::Local_Player->CurrentHealth,
                  Hack::Local_Player->MaxHealth);
  Draw::pFont[0]->DrawText(NULL, HealthBarString.c_str(), -1, &rect,
                           DT_CENTER | DT_VCENTER,
                           D3DCOLOR_ARGB(255, 255, 0, 0));

  return true;
}

/*
        brief: Draw the number of zombies alive on your screen
*/
bool Draw::DrawZombieCount(IDirect3DDevice9* dev) {
  int ZombieCount = Hack::GetNumZombies();

  std::string ZombieCountString = std::to_string(ZombieCount) + " zombies left";

  // pFont[0] has a width of 8 pixels and a height of 24
  int StringPixelWidth = ZombieCountString.length() * 8;
  int OffsetFromBottom = 0;
  RECT rect;

  // L T R B
  SetRect(&rect, (Hack::RefDef->Width / 2) - (StringPixelWidth / 2),
          Hack::RefDef->Height - 24 - OffsetFromBottom,
          (Hack::RefDef->Width / 2) + (StringPixelWidth / 2),
          Hack::RefDef->Height - OffsetFromBottom);

  Draw::pFont[0]->DrawText(NULL, ZombieCountString.c_str(), -1, &rect,
                           DT_CENTER | DT_VCENTER,
                           D3DCOLOR_ARGB(255, 255, 0, 0));
  return true;
}

/*
        brief: Draws tracers from the bottom of the screen to every zombie which
   is alive
*/
bool Draw::DrawZombieTracers(IDirect3DDevice9* dev) {
  EntityStateArray_New* EntityStateArray =
      *(EntityStateArray_New**)(Hack::WaW_BaseAddress + 0xbe1c0);

  for (int i = 0; i < 1024; i++) {
    if (EntityStateArray->EntityStateArray[i].eType == EntityType::Zombie &&
        EntityStateArray->EntityStateArray[i].CurrentHealth > 0) {
      Vector2 screen = {};
      if (WorldToScreen(EntityStateArray->EntityStateArray[i].position, screen,
                        Hack::pViewMatrix, Hack::RefDef->Width,
                        Hack::RefDef->Height)) {
        if (EntityStateArray->EntityStateArray[i].CurrentHealth < 152) {
          Draw::DrawLinePerf(dev, Hack::RefDef->Width / 2, Hack::RefDef->Height,
                             screen.x, screen.y, 3,
                             D3DCOLOR_ARGB(255, 255, 66, 66));
        } else {
          Draw::DrawLinePerf(dev, Hack::RefDef->Width / 2, Hack::RefDef->Height,
                             screen.x, screen.y, 3,
                             D3DCOLOR_ARGB(255, 255, 255, 255));
        }
      }
    }
  }

  return 1;
}

/*
        brief: Draw a tracer from the bottom of the screen to all entities with
   the specified type. Also prints out what the eType is and the entityState
   number
*/
bool Draw::DrawTypeTracers(IDirect3DDevice9* dev, EntityType eType) {
  EntityStateArray_New* EntityStateArray =
      *(EntityStateArray_New**)(Hack::WaW_BaseAddress + 0xbe1c0);

  for (int i = 0; i < 1024; i++) {
    if (EntityStateArray->EntityStateArray[i].eType == eType) {
      Vector2 screen = {};
      if (WorldToScreen(EntityStateArray->EntityStateArray[i].position, screen,
                        Hack::pViewMatrix, Hack::RefDef->Width,
                        Hack::RefDef->Height)) {
        Draw::DrawLinePerf(dev, Hack::RefDef->Width / 2, Hack::RefDef->Height,
                           screen.x, screen.y, 3,
                           D3DCOLOR_ARGB(125, 255, 255, 255));
        RECT rect;
        SetRect(&rect, screen.x, screen.y, screen.x + 120, screen.y + 100);
        std::string TypeString =
            std::format("Type:{}-ID:{}",
                        (int)EntityStateArray->EntityStateArray[i].eType, i);
        pFont[0]->DrawTextA(NULL, TypeString.c_str(), -1, &rect,
                            DT_LEFT | DT_TOP,
                            D3DCOLOR_ARGB(255, 125, 125, 125));
      }
    }
  }

  return 1;
}

/*
    brief:
    Copy and pasted from
   https://www.unknowncheats.me/forum/direct3d/60883-draw-drawprimtiveup-d3d9.html
*/
HRESULT Draw::GenerateTexture(IDirect3DDevice9* pD3Ddev,
                              IDirect3DTexture9** ppD3Dtex, DWORD colour32) {
  if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4,
                                    D3DPOOL_MANAGED, ppD3Dtex, NULL)))
    return E_FAIL;

  WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12) |
                  (WORD)(((colour32 >> 20) & 0xF) << 8) |
                  (WORD)(((colour32 >> 12) & 0xF) << 4) |
                  (WORD)(((colour32 >> 4) & 0xF) << 0);
  D3DLOCKED_RECT d3dlr;
  (*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
  WORD* pDst16 = (WORD*)d3dlr.pBits;
  for (int xy = 0; xy < 8 * 8; xy++) *pDst16++ = colour16;
  (*ppD3Dtex)->UnlockRect(0);
  return S_OK;
}

/*
    brief: Draw a rectangle on the screen more performantly
    Instead of clearing an entire area of the display buffer, this creates a
    vertex buffers and draws the rectangle on the texture we got from begin
   scene
*/
void Draw::DrawRectPerf(IDirect3DDevice9* m_pD3Ddev, float x, float y, float w,
                        float h, D3DCOLOR Color) {
  D3DTLVERTEX qV[4] = {{(float)x, (float)(y + h), 0.0f, 1.0f, Color},
                       {(float)x, (float)y, 0.0f, 1.0f, Color},
                       {(float)(x + w), (float)(y + h), 0.0f, 1.0f, Color},
                       {(float)(x + w), (float)y, 0.0f, 1.0f, Color}};
  const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  m_pD3Ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  m_pD3Ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  m_pD3Ddev->SetFVF(D3DFVF_TL);
  (SIZE) m_pD3Ddev->SetTexture(0, Primitive);
  (SIZE) m_pD3Ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV,
                                    sizeof(D3DTLVERTEX));
}

void Draw::DrawTrianglePerf(IDirect3DDevice9* m_pD3Ddev, float x, float y,
                            float z, D3DCOLOR Color) {
  D3DTLVERTEX qV[4] = {{(float)0.0f, (float)0.0f, 0.0f, 1.0f, Color},
                       {(float)0.0f, (float)100.0f, 0.0f, 1.0f, Color},
                       {(float)100.0f, (float)100.0f, 0.0f, 1.0f, Color},
                       {(float)200.0f, (float)200.0f, 0.0f, 1.0f, Color}};
  const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  m_pD3Ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  m_pD3Ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  m_pD3Ddev->SetFVF(D3DFVF_TL);
  (SIZE) m_pD3Ddev->SetTexture(0, Primitive);
  (SIZE) m_pD3Ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV,
                                    sizeof(D3DTLVERTEX));
}

/*
    brief: Draws line connecting 2 vertices more performantly.
    Instead of using the direct X line object, this creates a vertex buffer and
    renders it onto the texture we got with begin scene
*/
void Draw::DrawLinePerf(IDirect3DDevice9* m_pD3Ddev, float X, float Y, float X2,
                        float Y2, float Width, D3DCOLOR Color) {
  D3DTLVERTEX qV[2] = {
      {(float)X, (float)Y, 0.0f, 1.0f, Color},
      {(float)X2, (float)Y2, 0.0f, 1.0f, Color},
  };
  const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  m_pD3Ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
  m_pD3Ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  m_pD3Ddev->SetFVF(D3DFVF_TL);
  m_pD3Ddev->SetTexture(0, Primitive);
  m_pD3Ddev->DrawPrimitiveUP(D3DPT_LINELIST, 2, qV, sizeof(D3DTLVERTEX));
}
