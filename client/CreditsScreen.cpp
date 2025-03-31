#include "CreditsScreen.h"

#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "imgui.h"
#include "Logger.h"
#include "Texture.h"
#include "Vector4D.h"

namespace GDEngine {
	CreditsScreen::CreditsScreen() : UIScreen("CreditsScreen")
	{
		isActive = false;
		Logger::log(this, "Initialized");
	}

	CreditsScreen::~CreditsScreen()
	{
	}

	void CreditsScreen::draw()
	{
		float freq = 1.0f;
		count += EngineTime::getDeltaTime();
		Vector4D rainbow = Vector4D(
			std::sin(freq * count + 0) * 127 + 128,
			std::sin(freq * count + 2) * 127 + 128,
			std::sin(freq * count + 4) * 127 + 128,
			255);

		Vector4D rainbow2 = Vector4D(
			std::sin(freq * count + 2) * 127 + 128,
			std::sin(freq * count + 0) * 127 + 128,
			std::sin(freq * count + 4) * 127 + 128,
			255);

		Vector4D rainbow3 = Vector4D(
			std::sin(freq * count + 4) * 127 + 128,
			std::sin(freq * count + 2) * 127 + 128,
			std::sin(freq * count + 0) * 127 + 128,
			255);

		ImGui::Begin("Credits", &isActive, ImGuiWindowFlags_NoDocking);

		Texture* dlsuTexture = GDEngine::GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"assets/textures/dlsu.png");

		ImGui::Image((ImTextureID)dlsuTexture->m_shaderResourceView, ImVec2(256, 256));

		ImVec4 vec4 = ImVec4(rainbow.x / 255.0f, rainbow.y / 255.0f, rainbow.z / 255.0f, rainbow.w / 255.0f);
		ImVec4 vec4_2 = ImVec4(rainbow2.x / 255.0f, rainbow2.y / 255.0f, rainbow2.z / 255.0f, rainbow2.w / 255.0f);
		ImVec4 vec4_3 = ImVec4(rainbow3.x / 255.0f, rainbow3.y / 255.0f, rainbow3.z / 255.0f, rainbow3.w / 255.0f);

		ImGui::TextColored(vec4, "Scene Editor v0.05");
		ImGui::TextColored(vec4_2, "Multiple Viewports Developed by:\n");
		ImGui::Text("Shane Laurenze Cablayan");
		ImGui::Text("Zachary Gadjiel Breinard Que");
		ImGui::Text("Andre Vito Valdecantos");
		ImGui::Text("Kate Nicole Young");
		ImGui::Text("");
		ImGui::Text("Acknowledgements");
		ImGui::Text("Pardcode's 3D Game Tutorial");
		ImGui::Text("DLSU GDENG03 Course");
		ImGui::Text("DirectX11");
		ImGui::Text("ImGUI Library");
		ImGui::Text("DirectXTex Library");

		ImGui::End();
	}
}