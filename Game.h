#pragma once

#include <list>

#include <DXUTcamera.h>
#include <btBulletDynamicsCommon.h>
#include <btSoftRigidDynamicsWorld.h>
#include <btSoftBodyHelpers.h>
#include <btSoftBody.h>
#include <btSoftBodyRigidBodyCollisionConfiguration.h>

#include "GameComponent.h"
#include "Goal.h"
#include "Ball.h"

using std::list;

class Game
{
	friend GameComponent;

public:
	Game(void);
	virtual ~Game(void);

	void init(IDirect3DDevice9* device, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);
	void handleMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );

	void udpate(double fTime, float fElapsedTime, void* pUserContext);
	void draw(IDirect3DDevice9* device, double fTime, float fElapsedTime, void* pUserContext);

	btSoftRigidDynamicsWorld* getDynamicsWorld() { return dynamicsWorld; }

	btRigidBody* Game::localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

	float elapsed;
private:
	list<GameComponent*> gameComponents;

	CFirstPersonCamera camera;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	btSoftRigidDynamicsWorld* dynamicsWorld;
};

