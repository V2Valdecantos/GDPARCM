#include "UIManager.h"


#include "EngineTime.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "ViewportManager.h"

#include "HierarchyScreen.h"
#include "imgui_internal.h"
#include "InspectorScreen.h"
#include "MenuScreen.h"
#include "ProfilerScreen.h"
#include "ViewportScreen.h"
#include "ToolsScreen.h"
#include "PlaybackScreen.h"
#include "ColorPickerScreen.h"
#include "CreditsScreen.h"
#include "EngineBackend.h"

namespace GDEngine {

	UIManager* UIManager::P_SHARED_INSTANCE = NULL;

	int UIManager::RESIZE_WIDTH = 0;
	int UIManager::RESIZE_HEIGHT = 0;

	void UIManager::draw()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Make Main Viewport into a Dock Space

		this->drawDockspace();

		for (UIScreen* screen : listUI)
		{
			if (screen->isActive)
				screen->draw();
		}

		ViewportManager::getInstance()->update();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void UIManager::drawDockspace()
	{
		//ImGuiID id = ImGui::GetID("Main Window");
		//ImGui::DockSpaceOverViewport(id, ImGui::GetMainViewport());

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiWindowFlags host_window_flags = 0;
		host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
		host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Main Window", NULL, host_window_flags);
		ImGui::PopStyleVar(3);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

		ImGuiStyle& style = ImGui::GetStyle();
		float width = 0.0f;
		width += ImGui::CalcTextSize("Play").x;
		width += style.ItemSpacing.x;
		width += ImGui::CalcTextSize("Pause").x;
		width += style.ItemSpacing.x;
		width += ImGui::CalcTextSize("Step").x;
		AlignForWidth(width);

		EngineBackend* backend = EngineBackend::getInstance();
		if (ImGui::Button("Play"))
		{
			if (backend->getMode() == EngineBackend::EDITOR)
			{
				backend->setMode(EngineBackend::PLAY);
			}
			else
			{
				backend->setMode(EngineBackend::EDITOR);
			}

		}

		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			if (backend->getMode() == EngineBackend::PLAY)
			{
				backend->setMode(EngineBackend::PAUSED);
			}
			else if (backend->getMode() == EngineBackend::PAUSED)
			{
				backend->setMode(EngineBackend::PLAY);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Step"))
		{
			if (backend->getMode() == EngineBackend::PAUSED)
			{
				backend->startFrameStep();
			}
		}
		ImGui::PopStyleVar();

		ImGuiID id = ImGui::GetID("Main Window");
		ImGui::DockSpace(id);

		ImGui::End();

		if (firstTime)
		{
			ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();

			ImGui::DockBuilderRemoveNode(id);
			ImGui::DockBuilderAddNode(id);

			ImVec2 size{ WINDOW_WIDTH, WINDOW_HEIGHT };
			ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };

			ImGui::DockBuilderSetNodeSize(id, size);
			ImGui::DockBuilderSetNodePos(id, nodePos);

			ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.3f, nullptr, &id);
			ImGuiID dock2 = ImGui::DockBuilderSplitNode(dock1, ImGuiDir_Left, 0.5f, nullptr, &dock1);
			ImGuiID dock3 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);
			ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock3, ImGuiDir_Down, 0.25f, nullptr, &dock3);

			ImGuiID dockProfiler = ImGui::DockBuilderSplitNode(dock4, ImGuiDir_Left, 0.25f, nullptr, &dock4);
			ImGuiID dockLogger = dock4;

			ImGuiID dock3_top = ImGui::DockBuilderSplitNode(dock3, ImGuiDir_Up, 0.5f, nullptr, &dock3);
			ImGuiID dock3_bottom = dock3;

			ImGuiID dock3_top_left = ImGui::DockBuilderSplitNode(dock3_top, ImGuiDir_Left, 0.5f, nullptr, &dock3_top);
			ImGuiID dock3_top_right = dock3_top;

			ImGuiID dock3_bottom_left = ImGui::DockBuilderSplitNode(dock3_bottom, ImGuiDir_Left, 0.5f, nullptr, &dock3_bottom);
			ImGuiID dock3_bottom_right = dock3_bottom;

			ImGui::DockBuilderDockWindow("Inspector", dock1);
			ImGui::DockBuilderDockWindow("Hierarchy", dock2);
			ImGui::DockBuilderDockWindow("Viewport 1", dock3_top_left);
			ImGui::DockBuilderDockWindow("Viewport 2", dock3_top_right);
			ImGui::DockBuilderDockWindow("Viewport 3", dock3_bottom_left);
			ImGui::DockBuilderDockWindow("Viewport 4", dock3_bottom_right);
			ImGui::DockBuilderDockWindow("Profiler", dockProfiler);
			ImGui::DockBuilderDockWindow("Logger", dockLogger);

			ImGui::DockBuilderFinish(id);

			firstTime = false;
			Logger::log(this, "Initialized Dock Space on First Run");
		}

	}

	void UIManager::setActive(std::string name)
	{
		this->mapUI[name]->setActive(true);
	}

	void UIManager::setUIStyle()
	{
		// girlypop style from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 0.6000000238418579f;
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 11.80000019073486f;
		style.WindowBorderSize = 1.0f;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(4.0f, 3.0f);
		style.FrameRounding = 7.800000190734863f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 14.0f;
		style.ScrollbarRounding = 9.199999809265137f;
		style.GrabMinSize = 9.899999618530273f;
		style.GrabRounding = 20.0f;
		style.TabRounding = 5.599999904632568f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.274678111076355f, 0.2746753692626953f, 0.2746753692626953f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 0.9570815563201904f, 0.9714493155479431f, 0.8117647171020508f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.9800000190734863f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.300000011920929f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9764705896377563f, 0.6624683141708374f, 0.8693404197692871f, 0.4000000059604645f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.9764705896377563f, 0.6624683141708374f, 0.8403581380844116f, 0.6705882549285889f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.8117647171020508f, 0.6430450081825256f, 0.7224425077438354f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.5607843399047852f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.5803921818733215f, 0.7843137383460999f, 0.5098039507865906f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.9055793881416321f, 0.8123008608818054f, 0.8723514080047607f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.5299999713897705f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 0.800000011920929f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 0.800000011920929f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.6235294342041016f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.6235294342041016f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.6235294342041016f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.6235294342041016f, 0.7843137383460999f, 0.4000000059604645f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.5882353186607361f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.5882353186607361f, 0.7921568751335144f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(1.0f, 0.5882353186607361f, 0.7882353067398071f, 0.3098039329051971f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.5882353186607361f, 0.7882353067398071f, 0.800000011920929f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.5882353186607361f, 0.8039215803146362f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.4627451002597809f, 0.3882353007793427f, 0.3882353007793427f, 0.6196078658103943f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 0.5960784554481506f, 0.800000011920929f, 0.7803921699523926f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 0.5882353186607361f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 0.6196078658103943f, 0.7411764860153198f, 0.168627455830574f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 0.5882353186607361f, 0.7960784435272217f, 0.6705882549285889f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 0.5882353186607361f, 0.8117647171020508f, 0.9490196108818054f);
		style.Colors[ImGuiCol_Tab] = ImVec4(1.0f, 0.7960784435272217f, 0.8352941274642944f, 0.5176470875740051f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.5882353186607361f, 0.7882353067398071f, 0.800000011920929f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.5882353186607361f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(1.0f, 0.6235294342041016f, 0.7843137383460999f, 0.7058823704719543f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.0f, 0.5882353186607361f, 0.7921568751335144f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3803921639919281f, 0.2356939613819122f, 0.3037872314453125f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.6078431606292725f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.6980392336845398f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.4470588266849518f, 0.5882353186607361f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(1.0f, 0.5882353186607361f, 0.7843137383460999f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686274766921997f, 0.5686274766921997f, 0.6392157077789307f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784313917160034f, 0.6784313917160034f, 0.7372549176216125f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.501960813999176f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 0.8841201663017273f, 0.9393014907836914f, 0.501960813999176f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 0.6235294342041016f, 0.7843137383460999f, 0.3490196168422699f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 0.5882353186607361f, 0.7803921699523926f, 0.9490196108818054f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.0f, 0.5882353186607361f, 0.7843137383460999f, 0.800000011920929f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.699999988079071f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);

	}

	void UIManager::addViewport(UIScreen* viewport)
	{
		UINames uiNames;
		this->mapUI[uiNames.VIEWPORT_SCREEN] = viewport;
		this->listUI.push_back(viewport);
	}

	void UIManager::AlignForWidth(float width, float alignment)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - width) * alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	UIManager::UIManager(HWND hwnd)
	{
		RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		this->setUIStyle();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(renderSystem->getDirectXDevice(), renderSystem->getImmediateDeviceContext()->getContext());

		UINames uiNames;
		PlaybackScreen* playbackScreen = new PlaybackScreen();
		this->mapUI[uiNames.PLAYBACK_SCREEN] = playbackScreen;
		this->listUI.push_back(playbackScreen);

		MenuScreen* menuScreen = new MenuScreen();
		this->mapUI[uiNames.MENU_SCREEN] = menuScreen;
		this->listUI.push_back(menuScreen);

		CreditsScreen* creditsScreen = new CreditsScreen();
		this->mapUI[uiNames.CREDITS_SCREEN] = creditsScreen;
		this->listUI.push_back(creditsScreen);

		HierarchyScreen* hierarchyScreen = new HierarchyScreen();
		this->mapUI[uiNames.HIERARCHY_SCREEN] = hierarchyScreen;
		this->listUI.push_back(hierarchyScreen);

		InspectorScreen* inspectorScreen = new InspectorScreen();
		this->mapUI[uiNames.INSPECTOR_SCREEN] = inspectorScreen;
		this->listUI.push_back(inspectorScreen);

		ProfilerScreen* profilerScreen = new ProfilerScreen();
		this->mapUI[uiNames.PROFILER_SCREEN] = profilerScreen;
		this->listUI.push_back(profilerScreen);

		ColorPickerScreen* colorPickerScreen = new ColorPickerScreen();
		this->mapUI[uiNames.COLOR_PICKER_SCREEN] = colorPickerScreen;
		this->listUI.push_back(colorPickerScreen);


		LoggerScreen* loggerScreen = new LoggerScreen();
		this->mapUI[uiNames.LOGGER_SCREEN] = loggerScreen;
		this->listUI.push_back(loggerScreen);

		ViewportManager::getInstance()->createViewport();


		Logger::log(this, "Initialized");
	}

	UIManager::~UIManager()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		Logger::log(this, "Destroyed");
	}

	UIManager::UIManager(const UIManager&) {}

	UIManager* UIManager::getInstance()
	{
		return P_SHARED_INSTANCE;
	}

	void UIManager::initialize(HWND hwnd)
	{
		if (P_SHARED_INSTANCE)
			Logger::throw_exception("UIManager already created");
		P_SHARED_INSTANCE = new UIManager(hwnd);
	}

	void UIManager::destroy()
	{
		delete P_SHARED_INSTANCE;
	}
}
