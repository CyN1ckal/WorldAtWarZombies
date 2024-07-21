#include "pch.h"

/*
	brief: Copy and pasted world to screen... Need to do this by myself still. But  it takes 3d coordinates and makes them into 2d screen coordinates for esp drawing.
			Basically just a bunch of matrix math.
*/
bool Draw::WorldToScreen(const Vector3 pos, Vector2& screen, float matrix[16], const int windowWidth, const int windowHeight)
{
	Vector4 clipCoords = {};
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
	clipCoords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
	clipCoords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
	clipCoords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vector3 NDC = {};
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = ((float)windowWidth / static_cast<float>(2) * NDC.x) + (NDC.x + (float)windowWidth / static_cast<float>(2));
	screen.y = -((float)windowHeight / static_cast<float>(2) * NDC.y) + (NDC.y + (float)windowHeight / static_cast<float>(2));
	return true;
}

/*
	brief: Fills a rectangular area of your monitor with 1 single color
*/
void Draw::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

/*
	brief: My attempt at drawing a triangle with vertices. It sorta works, but then gets like drawn over or something? Need to figure it out a bit more.
*/
LPDIRECT3DVERTEXBUFFER9 Draw::v_buffer;
void Draw::DrawTriangle(D3DCOLOR color, IDirect3DDevice9* dev)
{

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
}

/*
	brief: Draw a line between 2 sets of screen coordinates
*/
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

/*
	brief: Draw a colored healthbar for your local player
*/
bool Draw::DrawHealthBar(IDirect3DDevice9* dev)
{

	if (!Hack::Local_Player->Time || Hack::Local_Player->Flags == 8 || Hack::Local_Player->Flags == 2) return false;

	Vector2 ScreenDimensions = { Hack::RefDef->Width,Hack::RefDef->Height };

	Vector2 CenterScreen = { ScreenDimensions.x / 2,ScreenDimensions.y / 2 };

	float PercentageMaxHealth = static_cast<float>(Hack::Local_Player->CurrentHealth) / static_cast<float>(Hack::Local_Player->MaxHealth);
	int HealthBarWidth = 300;
	int HealthBarHeight = 24;
	int HealthBarOffsetFromBottom = 24;

	D3DRECT BackgroundRect = { CenterScreen.x - HealthBarWidth / 2,
						ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight,
						CenterScreen.x + HealthBarWidth / 2,
						ScreenDimensions.y - HealthBarOffsetFromBottom };
	dev->Clear(1, &BackgroundRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 125, 125, 125), 0, 0);

	int HealthInPixels = floor(PercentageMaxHealth * HealthBarWidth);

	D3DRECT HealtRect = { CenterScreen.x - HealthBarWidth / 2,
						ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight,
						CenterScreen.x - HealthBarWidth / 2 + HealthInPixels,
						ScreenDimensions.y - HealthBarOffsetFromBottom };
	dev->Clear(1, &HealtRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 255, 0), 0, 0);


	RECT rect;
	SetRect(&rect, CenterScreen.x - HealthBarWidth / 2, ScreenDimensions.y - HealthBarOffsetFromBottom - HealthBarHeight, CenterScreen.x + HealthBarWidth / 2, ScreenDimensions.y - HealthBarOffsetFromBottom);
	std::string HealthBarString = std::format("{} / {}", Hack::Local_Player->CurrentHealth, Hack::Local_Player->MaxHealth);
	Draw::pFont[0]->DrawText(NULL, HealthBarString.c_str(), -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 0, 0));

	return true;
}

/*
	brief: Draw the number of zombies alive on your screen
*/
bool Draw::DrawZombieCount(IDirect3DDevice9* dev)
{
	int ZombieCount = Hack::GetNumZombies();

	std::string ZombieCountString = std::to_string(ZombieCount) + " zombies left";

	// pFont[0] has a width of 8 pixels and a height of 24
	int StringPixelWidth = ZombieCountString.length() * 8;
	int OffsetFromBottom = 0;
	RECT rect;

	// L T R B
	SetRect(&rect,
		(Hack::RefDef->Width / 2) - (StringPixelWidth / 2),
		Hack::RefDef->Height - 24 - OffsetFromBottom,
		(Hack::RefDef->Width / 2) + (StringPixelWidth / 2),
		Hack::RefDef->Height - OffsetFromBottom);

	Draw::pFont[0]->DrawText(NULL, ZombieCountString.c_str(), -1, &rect, DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
	return true;
}

/*
	brief: Draws tracers from the bottom of the screen to every zombie which is alive
*/
bool Draw::DrawZombieTracers(IDirect3DDevice9* dev)
{

	EntityStateArray_New* EntityStateArray = *(EntityStateArray_New**)(Hack::WaW_BaseAddress + 0xbe1c0);

	for (int i = 0; i < 1024; i++)
	{
		if (EntityStateArray->EntityStateArray[i].eType == EntityType::Zombie && EntityStateArray->EntityStateArray[i].CurrentHealth > 0)
		{
			Vector2 screen = {};
			if (WorldToScreen(EntityStateArray->EntityStateArray[i].position, screen, Hack::pViewMatrix, Hack::RefDef->Width, Hack::RefDef->Height))
			{
				Draw::DrawLine(Hack::RefDef->Width / 2, Hack::RefDef->Height, screen.x, screen.y, 2, false, D3DCOLOR_ARGB(255, 182, 3, 252), dev);
			}
		}
	}

	return 1;
}

/*
	brief: Draw a tracer from the bottom of the screen to all entities with the specified type. Also prints out what the eType is and the entityState number
*/
bool Draw::DrawTypeTracers(IDirect3DDevice9* dev, EntityType eType)
{

	EntityStateArray_New* EntityStateArray = *(EntityStateArray_New**)(Hack::WaW_BaseAddress + 0xbe1c0);

	for (int i = 0; i < 1024; i++)
	{
		if (EntityStateArray->EntityStateArray[i].eType == eType && EntityStateArray->EntityStateArray[i].CurrentHealth > 0)
		{
			Vector2 screen = {};
			if (WorldToScreen(EntityStateArray->EntityStateArray[i].position, screen, Hack::pViewMatrix, Hack::RefDef->Width, Hack::RefDef->Height))
			{
				Draw::DrawLine(Hack::RefDef->Width / 2, Hack::RefDef->Height, screen.x, screen.y, 2, false, D3DCOLOR_ARGB(255, 182, 3, 252), dev);
				RECT rect;
				SetRect(&rect, screen.x, screen.y, screen.x + 120, screen.y + 100);
				std::string TypeString = std::format("Type:{}-ID:{}", (int)EntityStateArray->EntityStateArray[i].eType, i);
				pFont[0]->DrawTextA(NULL, TypeString.c_str(), -1, &rect, DT_LEFT | DT_TOP, D3DCOLOR_ARGB(255, 125, 125, 125));
			}
		}
	}

	return 1;
}
