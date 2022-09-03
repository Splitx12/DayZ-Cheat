#pragma once
#include "Include.h"
#include "DirectOverlay.h"



enum TextLayout : std::uint32_t
{
	TextLeft = 0x00,
	TextRight = 0x01,
	TextCenterV = 0x02,
	TextCenterH = 0x04,
	TextCenter = (TextCenterV | TextCenterH),
};

class Renderer
{
public:
	static Renderer* GetInstance();
private:
	Renderer();
	~Renderer();
public:
	enum CrosshairStyle
	{
		Dot,
		Plus,
		Cross,
		Gap,
		Diagonal,
		HitMaker,
	};

	void DrawHorBar(int x, int y, int w, int h, float Health, float MaxHealth, D3DXCOLOR color1, D3DXCOLOR color2);
	void DrawVerBar(int x, int y, int w, int h, float Health, float MaxHealth, D3DXCOLOR color1, D3DXCOLOR color2);
	void DrawOutlineBox(int x, int y, int w, int h, D3DXCOLOR color, float thickness = 1.0f);
	void DrawCoalBox(int x, int y, int w, int h, D3DXCOLOR color, float thickness = 1.0f);
	void DrawOutlineCoalBox(int x, int y, int w, int h, D3DXCOLOR color, float thickness = 1.0f);
	void DrawWindow(int x, int y, int w, int h, D3DXCOLOR Border, D3DXCOLOR Background);
	void DrawPlayerName(uint64_t player, std::string buffer, int distance, bool sameTeam);

	void DrawCrosshair(CrosshairStyle style, float x, float y, float size, float stroke, D3DXCOLOR color);
private:
	static Renderer* m_pInstance;
};