#pragma once

#include "Asteroid.h"

class CAsteroidManager
{
public:
    CAsteroidManager();

    void SpawnAsteroid(EAsteroidType InType, SVector2D InPos, SVector2D InHead, SVector2D InVel);

    void Update(float InDeltaTime);
    void Render();
    void Cleanup();
    void Shutdown();

    inline std::vector<CAsteroid*> GetAsteroids() const { return MyAsteroids; }

private:

    std::vector<CAsteroid*> MyAsteroids;
    
};
