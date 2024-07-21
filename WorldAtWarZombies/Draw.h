#pragma once

/*
        brief: The Draw class includes variables and functions related to
   drawing with DirectX.
*/
class Draw {
 public:
  static bool WorldToScreen(const Vector3 pos, Vector2& screen,
                            float matrix[16], const int windowWidth,
                            const int windowHeight);

  static void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color,
                             IDirect3DDevice9* dev);
  static void DrawTriangle(IDirect3DDevice9* dev);
  static void DrawLine(float x1, float y1, float x2, float y2, float width,
                       bool antialias, D3DCOLOR color, IDirect3DDevice9* dev);
  static bool DrawHealthBar(IDirect3DDevice9* dev);
  static bool DrawZombieCount(IDirect3DDevice9* dev);
  static bool DrawZombieTracers(IDirect3DDevice9* dev);
  static bool DrawTypeTracers(IDirect3DDevice9* dev, EntityType eType);

  // This is an array of fonts which can grow as necessary. Just 1 for now.
  static ID3DXFont* pFont[1];

  // This is for my attempt at DrawTriangle
  static LPDIRECT3DVERTEXBUFFER9 v_buffer;

  static HRESULT GenerateTexture(IDirect3DDevice9* pD3Ddev,
                                 IDirect3DTexture9** ppD3Dtex, DWORD colour32);
  static void DrawRectPerf(IDirect3DDevice9* m_pD3Ddev, float x, float y,
                           float w, float h, D3DCOLOR Color);
  static void DrawLinePerf(IDirect3DDevice9* m_pD3Ddev, float X, float Y,
                           float X2, float Y2, float Width, D3DCOLOR Color);
  static void DrawTrianglePerf(IDirect3DDevice9* m_pD3Ddev, float x, float y,
                               float z, D3DCOLOR Color);
};

/*
        note: These are both for my DrawTriangle function. The #define is just a
   shortcut for my vertex type, and the struct is the custom vertex
*/
struct CUSTOMVERTEX {
  FLOAT X, Y, Z, RHW;
  DWORD COLOR;
};
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

/*
    brief: Vertex for the performant drawings
*/
struct D3DTLVERTEX {
  float x, y, z, rhw;
  DWORD color;
};