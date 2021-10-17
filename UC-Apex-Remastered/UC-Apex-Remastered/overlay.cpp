#pragma warning( disable : 4244 )
#include "overlay.h"
#include <string>
#include <filesystem>
#include <thread>
#include <algorithm>
#include <iterator>
#include "imgui/unknownController.h"
#include "imgui/FontAwesome.h"
#include "imgui/IconsFontAwesome.h"
#include "imgui/Comfortaa-Regular.h"
#include "imgui/Comfortaa-Bold.h"
#include "imgui/imgui_memory_editor.h"
#include <map>
#include "defs.h"
#include <string>

// set up these variables here in case we need them later
HWND overlayWindow;
IDirect3D9Ex* p_Object;
IDirect3DDevice9Ex* p_Device;
D3DPRESENT_PARAMETERS p_Params;
ID3DXLine* p_Line;
ID3DXFont* pFont;
ID3DXFont* pModuleFont;
ID3DXFont* pESPFont;
RECT rc;
MARGINS margin = { -1 };
ImFont* titleFont;
ImFont* bigFont;
MSG message;
IDirect3DTexture9* unknownCtrl;
float oldDelay = 0;

// winprochandler
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;

	case WM_DESTROY:
		overlay::DirectXShutdown();
		PostQuitMessage(0);
		return 0;
		break;

	default:
		return DefWindowProcW(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void HandleMessages(std::wstring lClassName, std::wstring lWindowName)
{
	// create the window and set window attributes
	overlayWindow = CreateWindowExW(/*WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED*/NULL, lClassName.c_str(), lWindowName.c_str(), WS_POPUP | WS_VISIBLE, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, 0);
	SetWindowLongA(overlayWindow, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
	DwmExtendFrameIntoClientArea(overlayWindow, &margin);
	ShowWindow(overlayWindow, SW_SHOW);
	UpdateWindow(overlayWindow);

	while (true)
	{
		if (PeekMessageW(&message, overlayWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessageW(&message);
			TranslateMessage(&message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == globals.hWnd)
		{
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(overlayWindow, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		Sleep(1);
	}
}

bool overlay::InitWindow()
{
	// create a window class
	std::string r1 = Util::RandomString(16);
	std::string r2 = Util::RandomString(16);

	std::wstring lClassName = std::wstring(r1.begin(), r1.end());
	std::wstring lWindowName = std::wstring(r2.begin(), r2.end());

	WNDCLASSEXW wClass;
	wClass.cbSize = sizeof(WNDCLASSEXW);
	wClass.style = 0;
	wClass.lpfnWndProc = WinProc;
	wClass.cbClsExtra = NULL;
	wClass.cbWndExtra = NULL;
	wClass.hInstance = nullptr;
	wClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hbrBackground = nullptr;
	wClass.lpszMenuName = lWindowName.c_str();
	wClass.lpszClassName = lClassName.c_str();
	wClass.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if (!RegisterClassExW(&wClass))
		return false;

	std::thread msgThread(HandleMessages, lClassName, lWindowName);
	msgThread.detach();

	return true;
}

bool overlay::DirectXInit()
{
	// initialize directx
	while (!overlayWindow) {}

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		return false;

	GetClientRect(globals.hWnd, &rc);

	globals.windowWH = { rc.right - rc.left, rc.bottom - rc.top };

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = overlayWindow;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = globals.windowWH.x;
	p_Params.BackBufferHeight = globals.windowWH.y;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
		return false;

	D3DXCreateLine(p_Device, &p_Line);

	D3DXCreateFont(p_Device, 50, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Helvetica", &pFont); // Create the font used for text rendering
	D3DXCreateFont(p_Device, 14, 0, FW_BOLD, 10, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Helvetica", &pESPFont); // Create the font used for esp rendering
	//D3DXCreateFontW
	if (FAILED(D3DXCreateTextureFromFileInMemory(p_Device, unknownController, sizeof(unknownController), &unknownCtrl)))
		return false;

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();

	// add default font and merge fontawesome icons
	io.Fonts->AddFontFromMemoryTTF(comfortaaRegular, sizeof(comfortaaRegular), 13.f);

	// you can uncomment this if you want to load the icon font/bold font
	//static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	//ImFontConfig icons_config;
	//icons_config.MergeMode = true;
	//icons_config.PixelSnapH = true;
	//icons_config.GlyphOffset = ImVec2(0.f, 2.f);
	//io.Fonts->AddFontFromMemoryTTF(font_awesome_data, sizeof(font_awesome_data), 16.f, &icons_config, icons_ranges); // icons font

	//// add title font and merge fontawesome icons
	//ImFontConfig titleConfig;
	//titleConfig.GlyphOffset = ImVec2(0.f, -2.f);
	//titleFont = io.Fonts->AddFontFromMemoryTTF(comfortaaBold, sizeof(comfortaaBold), 19.f, &titleConfig); // bold font

	//io.Fonts->AddFontFromMemoryTTF(font_awesome_data, sizeof(font_awesome_data), 21.f, &icons_config, icons_ranges);

	//// add big font and merge fontawesome icons
	//bigFont = io.Fonts->AddFontFromMemoryTTF(comfortaaRegular, sizeof(comfortaaRegular), 16.f);

	//icons_config.GlyphOffset = ImVec2(0.f, 1.f);
	//io.Fonts->AddFontFromMemoryTTF(font_awesome_data, sizeof(font_awesome_data), 18.f, &icons_config, icons_ranges);

	ImGui_ImplWin32_Init(overlayWindow);
	ImGui_ImplDX9_Init(p_Device);

	return true;
}

void InputHandler() {
	for (int i = 0; i < 5; i++) ImGui::GetIO().MouseDown[i] = false;
	int button = -1;
	if (GetAsyncKeyState(VK_LBUTTON)) button = 0;
	if (button != -1) ImGui::GetIO().MouseDown[button] = true;
}

ImU8 customRead(const ImU8* data, size_t offset) {
	return Driver.rpm<ImU8>(globals.testPtr + offset);
}

void customWrite(ImU8* data, size_t offset, ImU8 d) {
	Driver.wpm<ImU8>(globals.testPtr + offset, d);
}

void DrawImGui()
{
	// pass input to imgui
	InputHandler();

	// create an imgui frame
	
	// set the right window flags
	if (globals.menuActive)
		SetWindowLongA(overlayWindow, GWL_EXSTYLE, (WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));
	else
		SetWindowLongA(overlayWindow, GWL_EXSTYLE, (WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT));

	if (globals.menuActive)
	{
		static MemoryEditor mem_edit_1;
		char data[0x10000];
		size_t data_size = 0x10000;
		mem_edit_1.ReadFn = customRead;
		mem_edit_1.WriteFn = customWrite;
		mem_edit_1.DrawWindow(xorstr_("Memory Editor"), data, data_size, 0);

		ImGui::SetNextWindowSize(ImVec2(600, 700));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		/* -------------------------------------- Main Window Start ------------------------------------ */
		ImGui::Begin(xorstr_("Main"), 0);
		ImGui::InputInt(xorstr_("Test Int"), &globals.testInt);
		ImGui::SliderFloat(xorstr_("Test Float"), &globals.rapidFireSpeed, 1.f, 3.f);
		if (ImGui::Button(xorstr_("Test Button"))) {
			globals.testBool = !globals.testBool;
		}

		ImGui::Spacing();
		ImGui::Checkbox(xorstr_("Third Person"), &globals.thirdPerson);

		ImGui::Spacing();
		ImGui::Checkbox(xorstr_("Recoil Control"), &globals.rcs);
		ImGui::SliderFloat(xorstr_("RCS Intensity"), &globals.rcsIntensity, 0.f, 100.f, "%.1f%%");

		ImGui::Spacing();

		ImGui::ColorEdit4(xorstr_("Head Line"), (float*)&globals.headLineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::SliderInt(xorstr_("Head Line Thickness"), &globals.headLineThickness, 1, 5, "%dpx");
		
		ImGui::Spacing();

		ImGui::ColorEdit4(xorstr_("Text Color | Visible"), (float*)&globals.textColorVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(xorstr_("Text Color | Invisible"), (float*)&globals.textColorInvisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::SliderFloat(xorstr_("Max Box Distance"), &globals.maxBoxDistance, 0.f, 1000.f, "%.1fm");
		ImGui::SliderFloat(xorstr_("Max Text Distance"), &globals.maxTextDistance, 0.f, 1000.f, "%.1fm");
		ImGui::SliderFloat(xorstr_("Max Head Circle Distance"), &globals.maxHeadCircleDistance, 0.f, 1000.f, "%.1fm");
		ImGui::SliderFloat(xorstr_("Max Healthbars Circle Distance"), &globals.maxHealthDistance, 0.f, 1000.f, "%.1fm");
		ImGui::SliderFloat(xorstr_("Max Head Line Distance"), &globals.maxHeadLineDistance, 0.f, 1000.f, "%.1fm");

		ImGui::End();
		/* -------------------------------------- Main Window End ------------------------------------- */
		/* -------------------------------------- Player Glow Start ----------------------------------- */
		ImGui::Begin(xorstr_("Player ESP"), 0);
		
		// Box ESP Section
		ImGui::Checkbox(xorstr_("Box ESP"), &globals.esp);
		ImGui::ColorEdit4(xorstr_("Head Circle Color | Visible"), (float*)&globals.headCircleColorVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(xorstr_("Head Circle Color | Invisible"), (float*)&globals.headCircleColorInvisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::SliderInt(xorstr_("ESP Thickness"), &globals.espThickness, 1, 5, "%dpx");
		ImGui::ColorEdit4(xorstr_("ESP Color | Visible"), (float*)&globals.espColorVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(xorstr_("ESP Color | Invisible"), (float*)&globals.espColorInvisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::Checkbox(xorstr_("Fill Boxes"), &globals.fillBox);
		ImGui::ColorEdit4(xorstr_("ESP Fill Color | Visible"), (float*)&globals.espFillColorVisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);
		ImGui::ColorEdit4(xorstr_("ESP Fill Color | Invisible"), (float*)&globals.espFillColorInvisible, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_AlphaBar);

		ImGui::Spacing();
		
		// Chams section
		ImGui::Combo(xorstr_("Style"), &globals.playerGlowStyle, globals.playerGlowStyles, IM_ARRAYSIZE(globals.playerGlowStyles));
		ImGui::Checkbox(xorstr_("Glow"), &globals.playerGlow);
		if (globals.playerGlowStyles[globals.playerGlowStyle] == "Normal") {
			ImGui::ColorEdit3(xorstr_("Glow Color"), (float*)&globals.playerGlowColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
			ImGui::Checkbox(xorstr_("Through Walls"), &globals.playerGlowThroughWalls);
		}
		ImGui::End();
		/* -------------------------------------- Player Glow End ------------------------------------- */
		/* -------------------------------------- Item Glow Start ------------------------------------- */
		ImGui::Begin(xorstr_("Item ESP"), 0);
		ImGui::Checkbox(xorstr_("Item Glow"), &globals.itemGlow);
		ImGui::ColorEdit3(xorstr_("Common"), (float*)&globals.commonColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		ImGui::ColorEdit3(xorstr_("Rare"), (float*)&globals.rareColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		ImGui::ColorEdit3(xorstr_("Epic"), (float*)&globals.epicColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		ImGui::ColorEdit3(xorstr_("Legendary"), (float*)&globals.legendaryColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		ImGui::ColorEdit3(xorstr_("Heirloom"), (float*)&globals.heirloomColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
		ImGui::End();
		/* -------------------------------------- Item Glow End --------------------------------------- */
		/* -------------------------------------- Aimbot Start ---------------------------------------- */
		ImGui::Begin(xorstr_("Aimbot"), 0);
		ImGui::Checkbox(xorstr_("Aimbot"), &globals.aimbot);
		ImGui::SliderFloat(xorstr_("Smoothness"), &globals.aimbotSmoothness, 0, 100);
		ImGui::SliderFloat(xorstr_("Offset"), &globals.aimbotOffset, -20, 20);
		ImGui::SliderFloat(xorstr_("FOV"), &globals.aimbotFOV, 1, 300);
		ImGui::Combo(xorstr_("Hotkey"), &globals.aimbotKeyID, globals.aimbotKeys, IM_ARRAYSIZE(globals.aimbotKeys));
		// TODO: Add aimbot bone
		ImGui::End();
		/* -------------------------------------- Aimbot End ------------------------------------------ */
	}
}

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/client_class.h
struct ClientClass
{
	uint64_t pCreateFn;
	uint64_t pCreateEventFn;
	uint64_t pNetworkName;
	uint64_t pRecvTable;
	uint64_t pNext;
	uint32_t ClassID;
	uint32_t ClassSize;
};

// https://github.com/CasualX/apexbot/blob/d6dfbf40b5025e6676fe75177a39ddd285285160/src/state.cpp#L104
bool getClassNameX(uintptr_t ent, std::string& className)
{
	uintptr_t clientNetworkableVTable = Driver.rpm<uintptr_t>(ent + 8 * 3);
	if (!clientNetworkableVTable)
		return false;

	uintptr_t getClientClass = Driver.rpm<uintptr_t>(clientNetworkableVTable + 8 * 3);
	if (!getClientClass)
		return false;

	uint32_t disp = Driver.rpm<uint32_t>(getClientClass + 3);
	if (!disp) {
		return false;
	}
	const uint64_t client_class_ptr = getClientClass + disp + 7;
	// Get the ClientClass instance itself.
	ClientClass clientClass = Driver.rpm<ClientClass>(client_class_ptr);

	// Finally grab some bytes to be interpreted as a nul terminated c-string.
	className = Driver.ReadString(clientClass.pNetworkName, 128);
	if (className.length() == 128)
		className = "Unknown";
	return true;
}


void overlay::Render()
{
	static ImDrawList* drawList;

	while (!GetAsyncKeyState(VK_END))
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		DrawImGui();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin(xorstr_("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

		drawList = ImGui::GetWindowDrawList();

		p_Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.f, 0);
		p_Device->BeginScene();

		// Raw directx drawing
		if (Player::IsPlayer(globals.localPlayer))
		{
			// get view matrix every frame
			globals.viewMatrix = Driver.rpm<viewMatrix_t>(Driver.rpm<uintptr_t>(globals.viewRenderer + OFFSET_MATRIX));

			int index = 0; // we need the index for the visibility check
			// loop through the entity list
			for (uintptr_t& player : Player::GetPlayers())
			{
				bool visible = Player::IsVisible(player, index);
				index++;

				if (!Player::IsValidEnemy(player)) continue; // check if its a valid enemy

				vec3 targetHead = Util::GetBonePos(player, 8); // get head position
				vec2 targetHeadScreen;
				if (!Util::WorldToScreen(targetHead, targetHeadScreen)) continue; // convert to screen coordinates

				vec3 targetBody = Driver.rpm<vec3>(player + OFFSET_ORIGIN); // get vecOrigin
				vec2 targetBodyScreen;
				if (!Util::WorldToScreen(targetBody, targetBodyScreen)) continue; // convert to screen coordinates
				
				vec3 targetNeck = Util::GetBonePos(player, 7); // get neck bone
				vec2 targetNeckScreen;
				if (!Util::WorldToScreen(targetNeck, targetNeckScreen)) continue; // convert to screen coordinates

				if (globals.esp)
				{
					// calculate the stuff needed for the boxes
					float height = abs(abs(targetHeadScreen.y) - abs(targetBodyScreen.y));
					float width = height / 2.f;
					float middle = targetBodyScreen.x - (width / 2.f);
					float dist = 0.f;
					// If we're alive calculate it based off the localPlayer, however if we're dead use the player we're spectating
					if (!Player::IsDead(globals.localPlayer)) {
						dist = Driver.rpm<vec3>(globals.localPlayer + OFFSET_ORIGIN).DistTo(targetHead);
					}
					else
					{
						// Getting observer is aids
						int observingIndex = Driver.rpm<int>(globals.localPlayer + OFFSET_OBSERVING_TARGET);
						uintptr_t observingPlayer = Driver.rpm<uintptr_t>(globals.moduleBase + OFFSET_ENTITYLIST + (observingIndex << 5));
						dist = Driver.rpm<vec3>(observingPlayer + OFFSET_ORIGIN).DistTo(targetHead);
					}
					float distM = Util::ToMeters(dist);
					float neckHeadDistance = targetHeadScreen.Dist2D(targetNeckScreen);

					D3DCOLOR espColor;
					D3DCOLOR espFillColor;

					// get the appropriate colors
					if (visible)
					{
						espColor = Util::Vec4toARGB(globals.espColorVisible);
						espFillColor = Util::Vec4toARGB(globals.espFillColorVisible);
					}
					else
					{
						espColor = Util::Vec4toARGB(globals.espColorInvisible);
						espFillColor = Util::Vec4toARGB(globals.espFillColorInvisible);
					}

					// call the function to draw the box
					Modules::DrawBoxESP(middle, targetHeadScreen.y, width, height, globals.espThickness, espFillColor, espColor);

					if (distM <= globals.maxHealthDistance)
					{
						// call the function to draw the healthbars
						Modules::DrawHealthbars(middle, targetHeadScreen.y, width, height, 4, Driver.rpm<int>(player + OFFSET_HEALTH), Driver.rpm<int>(player + OFFSET_SHIELD), Driver.rpm<int>(player + OFFSET_ARMOR_TYPE));
					}

					if (distM <= globals.maxTextDistance)
					{
						D3DCOLOR textColor;

						// get the appropriate colors
						if (visible)
							textColor = Util::Vec4toARGB(globals.textColorVisible);
						else
							textColor = Util::Vec4toARGB(globals.textColorInvisible);

						// draw text
						DrawShadowString(Player::GetName(player).c_str(), middle, targetHeadScreen.y - 15, 1, textColor, ARGB(255, 0, 0, 0), pESPFont);
						std::string distance = std::to_string(distM);
						distance = distance.substr(0, distance.find('.')) + "m";
						DrawShadowString(distance.c_str(), middle, targetBodyScreen.y + 1, 1, textColor, ARGB(255, 0, 0, 0), pESPFont);
					}

					if (distM <= globals.maxHeadCircleDistance)
					{
						D3DCOLOR circleColor;

						// get the appropriate colors
						if (visible)
							circleColor = Util::Vec4toARGB(globals.headCircleColorVisible);
						else
							circleColor = Util::Vec4toARGB(globals.headCircleColorInvisible);

						// TODO: This needs to be fixed or removed, personally I don't care about it. Somethimes the circle breaks and becomes very large. I don't know why but I don't really care.
						DrawCircle(targetHeadScreen.x, targetHeadScreen.y, neckHeadDistance, 1.f, 20.f, circleColor); // we need the distance from head to neck so we can size it depending on distance
					}
				}
			}

		}

		// Draw FOV Circle
		int screenWidth = 1920;
		int screenHeight = 1080;
		DrawCircle(screenWidth / 2, screenHeight / 2, globals.aimbotFOV, 2.f, globals.aimbotFOV, Util::Vec4toARGB(globals.fovCircleColor));
		int seenTypes[50] = {};
		int seenIndex = 0;
		for (int i = 0; i < globals.entityList.size() && false; i++) {
			uintptr_t ent = globals.entityList[i];
			if (ent == NULL) continue;

			vec3 entPos = Driver.rpm<vec3>(ent + OFFSET_ORIGIN);
			vec2 w2sEntPos;
			Util::WorldToScreen(entPos, w2sEntPos);
			if (w2sEntPos.y <= 0.f)
				continue;
			
			std::string className;
			getClassNameX(ent, className);

			if (className == "CPropSurvival") { // TODO: Enable when getting item IDs
				int scriptID = Driver.rpm<int>(ent + OFFSET_SCRIPT_ID);
				DrawShadowString(std::to_string(scriptID).c_str(), w2sEntPos.x, w2sEntPos.y, 2, ARGB(255, 255, 255, 255), ARGB(255, 0, 0, 0), pESPFont);
			}

			if (
				// Firing Range
				className == "ScriptMoverLightweight" ||
				className == "CPropSurvival" ||
				className == "CBaseEntity" ||
				className == "CDynamicProp" ||
				className == "CWeaponX" || // Weapon in players hands
				className == "CPlayerWaypoint" ||
				className == "CBaseAnimating" ||
				className == "CGrappleHook" ||
				className == "CScriptNetData_SNDC_PLAYER_GLOBAL" || // wtf are these
				className == "CScriptNetData_SNDC_PLAYER_EXCLUSIVE" ||
				className == "CInfoTarget" ||
				className == "CAI_BaseNPC" ||
				className == "CZiplineEnd" ||
				className == "CZipline" ||
				className == "CAmbientGeneric" ||
				className == "CSkyCamera" ||
				className == "CParticleSystem" ||
				className == "CPointCamera" ||
				className == "CCascadeLight" ||
				className == "CTriggerNoZipline" ||
				className == "CEntityLinkPage" ||
				className == "CImportantOnEntSound" ||
				className == "Titan_Cockpit" ||
				className == "CPredictedFirstPersonProxy" ||
				className == "CTeam" ||
				className == "CBaseViewModel" ||
				className == "CWorld" ||
				className == "CFirstPersonProxy" ||
				className == "CEntityDissolve" ||
				className == "CPhysicsProp" ||
				className == "CGlobalNonRewinding" ||
				className == "CColorCorrection" ||
				className == "CScriptNetDataGlobal" ||
				className == "CScriptNetDataGlobalNonRewind" ||
				className == "CBaseGrenade" || // All grenades(Thermite, Frag, Arc Star)
				className == "CRopeKeyframe" || // Exists while using Pathfinder ultimate
				className == "CScriptProp" ||
				className == "CScriptTraceVolume" ||
				className == "CScriptMover" ||
				className == "CInfoPlacementHelper" ||
				className == "CCrossbowBolt" || // All(?) bullets
				className == "CStatusEffectPlugin" ||
				className == "CMissile" || // Missiles from ultimates(While in air)
				className == "CVortexSphere" ||
				className == "CPlayer" ||
				// World's Edge
				className == "CPropDoor" || // Door
				className == "CTriggerSlip" ||
				className == "CTriggerNoGrapple" ||
				className == "CDeathBoxProp" || // Death Box
				className == "CLootRoller" ||
				className == "CTriggerNoObjectPlacement" ||
				className == "CBoneFollower" ||
				className == "CBaseTrigger" ||
				className == "CScriptMoverTrainNode" ||
				className == "CScriptNetData_SNDC_DEATH_BOX" ||
				className == "CFuncBrush" ||
				className == "CLootGrabber" ||
				className == "CTriggerCylinderHeavy" ||
				className == "CTriggerUpdraft" ||
				className == "CDynamicPropLightweight" ||
				className == "CShieldProp" ||
				className == "CProjectile" ||
				className == "CNPC_Dropship" || // Respawn Ship(?)
				// Kings Canyon
				className == "CFuncBrushLightweight" || // Appeared in Selection Menu
				className == "CBreakableProp" ||
				className == "CPlayerDecoy" ||
				className == "CPlayerVehicle" || // Not sure how this existed on Kings Canyon
				className == "CTurret" ||
				className == "CKnockBack" ||
				className == "CZiplinePhysics" ||
				className == "CScriptNetData_SNDC_PLAYER_EXCLUSIVE_EXPANDED"
			) continue;

			//printf("%s\n", className.c_str());

			DrawShadowString(className.c_str(), w2sEntPos.x, w2sEntPos.y, 2, ARGB(255, 255, 255, 255), ARGB(255, 0, 0, 0), pESPFont);
		}
		/*
		 * CPropSurvival
		 * CDynamicProp
		 * CParticleSystem(DT_ParticleSystem)
		 * CAI_BaseNPC
		 * CWeaponX
		 * ScriptMoverLightweight - Targets that get shot down
		 * CBaseEntity
		 * CZiplineEnd
		 * CZipline
		*/
		drawList->PushClipRectFullScreen();
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		p_Device->EndScene();
		p_Device->PresentEx(0, 0, 0, 0, 0);
	}
}

void overlay::DirectXShutdown()
{
	// destroy everything
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();

	p_Object->Release();
	p_Device->Release();
	pFont->Release();
	pESPFont->Release();
	p_Line->Release();

	DestroyWindow(overlayWindow);
}
/*
void overlay::UpdateWindowPos()
{
	RECT oldRC = rc;
	while (!GetAsyncKeyState(VK_END))
	{
		// update the window position every 100 ms
		globals.hWnd = FindWindowA(NULL, xorstr_("Apex Legends"));

		if (globals.hWnd)
		{
			GetWindowRect(globals.hWnd, &rc);
			globals.windowWH = { rc.right - rc.left, rc.bottom - rc.top };

			DWORD dwStyle = GetWindowLong(globals.hWnd, GWL_STYLE);
			if (dwStyle & WS_BORDER)
			{
				rc.top += 32;
				globals.windowWH.y -= 39;
			}

			MoveWindow(overlayWindow, rc.left, rc.top, globals.windowWH.x, globals.windowWH.y, true);
		}

		Sleep(100);
	}
}*/