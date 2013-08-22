#include "DXUT.h"
#include "Ball.h"
#include "Game.h"

Ball::Ball(Game& game) : GameComponent(game), ballMesh(0)
{
}


Ball::~Ball(void)
{
	ballMesh->Release();
}


void Ball::update(double fTime, float fElapsedTime, void* pUserContext)
{
	//static float impulse = 3.0f;

	if (game.elapsed <= 0.0f)
	{
		btTransform startTransform;
		startTransform.setIdentity();
		//startTransform.setOrigin(btVector3(0, 0.5f, -5));
		//startTransform.setOrigin(btVector3(0,5,1));
		btSphereShape* ballShape = new btSphereShape(0.3f);

		ball = game.localCreateRigidBody(0.2f, startTransform, ballShape);
		/*ball->applyCentralImpulse(btVector3(1.0f - (std::rand() % 3), 1, 5));*/
		//ball->applyCentralImpulse(btVector3(0.0f, 1, impulse));
		ball->setRestitution(0.5f);

		//impulse += 1.0f;
	}
}


void Ball::draw(IDirect3DDevice9* device, double fTime, float fElapsedTime, void* pUserContext)
{
	if(ballMesh == 0)
		D3DXCreateSphere(device, 0.3f, 20, 20, &ballMesh, 0);

	device->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	device->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	btTransform ballTransform = ball->getWorldTransform();
	btScalar m[16];
	ballTransform.getOpenGLMatrix(m);

	D3DXMATRIX transform(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	device->SetTransform(D3DTS_WORLD, &transform);

	D3DMATERIAL9 mat;

	ZeroMemory(&mat, sizeof(mat));

	mat.Ambient.r = mat.Ambient.g = mat.Ambient.b = mat.Ambient.a = 0.8f;
	device->SetRenderState(D3DRS_LIGHTING, true);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255,255,255));
	device->SetMaterial(&mat);
	
	ballMesh->DrawSubset(0);
}