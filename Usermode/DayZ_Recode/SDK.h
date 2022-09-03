
#include "driver.h"
#include "Value.h"
#include <string>
#include "offset.h"
#include "Vector.h"
using namespace std;

namespace Game
{

	std::string read_string(uint64_t address, int size)
	{

		std::string return_str;
		return_str.resize(size);
		DriverAPI.ReadArr((uintptr_t)address, (uintptr_t*)return_str.data(), size * sizeof(char));
		return return_str;
	}

	std::string ReadArmaString(uint64_t address)
	{
		int len = DriverAPI.Read<int>(address + 0x8);
		std::string str = read_string(address + 0x10, len);
		return str;
	}

	void SetTerrainGrid(float value) {
		DriverAPI.Write<float>(globals.World + 0xB80, value);
	}
	

	template <typename T = std::uint64_t>
	bool valid_ptr(T ptr) {
		return (ptr && ptr > (T)0xFFFFFF && ptr < (T)0x7FFFFFFFFFFF);
	}


	std::string GetEntityTypeName(uint64_t Entity)
	{
		return ReadArmaString(DriverAPI.Read<uint64_t>(DriverAPI.Read<uint64_t>(Entity + 0x148) + 0xA0)).c_str(); 
	}

	uint64_t GetNetworkId(uint64_t Entity) {
		return DriverAPI.Read<uint64_t>(Entity + ONetoworkID);
	}


	string GetQuality(uint64_t Entity) {

		auto quality = DriverAPI.Read<uint64_t>(Entity + off_itemquality);

		if (quality == 1) return "Quality (Worn)";
		if (quality == 2) return "Quality(Damaged)";
		if (quality == 3) return "Quality (Badly damaged)";
		if (quality == 4) return "Quality (Ruined)";

		else return "Quality (Pristine)";
	}
	std::string GetEntityName(uint64_t Entity)
	{
		return ReadArmaString(DriverAPI.Read<uint64_t>(DriverAPI.Read<uint64_t>(Entity + 0x148) + 0x68)).c_str(); 
	}

	uint64_t GetInventory(uint64_t Entity) {
		return DriverAPI.Read<uint64_t>(Entity + 0x5F8); 
	}



	


	string GetItemInHands(uint64_t Entity) {
		return ReadArmaString(DriverAPI.Read<uint64_t>(
			DriverAPI.Read<uint64_t>(DriverAPI.Read<uint64_t>(Game::GetInventory(Entity) + 0x1B0) + 0x148) + 0x68)).c_str(); 
	}

	std::string GetEntityItemName(uint64_t Entity)
	{
		return ReadArmaString(DriverAPI.Read<uint64_t>(DriverAPI.Read<uint64_t>(Entity + 0x148) + 0x4E0)).c_str();
	}


	uint64_t NearEntityTable()
	{
		return DriverAPI.Read<uint64_t>(globals.World + oNearEntityTable);
	}

	uint32_t NearEntityTableSize()
	{
		return DriverAPI.Read<uint32_t>(globals.World + oNearEntityTable + 0x08);
	}

	uint64_t GetEntity(uint64_t PlayerList, uint64_t SelectedPlayer)
	{
		return DriverAPI.Read<uint64_t>(PlayerList + SelectedPlayer * 0x8); 
	}

	uint64_t FarEntityTable()
	{
		return DriverAPI.Read<uint64_t>(globals.World + oFarEntityTable);
	}

	uint32_t FarEntityTableSize()
	{
		return DriverAPI.Read<uint32_t>(globals.World + oFarEntityTable + 0x08);
	}

	uint64_t GetCameraOn()
	{
		return DriverAPI.Read<uint64_t>(globals.World + 0x28B8); 
	}

	uint64_t GetLocalPlayer()
	{
		return DriverAPI.Read<uint64_t>(DriverAPI.Read<uint64_t>(globals.World + oLocalPlayer) + 0x8) - 0xA8;
	}

	uint64_t GetLocalPlayerVisualState()
	{
		return DriverAPI.Read<uint64_t>(GetLocalPlayer() + 0x198);
	}

	uint64_t GetPlayerVisualState()
	{
		return DriverAPI.Read<uint64_t>(GetLocalPlayer() + 0x198); 
	}


	Vector3 GetPlayerVisualState(uintptr_t Entity)
	{
		if (Entity)
		{
			uintptr_t renderVisualState = DriverAPI.Read<uintptr_t>(Entity + 0x198); 

			if (renderVisualState)
			{
				Vector3 pos = DriverAPI.Read<Vector3>(renderVisualState + 0x2C);
				return pos;
			}
		}

		return Vector3(-1, -1, -1);
	}

	Vector3 GetCoordinate(uint64_t Entity)
	{
		while (true)
		{
			if (Entity == GetLocalPlayer())
			{
				return Vector3(DriverAPI.Read<Vector3>(GetLocalPlayerVisualState() + 0x2C)); 
			}
			else
			{
				return  Vector3(DriverAPI.Read<Vector3>(DriverAPI.Read<uint64_t>(Entity + 0x198) + 0x2C)); 
			}
		}
	}




#define O_NETWORK_MANAGER 0xE5AB20
#define O_NETWORK_CLIENT 0x50

#define O_NETWORK_SCOREBOARD 0x10
#define O_NETWORK_PLAYERCOUNT 0x18

#define O_SCOREBOARD_SIZE 0x158
#define O_SCOREBOARD_NAME 0xF0









	uint64_t GetCamera()
	{
		while (true)
		{
			return DriverAPI.Read<uint64_t>(globals.World + 0x1B8); 
		}
	}

	Vector3 GetInvertedViewTranslation()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0x2C));
	}

	Vector3 GetInvertedViewRight()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0x8));
	}

	Vector3 GetInvertedViewUp()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0x14)); 
	}

	Vector3 GetInvertedViewForward()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0x20)); 
	}

	Vector3 GetViewportSize()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0x58));
	}

	Vector3 GetProjectionD1()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0xD0)); 
	}

	Vector3 GetProjectionD2()
	{
		return Vector3(DriverAPI.Read<Vector3>(GetCamera() + 0xDC)); 
	}

	uint32_t GetSlowEntityTableSize()
	{
		return DriverAPI.Read<uint32_t>(globals.World + oSlowEntityTable + 0x08);
	}

	float GetDistanceToMe(Vector3 Entity)
	{
		return Entity.Distance(GetCoordinate(GetLocalPlayer()));
	}

	bool WorldToScreen(Vector3 Position, Vector3& output)
	{
		if (!GetCamera()) return false;

		Vector3 temp = Position - GetInvertedViewTranslation();

		float x = temp.Dot(GetInvertedViewRight());
		float y = temp.Dot(GetInvertedViewUp());
		float z = temp.Dot(GetInvertedViewForward());

		if (z < 0.1f)
			return false;

		Vector3 res(
			GetViewportSize().x * (1 + (x / GetProjectionD1().x / z)),
			GetViewportSize().y * (1 - (y / GetProjectionD2().y / z)),
			z);

		output.x = res.x;
		output.y = res.y;
		output.z = res.z;
		return true;
	}
	Vector3 GetHeadPosition()
	{
		return DriverAPI.Read<Vector3>((const DWORD64)+0xF8);

	}

	bool SetPosition(uint64_t Entity, Vector3 TargetPosition)
	{
		if (Entity == Game::GetLocalPlayer()) {
			DriverAPI.Write<Vector3>(
				DriverAPI.Read<uint64_t>(
					Entity + 0xF0) + 0x2C, TargetPosition);
		}
		else {
			DriverAPI.Write<Vector3>(
				DriverAPI.Read<uint64_t>(
					Entity + 0x198) + 0x2C, TargetPosition);
		}
		return true;
	}

	uint64_t BulletTable() {
		return DriverAPI.Read<uint64_t>(globals.World + 0xD70);
	}

	uint64_t BulletTableSize() {
		return sizeof(BulletTable());
	}

	bool SilentAim(uint64_t Entity)
	{
		if (!Entity) return false;
		for (uint64_t playerId = NULL; playerId < BulletTableSize(); ++playerId) {
			Vector3 WorldPosition = Game::GetCoordinate(Entity);
			int distance = Game::GetDistanceToMe(WorldPosition);
			SetPosition(Game::GetEntity(BulletTable(), playerId), Vector3(WorldPosition.x, WorldPosition.y + 1.f, WorldPosition.z));
		}
		return true;
	}

}