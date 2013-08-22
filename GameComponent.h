#pragma once

#include "DXUT.h"
#include "DXUTcamera.h"

#define D3DFVF_VertexPositionColor (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class Game;

class GameComponent
{
public:
	struct VertexPositionColor
	{
		FLOAT x, y, z;
		DWORD color;
	};

	GameComponent(Game& game);
	virtual ~GameComponent(void) = 0;

	virtual void init() {}
	virtual void update(double fTime, float fElapsedTime, void* pUserContext) {}
	virtual void draw(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext) {}

	GameComponent& operator=(const GameComponent&) {}
protected:
	Game& game;
	CFirstPersonCamera& camera;
};