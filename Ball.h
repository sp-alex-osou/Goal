#pragma once

#include "gamecomponent.h"
#include <btBulletDynamicsCommon.h>

class Ball :
	public GameComponent
{
public:
	Ball(Game& game);
	virtual ~Ball(void);

	void update(double fTime, float fElapsedTime, void* pUserContext);
	void draw(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext);
private:
	LPD3DXMESH ballMesh;
	btRigidBody* ball;
};

