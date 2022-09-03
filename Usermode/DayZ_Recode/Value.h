#pragma once
#include <cstdint>
#include <wtypes.h>
#include <vector>
#include <d3dx9.h>
inline float radius = 60.0f;
struct _globals
{
	HWND OverlayWnd;
	HWND TargetWnd;

	const char lTargetWindow[256] = "DayZ";
	const char lWindowName[256] = "Steam";

	uint64_t pID;
	uint64_t World;
	uint64_t RemoveFog;
	uint64_t Network;
	uint64_t Entitylist;
	uint64_t Entity;
	uint32_t Size;
	HWND GameWnd;

	bool menu = false;
	float width = GetSystemMetrics(SM_CXSCREEN);
	float height = GetSystemMetrics(SM_CYSCREEN);
};



struct _settings {
	float menux = 0;
	float menuy = 400;
	bool SilentAim = false;
	float test = 0.0;
	bool FovPlayer = false;
	float Horizontal = 1.333;
	float Vertical = 0.75;
	bool Line = false;
	bool Box = false;
	bool PlayerEsp = false;
	bool m_bDeadPlayers = false;
	bool ZombieEsp = false;
	bool Corpse = false;
	bool Weapon_Or_Ammo_Esp = false;
	bool QualityItems = false;
	bool Items = false;
	bool Barrel_Or_Wooden_Chest_Esp = false;
	bool Car_Esp = false;
	bool Building_Esp = false;
	bool Food_Medical_Esp = false;
	bool City_Esp = false;
	bool SpeedHack = true;
	bool SetDay = true;
	bool GrassDisable = false;
	
	bool DrawFov = false;
	float DistancePlayer = 150;
	float DistanceZombie = 150;
	float DistanceItems = 150;
	float CollorItems[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorPlayer[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorPlayerLine[4] = { 0.7, 0.7, 0.7, 1 };
	float ColorBox[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorZombie[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorCar[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorCity[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorWeapon[4] = { 0.7, 0.7, 0.7, 1 }; 
	float CollorQualityItems[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorEatHealth[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorFovAim[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorBarrel[4] = { 0.7, 0.7, 0.7, 1 };
	float CollorBuilding[4] = { 0.7, 0.7, 0.7, 1 };
	int zoombind = 0;
	bool zoomhack = false;
	bool zoomed = false;
	int zoomvalue = 4;
};



typedef struct _player_t
{
	std::uint64_t EntityPtr;
	std::uint64_t TableEntry;
	int NetworkID;
} player_t;

typedef struct _item_t
{
	std::uint64_t ItemPtr;
	std::uint64_t ItemTable;
} item_t;

extern _settings settings;
extern _globals globals;