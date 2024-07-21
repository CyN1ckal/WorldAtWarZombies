#include "pch.h"

bool MyImGui::ShowMyWindow()
{

	ImGui::Begin("My Window");
	ImGui::Text("Test Text");
	ImGui::End();

	return 1;
}