#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include "Driver.h"
#include "offsets.h"
#include "imgui/imgui.h"
#include "defs.h"

struct globalVars
{
	HWND hWnd;
	uintptr_t pID;
	uintptr_t moduleBase;

	uintptr_t localPlayer;
	std::vector<uintptr_t> entityList;
	uintptr_t nameList;
	uintptr_t viewRenderer;
	viewMatrix_t viewMatrix;

	POINT windowWH = { 0, 0 };

	ImVec4 fovCircleColor = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 espColorVisible = { 0.f, 1.f, 0.f, 1.f };
	ImVec4 espColorInvisible = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 espFillColorVisible = { 0.f, 1.f, 0.f, .039f };
	ImVec4 espFillColorInvisible = { 1.f, 0.f, 0.f, .039f };
	ImVec4 textColorVisible = { 0.f, 1.f, 0.f, 1.f };
	ImVec4 textColorInvisible = { 1.f, 0.f, 0.f, 1.f };
	ImVec4 headCircleColorVisible = { 0.f, 1.f, .1f, 1.f };
	ImVec4 headCircleColorInvisible = { 1.f, .1f, 0.f, 1.f };
	ImVec4 headLineColor = { 1.f, 0.f, 0.f, 1.f };

	bool rcs = true;								// Recoil Control System Toggle
	bool esp = true;								// Player Box ESP Toggle
	bool itemESP = true;							// Item ESP Toggle
	bool menuActive = false;						// imgui Toggle
	bool fillBox = false;

	float rcsIntensity = 100.f;
	float maxBoxDistance = 1000.f;
	float maxTextDistance = 300.f;
	float maxHealthDistance = 150.f;
	float maxHeadCircleDistance = 150.f;
	float maxHeadLineDistance = 150.f;
	int espThickness = 2;
	int headLineThickness = 2;

	/* Aimbot */
	bool aimbot = true;
	bool aimbotting = false;
	float aimbotFOV = 120.f;
	float aimbotSmoothness = 50.f;
	float aimbotOffset = 0.f;
	int aimbotKeyID = 0;
	const char * aimbotKeys[3] = { "Right Mouse", "Left Alt", "Left Mouse"}; // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

	/* Chams */
	const char* playerGlowStyles[2]{ "Normal", "Advanced" };
	int playerGlowStyle = 1;
	bool dummyGlow = true;
	bool playerGlow = true;
	bool playerGlowThroughWalls = true;
	float playerGlowBrightness = 1;
	Color playerGlowColor = { 0.f, 0.f, 1.f };

	/* Item Glow */
	bool itemGlow = true;
	bool deathBoxESP = true;
	/*
	*  Technically correct but they look bad
	Color heirloomColor = { 1.0f, 0.306f, 0.114f };
	Color legendaryColor = { 0.98f, 0.78f, 0.f };
	Color epicColor = { 0.34f, 0.f, 0.7f };
	Color rareColor = { 0.11f, 0.56f, 1.f };
	Color commonColor = { 0.55f, 0.55f, 0.5f };
	*/
	Color heirloomColor = { 0.5f, 0.f, 0.f };
	Color legendaryColor = { 1.f, 1.f, 0.f };
	Color epicColor = { 1.f, 0.f, 1.f };
	Color rareColor = { 0.f, 1.f, 1.f };
	Color commonColor = { 1.f, 1.f, 1.f };

	/* Third Person */
	bool thirdPerson = false;

	/* Speed Hack */
	bool rapidFire = true;
	float rapidFireSpeed = 2.f;

	int testInt = 0;
	bool testBool = false;
	float testFloat = 1.f;
	uintptr_t testPtr = 0x01ccf078 - 0x100 + 0x18;
};

extern KDriver Driver;
extern globalVars globals;