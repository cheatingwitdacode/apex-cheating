#include <thread>
#include <inttypes.h>
#include "globals.h"
#include "Util.h"
#include "XorStr.hpp"
#include "overlay.h"
#include "Modules.h"

void rcsLoop() {
	while (true) {
		Modules::NoRecoil();
		Sleep(1);
	}
}

void aimbotLoop() {
	while (true) {
		Modules::Aimbot();
		Sleep(1);
	}
}

int main(int argCount, char** argVector)
{
	srand(time(NULL)); // Seed random on start
	std::string filePath = argVector[0]; // i'll create this variable in case we need it later

	if (!Util::RenameFile(filePath)) /* renaming the file name and console window name
									 and saving it to the filePath variable */
	{
		printf(xorstr_("Failed to rename file!\n"));
		Sleep(5000);
		return -1;
	}

	bool eacRunning = Util::GetPid(xorstr_(L"EasyAntiCheat.exe"));
	
	if (!eacRunning)
	{
		printf(xorstr_("Loading driver..\n"));
		system(xorstr_("kdmapper.exe Driver.sys")); // mapping the driver | there is much better ways for doing this but it's enough

		Sleep(5000); // waiting before clearing the screen so you can see if the driver mapped successfully
		system(xorstr_("cls"));
	}

	if (!Driver.Init())
	{
		printf(xorstr_("Failed to get the hooked function\n"));
		Sleep(5000);
		return -1;
	}

	if (!eacRunning)
	{
		printf(xorstr_("Start Apex\n"));
		system(xorstr_("G:\\Origin\\Apex\\r5apex.exe"));
	}

	while (!globals.hWnd) /* make sure the game is running and has a window instance,
						  create a handle to the window and save it to a variable */
	{
		globals.hWnd = FindWindowA(NULL, xorstr_("Apex Legends"));
		Sleep(500);
	}

	while (!globals.pID) // get the process id
	{
		//globals.pID = Util::GetPid(xorstr_(L"EasyAntiCheat_launcher.exe"));
		globals.pID = Util::GetPid(xorstr_(L"r5apex.exe"));
		Sleep(500);
	}

	printf(xorstr_("Requesting base from driver\n"));

	while (!globals.moduleBase) // request the module base from driver
	{
		//globals.moduleBase = Driver.GetModuleBase(globals.pID, L"EasyAntiCheat_launcher.exe");
		globals.moduleBase = Driver.GetModuleBase(globals.pID, L"r5apex.exe");
		Sleep(500);
	}

	// create window
	if (!overlay::InitWindow())
	{
		printf(xorstr_("Failed to create window\n"));
		Sleep(5000);
		return 0;
	}

	if (!overlay::DirectXInit())
	{
		printf(xorstr_("Failed to initialize directx\n"));
		Sleep(5000);
		return 0;
	}

	// Seperate threads for rendering, aimbotting, recoil for maximum speed
	std::thread(overlay::Render).detach();
	std::thread(aimbotLoop).detach();
	std::thread(rcsLoop).detach();
	//std::thread(overlay::UpdateWindowPos).detach();

	globals.nameList = globals.moduleBase + OFFSET_NAMELIST;
	printf(xorstr_("success!\n"));

	Driver.wpm<int>(globals.moduleBase + OFFSET_DISABLE_BLOOM + 0x6C, 1);

	while (!GetAsyncKeyState(VK_END)) // Press 'END' to kill the cheat
	{
		Modules::Run();
		Sleep(1);
	}

	overlay::DirectXShutdown();
	
	return 0;
}