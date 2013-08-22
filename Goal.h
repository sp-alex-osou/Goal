#pragma once

#include "GameComponent.h"

#include <vector>
#include <btSoftBody.h>

class Goal : public GameComponent
{
public:
	Goal(Game& game);
	virtual ~Goal(void);

	void init();
	void update(double fTime, float fElapsedTime, void* pUserContext);
	void draw(IDirect3DDevice9* device, double fTime, float fElapsedTime, void* pUserContext);
	btSoftBody* createNet(const btVector3& corner00, const btVector3& corner01, const btVector3& corner10, const btVector3& corner11, int resx, int resy);
private:
	std::vector<btSoftBody*> nets;
	btSoftBody::Node* node;

	std::vector<std::vector<float>> data;
};

