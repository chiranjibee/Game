#pragma once

#include "Bullet.h"

class CBulletManager
{
public:
    CBulletManager();

    void SpawnBullet(SVector2D InPos, SVector2D InHead, SVector2D InVel);

    void Update(float InDeltaTime);
    void Render();
    void Cleanup();
    void Shutdown();

    inline std::vector<CBullet*> GetBullets() const { return MyBullets; }

private:

    std::vector<CBullet*> MyBullets;
    
};
