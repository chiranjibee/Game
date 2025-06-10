#include "GameManager.h"

#include "AsteroidManager.h"
#include "BulletManager.h"
#include "GameData.h"
#include "GameUtil.h"

CGameManager::CGameManager()
{
    MyGameData = new SAsteroidGameData();
    MyAsteroidManager = new CAsteroidManager();
    MyBulletManager = new CBulletManager();
}

void CGameManager::Init()
{
}

void CGameManager::Update(float InDeltaTime)
{
    MyAsteroidManager->Update(InDeltaTime);
    MyBulletManager->Update(InDeltaTime);
}

void CGameManager::Render()
{
    MyAsteroidManager->Render();
    MyBulletManager->Render();
}

void CGameManager::Shutdown()
{
    MyAsteroidManager->Shutdown();
    MyBulletManager->Shutdown();

    SafeDelete(&MyGameData);
    SafeDelete(&MyAsteroidManager);
    SafeDelete(&MyBulletManager);
}
