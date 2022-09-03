#include <thread>
#include <Windows.h>
#include "driver.h"
#include "SDK.h"
#include "Common.h"
#include "Vector.h"
#include "Value.h"
#include "Overlay.h"
#include "xorstr.h"
#include "Entity.h"
#include <iostream>
#include <sstream>
#include <thread>
#include<string.h>
#include <amp.h>
#include <WinUser.h>
#include "offset.h"

using namespace std;

uintptr_t base_address{};
std::vector<player_t> entities = {};
std::vector<item_t> items = {};
uintptr_t process_id;
#define safe_read(Addr, Type) DriverAPI.Read<Type>(Addr)
#define safe_write(Addr, Data, Type) DriverAPI.Write<Type>(Addr, Data)
ProcCBACAPC NtConvertBetweenAuxiliaryCounterAndPerformanceCounter = 0x0;

DWORD FindPID(const char* szWndName)
{
	DWORD PID = 0;
	DWORD ThreadID = GetWindowThreadProcessId(FindWindowA(szWndName, nullptr), &PID);
	return PID;
}
int pId = 0;
void Entry()
{
	auto module = LoadLibraryW((L"ntdll.dll"));
	NtConvertBetweenAuxiliaryCounterAndPerformanceCounter = (ProcCBACAPC)GetProcAddress(module, xorstr("NtConvertBetweenAuxiliaryCounterAndPerformanceCounter"));

	pId = FindPID(xorstr("DayZ"));
	DriverAPI.SetPID(pId);
	base_address = DriverAPI.GetModInfo((xorstr("DayZ_x64.exe"))).ModBase;
	cout << "PID: \t" << pId << endl;
	cout << "Base address: \t" << base_address <<endl;
	globals.World = DriverAPI.Read<uint64_t>(base_address + 0x40B7D50); 
	cout << "globals.World: \t" << globals.World << endl;
	
	std::thread(UpdateList).detach();
	std::thread(UpdateItems).detach();
}



uint64_t TargetPlayer;

int addr;
mutex sync;

void UpdateList()
{
	while (true)
	{
		std::vector<player_t> tmp{};

		sync.lock();

		uint64_t NearTableSize = DriverAPI.Read<uint32_t>(globals.World + oNearEntityTable + 0x08);
		uint64_t FarTableSize = DriverAPI.Read<uint32_t>(globals.World + oFarEntityTable + 0x08); 

		for (int i = 0; i < NearTableSize; i++)
		{
			uint64_t EntityTable = DriverAPI.Read<uint64_t>(globals.World + oNearEntityTable); 
			if (!EntityTable) continue;

			uint64_t Entity = DriverAPI.Read<uint64_t>(EntityTable + (i * 0x8));
			if (!Entity) continue;

			uintptr_t networkId = Game::GetNetworkId(Entity);
			if (networkId == 0) continue;

			player_t Player{};
			Player.NetworkID = networkId;
			Player.TableEntry = EntityTable;
			Player.EntityPtr = Entity;
			tmp.push_back(Player);
		}

		for (int i = 0; i < FarTableSize; i++)
		{
			uint64_t EntityTable = DriverAPI.Read<uint64_t>(globals.World + oFarEntityTable); 
			if (!EntityTable) continue;
			
			uint64_t Entity = DriverAPI.Read<uint64_t>(EntityTable + (i * 0x8));
			if (!Entity) continue;
			
			uintptr_t networkId = Game::GetNetworkId(Entity);
			uintptr_t worldptr = DriverAPI.Read<uintptr_t>(base_address + globals.World);;
			if (networkId == 0) continue;
			player_t Player{};
			Player.NetworkID = networkId;
			Player.TableEntry = EntityTable;
			Player.EntityPtr = Entity;

			if (settings.SilentAim) 
			{
				Game::SilentAim(TargetPlayer);
			}
			if (settings.FovPlayer)
			{
				DriverAPI.Write<float>(DriverAPI.Read<uint64_t>(globals.World + 0x28) + 0x6C, settings.Horizontal);
				DriverAPI.Write(DriverAPI.Read<uint64_t>(globals.World + 0x28) + 0x70, settings.Vertical);
			}
			if (settings.GrassDisable) {
				Game::SetTerrainGrid(0);
			}	
			else {
				Game::SetTerrainGrid(12);
			}
			tmp.push_back(Player);
		}
		entities = tmp;
		sync.unlock();
	}
}

void UpdateItems()
{
	while (true)
	{
		std::vector<item_t> tmp{};

		sync.lock();

		uint64_t ItemTableSize = DriverAPI.Read<uint32_t>(globals.World + oItemTables + 0x08); 
		for (int i = 0; i < ItemTableSize; i++)
		{
			uint64_t ItemTable = DriverAPI.Read<uint64_t>(globals.World + oItemTables);
			if (!ItemTable) continue;

			int Check = DriverAPI.Read<int>(ItemTable + (i * 0x18));
			if (Check != 1) continue;

			uint64_t ItemEntity = DriverAPI.Read<uint64_t>(ItemTable + ((i * 0x18) + 0x8));

			item_t Item{};
			Item.ItemTable = ItemTable;
			Item.ItemPtr = ItemEntity;
			tmp.push_back(Item);
		}
		items = tmp;
		sync.unlock();
		
	}
}

bool Checking(std::string firstString, std::string secondString) {
	if (secondString.size() > firstString.size())
		return false;

	for (int i = 0; i < firstString.size(); i++) {
		int j = 0;
		if (firstString[i] == secondString[j]) {
			int k = i;
			while (firstString[i] == secondString[j] && j < secondString.size()) {
				j++;
				i++;
			}
			if (j == secondString.size())
				return true;
			else
				i = k;
		}
	}
	return false;
}
void Items()
{

	for (int i = 0; i < items.size(); i++)
	{
		auto ItemEntities = items[i];

		Vector3 worldPosition = Game::GetCoordinate(ItemEntities.ItemPtr);
		Vector3 screenPosition;

		Game::WorldToScreen(worldPosition, screenPosition);

		if (screenPosition.z < 1.0f) continue;
		int distance = Game::GetDistanceToMe(worldPosition);

		if (screenPosition.z >= 1.0f)
		{
			std::string TypeName = Game::GetEntityTypeName(ItemEntities.ItemPtr);
			std::string Name = Game::GetEntityName(ItemEntities.ItemPtr);

			if (settings.QualityItems)
			{
				draw_text_white(screenPosition.x, screenPosition.y + 40, (Game::GetQuality(ItemEntities.ItemPtr)).c_str());
			}
			if (settings.Items && distance <= settings.DistanceItems) 
			{
				draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
			}

			if (distance <= 200)
			{

				if (TypeName == xorstr("ProxyMagazines"))
				{
					if (settings.Weapon_Or_Ammo_Esp)
					{
						draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
					}
				}
				if (TypeName == xorstr("Weapon"))
				{
					if (settings.Weapon_Or_Ammo_Esp)
					{
						draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
					}
				}
				if (TypeName == xorstr("carwheel"))
				{
					if (settings.Car_Esp)
					{
						draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
					}
				}
				if (TypeName == xorstr("clothing"))
				{
					if (settings.Barrel_Or_Wooden_Chest_Esp)
					{
						if (Checking(Name, xorstr("Bag")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
					}
				}
				if (TypeName == xorstr("inventoryItem"))
				{
					if (settings.Food_Medical_Esp)
					{
						if (Name == xorstr("BandageDressing")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("TetracyclineAntibiotics")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Morphine")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Syringe")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("SyringeClear")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Defibrillator")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("DisinfectantAlcohol")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("CharcoalTablets")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("BloodBag_Empty")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("BloodBag")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Epinephrine")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("PainkillerTablets")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("PurificationTablets")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("VitaminBottle")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("FirstAidKit")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Chips")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Apple")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Crackers")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("PorkCan")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("PorkCan")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("CaninaBerry")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Pajka")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Pate")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("BrisketSpread")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Plum")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Vodka")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Pear")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Banana")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Orange")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Tomato")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Zucchini")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Pumpkin")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Kiwi")) {
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Mushroom")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Bacon")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("BakedBeans")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Spaghetti")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Sardines")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Tuna")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Tuna")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Chips")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("SodaCan")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Water")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Food")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
					}
					if (settings.Building_Esp)
					{

						if (Checking(Name, xorstr("CombinationLock")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Pliers"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Rope"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("WoodAxe"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Tool_Pliers"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("WoodAxe"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Shovel"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("FieldShovel"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Hacksaw"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("HandSaw"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("Nail"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("NailBox"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("WoodenPlank"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("WoodenLog"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("MetalPlate"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
					}
					if (settings.Weapon_Or_Ammo_Esp)
					{
						if (Checking(Name, xorstr("AmmoBox")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
					}
					if (settings.Barrel_Or_Wooden_Chest_Esp)
					{
						if (Name == xorstr("WoodenCrate"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Barrel")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Tent")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Stash")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
					}
					if (settings.Car_Esp)
					{
						if (Name == xorstr("CarRadiator"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("SparkPlug"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Name == xorstr("CarBattery"))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Hatchback")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Sedan_02")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("CivSedan")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Truck_01")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("Niva2329")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("UAZ469")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
						if (Checking(Name, xorstr("URAL_375_1964")))
						{
							draw_text_white(screenPosition.x, screenPosition.y + 25, (Name + "[" + std::to_string(distance) + xorstr("m]")).c_str());
						}
					}
				}
			}
		}
	}
}

void Hack()
{
	if (settings.DrawFov) 
	{
		if (settings.SilentAim) {
			
			DrawCircle(globals.width / 2, globals.height / 2, radius * 8, 1, 255, 255, 255, 255, false);
		}
	}
	if (settings.City_Esp)
	{
		Vector3 berezino_down_world_position = { 12972, 6, 10058 };
		Vector3 berezino_screen_position;

		Vector3 solnechniy_world_position = { 13453, 7, 6239 };
		Vector3 solnechniy_screen_position;

		Vector3 kamyshovo_world_position = { 12010, 7, 3486 };
		Vector3 kamyshovo_screen_position;

		Vector3 electrozavodsk_world_position = { 10430, 7, 2277 };
		Vector3 electrozavodsk_screen_position;

		Vector3 prigorodki_world_position = { 7762, 5, 3171 };
		Vector3 prigorodki_screen_position;

		Vector3 chernogorsk_world_position = { 6400, 10, 2679 };
		Vector3 chernogorsk_screen_position;

		Vector3 svetloyarsk_world_position = { 13929, 20, 13234 };
		Vector3 svetloyarsk_screen_position;

		Vector3 mogilevka_world_position = { 7500, 220, 5238 };
		Vector3 mogilevka_screen_position;

		Vector3 guglovo_world_position = { 8412, 360, 6688 };
		Vector3 guglovo_screen_position;

		Vector3 kabanino_world_position = { 5297, 336, 8564 };
		Vector3 kabanino_screen_position;

		Vector3 kozlovka_world_position = { 4452, 230, 4578 };
		Vector3 kozlovka_screen_position;

		Vector3 nadezhdino_world_position = { 5865, 133, 4827 };
		Vector3 nadezhdino_screen_position;

		Vector3 airfield_vybor_world_position = { 4617, 340, 10438 };
		Vector3 airfield_vybor_screen_position;

		Vector3 novodmitrovsk_world_position = { 11619, 43, 14392 };
		Vector3 novodmitrovsk_screen_position;

		Vector3 zelenogorsk_world_position = { 2739, 200, 5223 };
		Vector3 zelenogorsk_screen_position;

		Vector3 krasnostav_world_position = { 11252, 194, 12231 };
		Vector3 krasnostav_screen_position;

		Vector3 severograd_world_position = { 7951, 115, 13688 };
		Vector3 severograd_screen_position;

		Vector3 stariy_sobor_world_position = { 6075, 300, 7757 };
		Vector3 stariy_sobor_screen_position;

		Vector3 military_base_pavlovo_world_position = { 2144, 91, 3369 };
		Vector3 military_base_pavlovo_screen_position;

		Vector3 military_base_kabanino_world_position = { 4570, 318, 8266 };
		Vector3 military_base_kabanino_screen_position;

		Game::WorldToScreen(berezino_down_world_position, berezino_screen_position);
		Game::WorldToScreen(solnechniy_world_position, solnechniy_screen_position);
		Game::WorldToScreen(kamyshovo_world_position, kamyshovo_screen_position);
		Game::WorldToScreen(electrozavodsk_world_position, electrozavodsk_screen_position);
		Game::WorldToScreen(prigorodki_world_position, prigorodki_screen_position);
		Game::WorldToScreen(chernogorsk_world_position, chernogorsk_screen_position);
		Game::WorldToScreen(svetloyarsk_world_position, svetloyarsk_screen_position);
		Game::WorldToScreen(mogilevka_world_position, mogilevka_screen_position);
		Game::WorldToScreen(guglovo_world_position, guglovo_screen_position);
		Game::WorldToScreen(kabanino_world_position, kabanino_screen_position);
		Game::WorldToScreen(kozlovka_world_position, kozlovka_screen_position);
		Game::WorldToScreen(nadezhdino_world_position, nadezhdino_screen_position);
		Game::WorldToScreen(airfield_vybor_world_position, airfield_vybor_screen_position);
		Game::WorldToScreen(novodmitrovsk_world_position, novodmitrovsk_screen_position);
		Game::WorldToScreen(zelenogorsk_world_position, zelenogorsk_screen_position);
		Game::WorldToScreen(krasnostav_world_position, krasnostav_screen_position);
		Game::WorldToScreen(severograd_world_position, severograd_screen_position);
		Game::WorldToScreen(stariy_sobor_world_position, stariy_sobor_screen_position);
		Game::WorldToScreen(military_base_pavlovo_world_position, military_base_pavlovo_screen_position);
		Game::WorldToScreen(military_base_kabanino_world_position, military_base_kabanino_screen_position);

		int distance_berezino = Game::GetDistanceToMe(berezino_down_world_position);
		int distance_solnechniy = Game::GetDistanceToMe(solnechniy_world_position);
		int distance_kamyshovo = Game::GetDistanceToMe(kamyshovo_world_position);
		int distance_electrozavodsk = Game::GetDistanceToMe(electrozavodsk_world_position);
		int distance_prigorodki = Game::GetDistanceToMe(prigorodki_world_position);
		int distance_chernogorsk = Game::GetDistanceToMe(chernogorsk_world_position);
		int distance_svetloyarsk = Game::GetDistanceToMe(svetloyarsk_world_position);
		int distance_mogilevka = Game::GetDistanceToMe(mogilevka_world_position);
		int distance_guglovo = Game::GetDistanceToMe(guglovo_world_position);
		int distance_kabanino = Game::GetDistanceToMe(kabanino_world_position);
		int distance_kozlovka = Game::GetDistanceToMe(kozlovka_world_position);
		int distance_nadezhdino = Game::GetDistanceToMe(nadezhdino_world_position);
		int distance_airfield_vybor = Game::GetDistanceToMe(airfield_vybor_world_position);
		int distance_novodmitrovsk = Game::GetDistanceToMe(novodmitrovsk_world_position);
		int distance_zelenogorsk = Game::GetDistanceToMe(zelenogorsk_world_position);
		int distance_krasnostav = Game::GetDistanceToMe(krasnostav_world_position);
		int distance_severograd = Game::GetDistanceToMe(severograd_world_position);
		int distance_stariy_sobor = Game::GetDistanceToMe(stariy_sobor_world_position);
		int distance_military_base_pavlovo = Game::GetDistanceToMe(military_base_pavlovo_world_position);
		int distance_military_base_kabanino = Game::GetDistanceToMe(military_base_kabanino_world_position);

		

		
		draw_text_white(berezino_screen_position.x, berezino_screen_position.y, (xorstr("Berezino (Down) [") + std::to_string(distance_berezino) + xorstr("m]")).c_str());
		draw_text_white(solnechniy_screen_position.x, solnechniy_screen_position.y, (xorstr("Solnechniy [") + std::to_string(distance_solnechniy) + xorstr("m]")).c_str());
		draw_text_white(kamyshovo_screen_position.x, kamyshovo_screen_position.y, (xorstr("Kamyshovo [") + std::to_string(distance_kamyshovo) + xorstr("m]")).c_str());
		draw_text_white(electrozavodsk_screen_position.x, electrozavodsk_screen_position.y, (xorstr("Electrozavodsk [") + std::to_string(distance_electrozavodsk) + xorstr("m]")).c_str());

		
		
		draw_text_white(prigorodki_screen_position.x, electrozavodsk_screen_position.y , (xorstr("Prigorodki [") + std::to_string(distance_prigorodki) + xorstr("m]")).c_str());
		draw_text_white(chernogorsk_screen_position.x, electrozavodsk_screen_position.y, (xorstr("Chernogorsk [") + std::to_string(distance_chernogorsk) + xorstr("m]")).c_str());
		draw_text_white(svetloyarsk_screen_position.x, electrozavodsk_screen_position.y, (xorstr("Svetloyarsk[") + std::to_string(distance_svetloyarsk) + xorstr("m]")).c_str());
		draw_text_white(mogilevka_screen_position.x, electrozavodsk_screen_position.y, (xorstr("Mogilevka [") + std::to_string(distance_mogilevka) + xorstr("m]")).c_str());
		draw_text_white(guglovo_screen_position.x, guglovo_screen_position.y, (xorstr("Guglovo [") + std::to_string(distance_guglovo) + xorstr("m]")).c_str());
		draw_text_white(kabanino_screen_position.x, kabanino_screen_position.y, (xorstr("Kabanino [") + std::to_string(distance_kabanino) + xorstr("m]")).c_str());
		draw_text_white(kozlovka_screen_position.x, kozlovka_screen_position.y, (xorstr("Kozlovka[") + std::to_string(distance_kozlovka) + xorstr("m]")).c_str());
		draw_text_white(nadezhdino_screen_position.x, nadezhdino_screen_position.y, (xorstr("Nadezhdino [") + std::to_string(distance_nadezhdino) + xorstr("m]")).c_str());
		draw_text_white(airfield_vybor_screen_position.x, airfield_vybor_screen_position.y, (xorstr("Air-Field (Vybor) [") + std::to_string(distance_airfield_vybor) + xorstr("m]")).c_str());
		draw_text_white(novodmitrovsk_screen_position.x, novodmitrovsk_screen_position.y, (xorstr("Novodmitrovsk [") + std::to_string(distance_novodmitrovsk) + xorstr("m]")).c_str());


		draw_text_white(zelenogorsk_screen_position.x, zelenogorsk_screen_position.y, (xorstr("Zelenogorsk[") + std::to_string(distance_zelenogorsk) + xorstr("m]")).c_str());
		draw_text_white(krasnostav_screen_position.x, krasnostav_screen_position.y, (xorstr("Krasnostav [") + std::to_string(distance_krasnostav) + xorstr("m]")).c_str());
		draw_text_white(severograd_screen_position.x, severograd_screen_position.y, (xorstr("Severograd [") + std::to_string(distance_severograd) + xorstr("m]")).c_str());
		draw_text_white(stariy_sobor_screen_position.x, stariy_sobor_screen_position.y, (xorstr("Stariy Sobor [") + std::to_string(distance_stariy_sobor) + xorstr("m]")).c_str());



		draw_text_white(military_base_pavlovo_screen_position.x, military_base_pavlovo_screen_position.y, (xorstr("Military base (Pavlovo)[") + std::to_string(distance_military_base_pavlovo) + xorstr("m]")).c_str());
		draw_text_white(military_base_kabanino_screen_position.x, military_base_kabanino_screen_position.y, (xorstr("Military base (Kabanino) [") + std::to_string(distance_military_base_kabanino) + xorstr("m]")).c_str());

	}
	for (int i = 0; i < entities.size(); i++)
	{
		auto Entities = entities[i];
		Vector3 player_world_position = Game::GetCoordinate(Entities.EntityPtr);
		Vector3 player_screen_position;
		Vector3 tempFeetR, tempFeetL;

		if (Game::WorldToScreen(player_world_position, player_screen_position)) {
			int distance = Game::GetDistanceToMe(player_world_position);
			if (player_screen_position.z < 1.0f) continue;
			if (TargetPlayer != NULL)
			{
				Vector3 targetplayer_world_position = Game::GetCoordinate(TargetPlayer);
				Vector3 targetplayer_screen_position;
				if (Game::WorldToScreen(targetplayer_world_position, targetplayer_screen_position))
				{

					
				}
			}
			Vector3 m_vecEntHeadPos = Game::GetHeadPosition();
			Vector3 m_vecAimSpot = m_vecEntHeadPos;

			float Entity_h = player_screen_position.y - m_vecAimSpot.y;
			float Entity_w = Entity_h / 2;
			float Entity_X = player_screen_position.x - Entity_w / 2;
			float Entity_Y = m_vecAimSpot.y;

			if (player_screen_position.z >= 1.0f)
			{
				std::string TypeName = Game::GetEntityTypeName(Entities.EntityPtr);
				std::string Name = Game::GetEntityName(Entities.EntityPtr);

				
				if (distance > 1)
				{
					if (settings.PlayerEsp)
					{
						if (TypeName == xorstr("dayzplayer") && distance <= settings.DistancePlayer)
						{

							draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("Player [") + std::to_string(distance) + xorstr("m]")).c_str());
							draw_text_white(player_screen_position.x, player_screen_position.y + 40, (Game::GetItemInHands(Entities.EntityPtr)).c_str());

							float centerX = globals.width / 2.0f;
							float centerY = globals.height / 2.0f;
							if (player_screen_position.x - centerX < radius && centerX - player_screen_position.x < radius && centerY - player_screen_position.y < radius && player_screen_position.y - centerY < radius)
							{
								TargetPlayer = Entities.EntityPtr;
							}
						}
					}

					if (settings.ZombieEsp)
					{
						 if (TypeName == xorstr("dayzinfected") && distance <= settings.DistanceZombie)
						 {
							
							 draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("Zombie [") + std::to_string(distance) + xorstr("m]")).c_str());
						 }
					}
					
					if (settings.Car_Esp)
					{
						

						if (TypeName == xorstr("car"))
						{
							if (Name == xorstr("Niva2329"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("Niva2329 [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("UAZ469"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("UAZ469 [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("URAL_375_1964"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("URAL_375_1964 [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("Hatchback_02"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("Hatchback_02 [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("OffroadHatchback"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("OffroadHatchback [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("CivilianSedan"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("CivilianSedan [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("Sedan_02"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("Sedan_02 [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
							if (Name == xorstr("Truck_01_Covered"))
							{
								draw_text_white(player_screen_position.x, player_screen_position.y + 25, (xorstr("Truck_01_Covered [") + std::to_string(distance) + xorstr("m]")).c_str());
							}
						}
					}
				}
			}
		}
	}
}