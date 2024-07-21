#include "pch.h"

bool MyImGui::ShowMyWindow()
{

	ImGui::Begin("My Window");
	ImGui::Text("Test Text");
	ImGui::End();

	return true;
}

bool MyImGui::Initialize(LPDIRECT3DDEVICE9 pD3DDevice)
{
	D3DDEVICE_CREATION_PARAMETERS CP;
	pD3DDevice->GetCreationParameters(&CP);
	HWND window = CP.hFocusWindow;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pD3DDevice);
	MyImGui::Initialized = true;
	return true;
}