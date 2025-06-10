#pragma once

#include "../App/App.h"

class SAsteroidGameData
{
public:
    SAsteroidGameData();
    
    unsigned int AsteroidPoolSize;
    unsigned int BulletPoolSize;

    float PlayerStartPositionX;
    float PlayerStartPositionY;
    float PlayerMaxSpeed;
    float PlayerAcceleration;
    float PlayerDeceleration;
    float PlayerDragFactor;
    float PlayerMaxTurnRate;
    float PlayerDamageImmuneDuration;
    float PlayerRespawnDelayDuration;
    float PlayerBulletFireRate;
    unsigned int PlayerFlickerCounter;

    float AsteroidMaxSpeed;
    float AsteroidGenMinRadius;
    float AsteroidGenMaxRadius;
    float AsteroidGenGranularity;
    float AsteroidGenMinVariance;
    float AsteroidGenMaxVariance;

    float BulletMaxSpeed;
    float BulletLifetime;

    unsigned int DefaultPlayerLives;
    unsigned int FirstWaveAsteroidCount;
    unsigned int MinAsteroidCountForRespawn;
    unsigned int ScorePerSecond;
    unsigned int ScorePerWaveClear;
    unsigned int ScorePerLargeAsteroid;
    unsigned int ScorePerMediumAsteroid;
    unsigned int ScorePerSmallAsteroid;
    float GameOverDelayDuration;
};

inline SAsteroidGameData::SAsteroidGameData()
{
    AsteroidPoolSize = 40;
    BulletPoolSize = 100;

    PlayerStartPositionX = APP_VIRTUAL_WIDTH * 0.5f;
    PlayerStartPositionY = APP_VIRTUAL_HEIGHT * 0.5f;
    PlayerMaxSpeed = 5.0f;
    PlayerAcceleration = 2.0f;
    PlayerDeceleration = 0.5f;
    PlayerDragFactor = 0.07f;
    PlayerMaxTurnRate = 0.1f;
    PlayerDamageImmuneDuration = 2.0f;
    PlayerRespawnDelayDuration = 2.0f;
    PlayerBulletFireRate = 1.0f;
    PlayerFlickerCounter = 11;

    AsteroidMaxSpeed = 4.0f;
    AsteroidGenMinRadius = 30.0f;
    AsteroidGenMaxRadius = 50.0f;
    AsteroidGenGranularity = 16.0f;
    AsteroidGenMinVariance = 0.25f;
    AsteroidGenMaxVariance = 0.75f;

    BulletMaxSpeed = 8.0f;
    BulletLifetime = 2.0f;

    DefaultPlayerLives = 3;
    FirstWaveAsteroidCount = 3;
    MinAsteroidCountForRespawn = 0;
    ScorePerSecond = 1;
    ScorePerWaveClear = 250;
    ScorePerLargeAsteroid = 5;
    ScorePerMediumAsteroid = 15;
    ScorePerSmallAsteroid = 30;
    GameOverDelayDuration = 4.0f;
}
