#include <thread>
#include <fstream>
#include "Common.h"
#include "xorstr.h"
#include <TlHelp32.h>
#include <WinInet.h>
#include <urlmon.h>
#include <random>
#include "skCrypt.h"
#include <mutex>
#include "Menu.h"
#include "Entity.h"
c_menu_framework* menu_framework = new c_menu_framework;


using namespace std;


HWND hWnd;

void drawLoop(int width, int height)
{
	menu_framework->do_menu_controls();
	Items();
	Hack();
}

void OverlaySetup()
{
	DirectOverlaySetup(drawLoop, FindWindow(NULL, "DayZ"));
}




int main()
{
	ShowWindow(::GetConsoleWindow(), SW_SHOW);
	std::string select;
	std::string selectcheat;
	Entry();
	std::thread(OverlaySetup).detach();
	std::cin.get();
	

	
}