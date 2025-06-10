#include "AsteroidManager.h"

#include "GameUtil.h"

static constexpr unsigned int MAX_ASTEROID_POOL = 40;

CAsteroidManager::CAsteroidManager()
{
    MyAsteroids.reserve(MAX_ASTEROID_POOL);

    for (unsigned int i = 0; i < MAX_ASTEROID_POOL; ++i)
    {
        CAsteroid* NewAsteroid = new CAsteroid();
        MyAsteroids.emplace_back(NewAsteroid);
    }
}

void CAsteroidManager::SpawnAsteroid(EAsteroidType InType, SVector2D InPos, SVector2D InHead, SVector2D InVel)
{
    for (const auto& InAsteroid : MyAsteroids)
    {
        if (!InAsteroid->IsAlive())
        {
            InAsteroid->Init();
            InAsteroid->SetAsteroidType(InType);
            InAsteroid->SetPosition(InPos);
            InAsteroid->SetHeading(InHead);
            InAsteroid->SetVelocity(InVel);
            return;
        }
    }
}

void CAsteroidManager::Update(float InDeltaTime)
{
    for (const auto& InAsteroid : MyAsteroids)
    {
        if (InAsteroid->IsAlive())
        {
            InAsteroid->Update(InDeltaTime);
        }
    }
}

void CAsteroidManager::Render()
{
    for (const auto& InAsteroid : MyAsteroids)
    {
        if (InAsteroid->IsAlive())
        {
            InAsteroid->Render();
        }
    }
}

void CAsteroidManager::Cleanup()
{
    for (const auto& InAsteroid : MyAsteroids)
    {
        InAsteroid->Shutdown();
    }
}

void CAsteroidManager::Shutdown()
{
    if (MyAsteroids.empty()) return;

    for (auto& InAsteroid : MyAsteroids)
    {
        InAsteroid->Shutdown();
        SafeDelete(&InAsteroid);
    }
}
