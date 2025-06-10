#include "BulletManager.h"

#include "GameUtil.h"

static constexpr unsigned int MAX_BULLET_POOL = 100;

CBulletManager::CBulletManager()
{
    MyBullets.reserve(MAX_BULLET_POOL);

    for (unsigned int i = 0; i < MAX_BULLET_POOL; ++i)
    {
        CBullet* NewBullet = new CBullet();
        MyBullets.emplace_back(NewBullet);
    }
}

void CBulletManager::SpawnBullet(SVector2D InPos, SVector2D InHead, SVector2D InVel)
{
    for (const auto& InBullet : MyBullets)
    {
        if (!InBullet->IsAlive())
        {
            InBullet->Init();
            InBullet->SetPosition(InPos);
            InBullet->SetHeading(InHead);
            InBullet->SetVelocity(InVel);
            return;
        }
    }
}

void CBulletManager::Update(float InDeltaTime)
{
    for (const auto& InBullet : MyBullets)
    {
        if (InBullet->IsAlive())
        {
            InBullet->Update(InDeltaTime);
        }
    }
}

void CBulletManager::Render()
{
    for (const auto& InBullet : MyBullets)
    {
        if (InBullet->IsAlive())
        {
            InBullet->Render();
        }
    }
}

void CBulletManager::Cleanup()
{
    for (const auto& InBullet : MyBullets)
    {
        InBullet->Shutdown();
    }
}

void CBulletManager::Shutdown()
{
    if (MyBullets.empty()) return;

    for (auto& InBullet : MyBullets)
    {
        InBullet->Shutdown();
        SafeDelete(&InBullet);
    }

    // Erase remove idiom
    //MyBullets.erase(std::remove_if(std::begin(MyBullets), std::end(MyBullets), [](Bullet* InBullet) { return !InBullet->IsAlive(); }), std::end(MyBullets));
}
