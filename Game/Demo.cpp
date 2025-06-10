#include "App/App.h"

#include "Game/AsteroidGame.h"

CAsteroidGame* MyGame = new CAsteroidGame();

void Init()
{
	MyGame->Init();
}

void Update(float DeltaTime)
{
	MyGame->Update(DeltaTime);
}

void Render()
{
	MyGame->Render();
}

void Shutdown()
{
	MyGame->Shutdown();
}