#include "DXUT.h"
#include "Game.h"

Game::Game(void)
{
	D3DXVECTOR3 eye = D3DXVECTOR3(0, 1, -11);
	D3DXVECTOR3 lookat = D3DXVECTOR3(0, 1.0f, 0);

	elapsed = 0.0f;

	camera.SetViewParams(&eye, &lookat);

	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-10,0));

	gameComponents.push_back(new Goal(*this));
	gameComponents.push_back(new Ball(*this));

	for(list<GameComponent*>::iterator it = gameComponents.begin(); it != gameComponents.end(); ++it)
	{
		(*it)->init();
	}
}


Game::~Game(void)
{
	delete dynamicsWorld;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;

	for(list<GameComponent*>::iterator it = gameComponents.begin(); it != gameComponents.end(); ++it)
	{
		delete *it;
	}
}


void Game::handleMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
	camera.HandleMessages(hWnd, uMsg, wParam, lParam);
}


void Game::init(IDirect3DDevice9* device, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	camera.SetProjParams(D3DX_PI / 4, pBackBufferSurfaceDesc->Width / (float)pBackBufferSurfaceDesc->Height, 0.1f, 1000.0f);
}


void Game::udpate(double fTime, float fElapsedTime, void* pUserContext)
{
	for(list<GameComponent*>::iterator it = gameComponents.begin(); it != gameComponents.end(); ++it)
	{
		(*it)->update(fTime, fElapsedTime, pUserContext);
	}

	camera.FrameMove(fElapsedTime);
	dynamicsWorld->stepSimulation(fElapsedTime);

	if (elapsed <= 0.0f)
		elapsed += 20.0f;

	elapsed -= fElapsedTime;
}


void Game::draw(IDirect3DDevice9* device, double fTime, float fElapsedTime, void* pUserContext)
{
	for(list<GameComponent*>::iterator it = gameComponents.begin(); it != gameComponents.end(); ++it)
	{
		(*it)->draw(device, fTime, fElapsedTime, pUserContext);
	}
}


btRigidBody* Game::localCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);

	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);

	dynamicsWorld->addRigidBody(body);

	return body;
}