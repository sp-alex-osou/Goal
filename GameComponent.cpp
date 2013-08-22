#include "DXUT.h"
#include "GameComponent.h"
#include "Game.h"

GameComponent::GameComponent(Game& game) : game(game), camera(game.camera)
{
}

GameComponent::~GameComponent()
{
}