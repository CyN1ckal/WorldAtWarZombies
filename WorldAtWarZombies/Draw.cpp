#include "pch.h"

IDirect3DTexture9 *Primitive = NULL;
bool PerfDrawInit = false;
/*
        brief: Copy and pasted world to screen... Need to do this by myself
   still. But  it takes 3d coordinates and makes them into 2d screen coordinates
   for esp drawing. Basically just a bunch of matrix math.
*/
bool Draw::WorldToScreen(const Vector3 pos, Vector2 &screen, float matrix[16],
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

  if (clipCoords.w < 0.1f)
    return false;

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
                          IDirect3DDevice9 *dev) {
  D3DRECT BarRect = {x, y, x + w, y + h};
  dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

/*
        brief: My attempt at drawing a triangle with vertices. It sorta works,
   but then gets like drawn over or something? Need to figure it out a bit more.
*/
LPDIRECT3DVERTEXBUFFER9 Draw::v_buffer;
void Draw::DrawTriangle(IDirect3DDevice9 *dev) {
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

  VOID *pVoid; // a void pointer

  // lock v_buffer and load the vertices into it
  v_buffer->Lock(0, 0, (void **)&pVoid, 0);
  memcpy(pVoid, OurVertices, sizeof(OurVertices));
  v_buffer->Unlock();
}

/*
        brief: Draw a line between 2 sets of screen coordinates
*/
void Draw::DrawLine(float x1, float y1, float x2, float y2, float width,
                    bool antialias, D3DCOLOR color, IDirect3DDevice9 *dev) {
  ID3DXLine *m_Line;

  D3DXCreateLine(dev, &m_Line);
  D3DXVECTOR2 line[] = {D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2)};
  m_Line->SetWidth(width);
  if (antialias)
    m_Line->SetAntialias(1);
  m_Line->Begin();
  m_Line->Draw(line, 2, color);
  m_Line->End();
  m_Line->Release();
}

/*
        brief: Draw a colored healthbar for your local player
*/
bool Draw::DrawHealthBar(IDirect3DDevice9 *dev) {
  if (!Hack::Local_Player->Time || Hack::Local_Player->CurrentHealth < 1)
    return false;

  Vector2 ScreenDimensions = {Hack::RefDef->Width, Hack::RefDef->Height};

  Vector2 CenterScreen = {ScreenDimensions.x / 2, ScreenDimensions.y / 2};

  float PercentageMaxHealth =
      static_cast<float>(Hack::Local_Player->CurrentHealth) /
      static_cast<float>(Hack::Local_Player->MaxHealth);

  int HealthBarWidth = 300;
  int HealthBarHeight = 24;
  int HealthBarOffsetFromBottom = 24;

  Draw::DrawRectPerf(dev, CenterScreen.x - HealthBarWidth / 2,
                     ScreenDimensions.y - HealthBarOffsetFromBottom -
                         HealthBarHeight,
                     HealthBarWidth, HealthBarHeight, D3DCOLOR_RGBA(255, 255, 255, 55));

  int HealthInPixels = floor(PercentageMaxHealth * HealthBarWidth);

  Draw::DrawRectPerf(
      dev, CenterScreen.x - HealthBarWidth / 2,
      ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight,
      HealthInPixels, HealthBarHeight, D3DCOLOR_RGBA(0, 255, 0, 255));

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
                           D3DCOLOR_RGBA(255, 0, 0, 255));

  return true;
}

/*
        brief: Draw the number of zombies alive on your screen
*/
bool Draw::DrawZombieCount(IDirect3DDevice9 *dev) {
  int ZombieCount = Hack::AliveZombieVector.size();

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
                           D3DCOLOR_RGBA(255, 0, 0, 155));
  return true;
}

/*
        brief: Draws tracers from the bottom of the screen to every zombie which
   is alive
*/
RGBA_Struct MyImGui::ZombieTracerColor_RGBA;
bool Draw::DrawZombieTracers(IDirect3DDevice9 *dev, DrawTracerType TracerType) {

  for (int i = 0; i < Hack::AliveZombieVector.size(); i++) {
    Vector2 ScreenCoordinates;
    Vector2 CenterScreen = {Hack::RefDef->Width / 2, Hack::RefDef->Height / 2};

    switch (TracerType) {
    case DrawTracerType::Head:
      if (!Draw::WorldToScreen(Hack::AliveZombieVector[i].HeadPosition,
                               ScreenCoordinates, Hack::pViewMatrix,
                               Hack::RefDef->Width, Hack::RefDef->Height))
        continue;

      Draw::DrawLinePerf(dev, CenterScreen.x, Hack::RefDef->Height,
                         ScreenCoordinates.x, ScreenCoordinates.y,
                         D3DCOLOR_RGBA(MyImGui::ZombieTracerColor_RGBA.r,
                                       MyImGui::ZombieTracerColor_RGBA.g,
                                       MyImGui::ZombieTracerColor_RGBA.b,
                                       MyImGui::ZombieTracerColor_RGBA.a));
      break;
    case DrawTracerType::CenterMass:
      Vector3 CenterMass =
          Hack::EntityStateArray
              ->EntityStateArray[Hack::AliveZombieVector[i].EntStateArrayNumber]
              .position;
      CenterMass.z += 30.0f;
      if (!Draw::WorldToScreen(CenterMass, ScreenCoordinates, Hack::pViewMatrix,
                               Hack::RefDef->Width, Hack::RefDef->Height))
        continue;

      Draw::DrawLinePerf(dev, CenterScreen.x, Hack::RefDef->Height,
                         ScreenCoordinates.x, ScreenCoordinates.y,
                         D3DCOLOR_RGBA(MyImGui::ZombieTracerColor_RGBA.r,
                                       MyImGui::ZombieTracerColor_RGBA.g,
                                       MyImGui::ZombieTracerColor_RGBA.b,
                                       MyImGui::ZombieTracerColor_RGBA.a));
      break;
    case DrawTracerType::Origin:
      if (!Draw::WorldToScreen(Hack::EntityStateArray
                                   ->EntityStateArray[Hack::AliveZombieVector[i]
                                                          .EntStateArrayNumber]
                                   .position,
                               ScreenCoordinates, Hack::pViewMatrix,
                               Hack::RefDef->Width, Hack::RefDef->Height))
        continue;

      Draw::DrawLinePerf(dev, CenterScreen.x, Hack::RefDef->Height,
                         ScreenCoordinates.x, ScreenCoordinates.y,
                         D3DCOLOR_RGBA(MyImGui::ZombieTracerColor_RGBA.r,
                                       MyImGui::ZombieTracerColor_RGBA.g,
                                       MyImGui::ZombieTracerColor_RGBA.b,
                                       MyImGui::ZombieTracerColor_RGBA.a));
      break;
    default:
      return 0;
    }
  }

  return 1;
}

/*
        brief: Draw a tracer from the bottom of the screen to all entities with
   the specified type. Also prints out what the eType is and the entityState
   number
*/
bool Draw::DrawTypeTracers(IDirect3DDevice9 *dev, EntityType eType) {
  EntityStateArray_New *EntityStateArray =
      *(EntityStateArray_New **)(Hack::WaW_BaseAddress +
                                 Offsets::EntityStateArrayppOffset);

  for (int i = 0; i < 1024; i++) {
    if (EntityStateArray->EntityStateArray[i].eType == eType) {
      Vector2 screen = {};
      if (WorldToScreen(EntityStateArray->EntityStateArray[i].position, screen,
                        Hack::pViewMatrix, Hack::RefDef->Width,
                        Hack::RefDef->Height)) {
        Draw::DrawLinePerf(dev, Hack::RefDef->Width / 2, Hack::RefDef->Height,
                           screen.x, screen.y,
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
HRESULT Draw::GenerateTexture(IDirect3DDevice9 *pD3Ddev,
                              IDirect3DTexture9 **ppD3Dtex, DWORD colour32) {
  if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4,
                                    D3DPOOL_MANAGED, ppD3Dtex, NULL)))
    return E_FAIL;

  WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12) |
                  (WORD)(((colour32 >> 20) & 0xF) << 8) |
                  (WORD)(((colour32 >> 12) & 0xF) << 4) |
                  (WORD)(((colour32 >> 4) & 0xF) << 0);
  D3DLOCKED_RECT d3dlr;
  (*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
  WORD *pDst16 = (WORD *)d3dlr.pBits;
  for (int xy = 0; xy < 8 * 8; xy++)
    *pDst16++ = colour16;
  (*ppD3Dtex)->UnlockRect(0);
  return S_OK;
}

/*
    brief: Draw a rectangle on the screen more performantly
    Instead of clearing an entire area of the display buffer, this creates a
    vertex buffers and draws the rectangle on the texture we got from begin
   scene
*/
void Draw::DrawRectPerf(IDirect3DDevice9 *m_pD3Ddev, float x, float y, float w,
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

/*
    brief: This shit broke...
*/
void Draw::DrawTrianglePerf(IDirect3DDevice9 *m_pD3Ddev, float x, float y,
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
void Draw::DrawLinePerf(IDirect3DDevice9 *m_pD3Ddev, float X, float Y, float X2,
                        float Y2, D3DCOLOR Color) {
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

/*
    brief: Draw text at the top left whenever injected
*/
RGBA_Struct MyImGui::MenuThemeColor_RGBA;
void Draw::Watermark() {
  RECT TextLocation = {0, 0, 200, 100};
  Draw::pFont[2]->DrawTextA(NULL, "Zombies Mod", -1, &TextLocation,
                            DT_LEFT | DT_TOP,
                            D3DCOLOR_RGBA(MyImGui::MenuThemeColor_RGBA.r,
                                          MyImGui::MenuThemeColor_RGBA.g,
                                          MyImGui::MenuThemeColor_RGBA.b,
                                          MyImGui::MenuThemeColor_RGBA.a));
}

/*
    brief: Draw text on screen for when infinite ammo is active
*/
bool Draw::InfiniteAmmoText(IDirect3DDevice9 *pD3DDevice) {
  std::string InfiniteAmmoString = "Infinite Ammo On!";

  int StringPixelLength = InfiniteAmmoString.length() * 12;
  int OffsetFromBottom =
      static_cast<int>(static_cast<float>(Hack::RefDef->Height) * 0.35);

  RECT TextLocation = {Hack::RefDef->Width / 2 - StringPixelLength / 2,
                       Hack::RefDef->Height - OffsetFromBottom - 24,
                       Hack::RefDef->Width / 2 + StringPixelLength / 2,
                       Hack::RefDef->Height - OffsetFromBottom};
  pFont[2]->DrawTextA(NULL, InfiniteAmmoString.c_str(), -1, &TextLocation,
                      DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
  return 1;
}

/*
    brief: Populates the pFont array with fonts
*/
bool Draw::CreateFonts(IDirect3DDevice9 *pD3DDevice) {
  D3DXCreateFont(pD3DDevice, 24, 8, FW_NORMAL, 0, 0, DEFAULT_CHARSET,
                 OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH,
                 "Consolas", &Draw::pFont[0]);
  D3DXCreateFont(pD3DDevice, 24, 10, FW_NORMAL, 0, 0, DEFAULT_CHARSET,
                 OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH,
                 "Roboto", &Draw::pFont[1]);
  D3DXCreateFont(pD3DDevice, 24, 10, FW_NORMAL, 0, 0, DEFAULT_CHARSET,
                 OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH,
                 "Inconsolata Expanded", &Draw::pFont[2]);
  return 1;
}

/*
    brief: Draw a line from each zombie's origin to their head
*/
RGBA_Struct MyImGui::ZombieVerticalLineColor_RGBA;
bool Draw::VerticalLineESP(IDirect3DDevice9 *pD3DDevice) {

  for (int i = 0; i < Hack::AliveZombieVector.size(); i++) {
    Vector2 HeadPositionScreenCoordinates;
    if (!Draw::WorldToScreen(Hack::AliveZombieVector[i].HeadPosition,
                             HeadPositionScreenCoordinates, Hack::pViewMatrix,
                             Hack::RefDef->Width, Hack::RefDef->Height))
      continue;

    Vector2 OriginPositionScreenCoordinates;
    if (!Draw::WorldToScreen(Hack::EntityStateArray
                                 ->EntityStateArray[Hack::AliveZombieVector[i]
                                                        .EntStateArrayNumber]
                                 .position,
                             OriginPositionScreenCoordinates, Hack::pViewMatrix,
                             Hack::RefDef->Width, Hack::RefDef->Height))
      continue;

    Draw::DrawLinePerf(pD3DDevice, OriginPositionScreenCoordinates.x,
                       OriginPositionScreenCoordinates.y,
                       HeadPositionScreenCoordinates.x,
                       HeadPositionScreenCoordinates.y,
                       D3DCOLOR_RGBA(MyImGui::ZombieVerticalLineColor_RGBA.r,
                                     MyImGui::ZombieVerticalLineColor_RGBA.g,
                                     MyImGui::ZombieVerticalLineColor_RGBA.b,
                                     MyImGui::ZombieVerticalLineColor_RGBA.a));
  }

  return 1;
}