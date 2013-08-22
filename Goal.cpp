#include "DXUT.h"
#include "Goal.h"
#include "Game.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

#include "Output.h"

#include <cmath>

Goal::Goal(Game& game) : GameComponent(game)
{
}


Goal::~Goal(void)
{
}


void Goal::init()
{
	int resx = 55;
	int resy = resx / 2;

	float w = 5.0f / 2;
	float h = 2.5f;
	float d = 2.0f;

	btVector3 cornerLBF(-w, 0, 0);
	btVector3 cornerRBF(w, 0, 0);
	btVector3 cornerLBB(-w, 0, d);
	btVector3 cornerRBB(w, 0, d);
	btVector3 cornerLTF(-w, h, 0);
	btVector3 cornerRTF(w, h, 0);
	btVector3 cornerLTB(-w, h, d);
	btVector3 cornerRTB(w, h, d);

	nets.clear();
	nets.push_back(createNet(cornerLTB, cornerRTB, cornerLBB, cornerRBB, resx, resy));
	nets.push_back(createNet(cornerLTB, cornerRTB, cornerLTF, cornerRTF, resx, resy / 2));
	nets.push_back(createNet(cornerRTB, cornerRTF, cornerRBB, cornerRBF, resx / 2, resy));
	nets.push_back(createNet(cornerLTF, cornerLTB, cornerLBF, cornerLBB, resx / 2, resy));

	std::vector<btSoftBody::Node*> nodes;

	for (int i = 0; i < nets[0]->m_nodes.size(); ++i)
		if (nets[0]->m_nodes[i].m_x.getX() == 0.0f)
			nodes.push_back(&nets[0]->m_nodes[i]);

	node = nodes[nodes.size() / 2];

	for (std::vector<btSoftBody*>::iterator it = nets.begin(); it != nets.end(); ++it)
		game.getDynamicsWorld()->addSoftBody(*it);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0,0,0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btRigidBody* ground = game.localCreateRigidBody(0.f, startTransform, groundShape);
	ground->setRestitution(1.0f);
}


btSoftBody* Goal::createNet(const btVector3& corner00, const btVector3& corner01, const btVector3& corner10, const btVector3& corner11, int resx, int resy)
{
	btSoftBodyWorldInfo& worldInfo = game.getDynamicsWorld()->getWorldInfo();

	btSoftBody* net = btSoftBodyHelpers::CreatePatch(worldInfo, corner00, corner01, corner10, corner11, resx, resy, 0, false);
	net->generateBendingConstraints(8);
	net->setTotalMass(10.0f);

	for (int i = 0; i < resx; ++i)
	{
		net->setMass(i, 0.f);
		net->setMass(net->m_nodes.size() - 1 - i, 0.f);
	}

	for (int j = 0; j < resy; ++j)
	{
		net->setMass(j * resx, 0.f);
		net->setMass((j+1) * resx - 1, 0.f);
	}

	return net;
}


void Goal::update(double fTime, float fElapsedTime, void* pUserContext)
{
	/*static bool output = false;

	if (game.elapsed <= 0.0f && data.size() <= 3)
	{
		for (std::vector<btSoftBody*>::iterator it = nets.begin(); it != nets.end(); ++it)
			game.getDynamicsWorld()->removeSoftBody(*it);

		init();

		data.push_back(std::vector<float>());
	}

	if (data.size() <= 3)
	{
		btSoftBody* net = nets[0];

		float z = 0.0f;

		for (int i = 0; i < net->m_nodes.size(); ++i)
			z += net->m_nodes[i].m_x.getZ();

		z /= net->m_nodes.size();

		data.rbegin()->push_back(z);
	}
	else if (!output)
	{
		Output::open();

		int size = std::min<unsigned>(std::min<unsigned>(data[0].size(), data[1].size()), data[2].size());
		
		for (int i = 0; i < size; ++i)
			Output::print(i * (20.0f / size), data[0][i], data[1][i], data[2][i]);

		Output::close();

		output = true;
	}*/
}


void Goal::draw(IDirect3DDevice9* device, double fTime, float fElapsedTime, void* pUserContext)
{
	device->SetFVF(D3DFVF_VertexPositionColor);

	D3DXMATRIX identity;

	D3DXMatrixIdentity(&identity);

	device->SetTransform(D3DTS_WORLD, &identity);
	device->SetTransform(D3DTS_VIEW, camera.GetViewMatrix());
	device->SetTransform(D3DTS_PROJECTION, camera.GetProjMatrix());

	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	VertexPositionColor vertices[3];

	for (std::vector<btSoftBody*>::iterator it = nets.begin(); it != nets.end(); ++it)
	{
		btSoftBody* net = *it;

		for (int i = 0; i < net->m_faces.size(); ++i)
		{
			btSoftBody::Face face = net->m_faces[i];

			for (int j = 0; j < 3; ++j)
			{
				btVector3 v = face.m_n[j]->m_x;

				vertices[j].x = v.getX();
				vertices[j].y = v.getY();
				vertices[j].z = v.getZ();
				vertices[j].color = D3DCOLOR_XRGB(0, 128, 0);
			}

			device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, vertices, sizeof(VertexPositionColor));
		}
	}

	VertexPositionColor groundVertices[] =
	{
		{ -100, 0, -100, D3DCOLOR_XRGB(0, 50, 0) },
		{ -100, 0, 100, D3DCOLOR_XRGB(0, 50, 0) },
		{ 100, 0, -100, D3DCOLOR_XRGB(0, 50, 0) },
		{ 100, 0, 100, D3DCOLOR_XRGB(0, 50, 0) }
	};

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, groundVertices, sizeof(VertexPositionColor));

	LPD3DXLINE post;

	D3DXCreateLine(device, &post);

	float w = 5.0f / 2;
	float h = 2.5f;
	float d = 2.0f;

	D3DXVECTOR3 vertices0[] = 
	{
		D3DXVECTOR3(-w, 0, d),
		D3DXVECTOR3(-w, 0, 0),
		D3DXVECTOR3(-w, h, 0),
		D3DXVECTOR3(-w, h, d),
		D3DXVECTOR3(-w, 0, d),
		D3DXVECTOR3(w, 0, d),
		D3DXVECTOR3(w, 0, 0),
		D3DXVECTOR3(w, h, 0),
		D3DXVECTOR3(w, h, d),
		D3DXVECTOR3(w, 0, d)
	};

	D3DXVECTOR3 vertices1[] = 
	{
		D3DXVECTOR3(-w, h, 0),
		D3DXVECTOR3(-w, h, d),
		D3DXVECTOR3(w, h, d),
		D3DXVECTOR3(w, h, 0),
		D3DXVECTOR3(-w, h, 0)
	};

	D3DXMATRIX m = *camera.GetViewMatrix() * *camera.GetProjMatrix();

	post->SetWidth(5);
	post->DrawTransform(vertices0, 10, &m, D3DCOLOR_XRGB(255, 255, 255));
	post->DrawTransform(vertices1, 5, &m, D3DCOLOR_XRGB(255, 255, 255));

	post->Release();
}