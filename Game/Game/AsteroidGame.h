#pragma once

#include "Player.h"
#include "Asteroid.h"

class CGameManager;

class CAsteroidGame
{
public:
    CAsteroidGame();
    ~CAsteroidGame();

    void Init();
    void Update(float deltaTime);
    void Render();
    void Shutdown();

    CGameManager* GetGameManager() const { return GameManager; }

    //////////////////////////////////////////////////////////////////////////
    // Game state

    enum EGameState
    {
        None,
        MainMenu,
        HelpMenu,
        GameRun,
        Paused,
        GameOver
    };

private:

    /////////////////////////////////////////////////////////////////////////
    // Helper methods

    void DefaultInitialization();
    void RestartInitialization();
	
    void CleanupGame();
    void CreatePlayer();
	
    void SpawnDefaultAsteroidWave();
    void SpawnChildAsteroidWave(EAsteroidType InPrevType, SVector2D InPrevPos, SVector2D InPrevHead, SVector2D InPrevVel);
    //void SpawnSubsequentAsteroidWave();
	
    void CheckPlayerAsteroidCollision();
    void CheckBulletAsteroidCollision();

    void AddPlayerScore_KillAsteroid(EAsteroidType InType);
    void AddPlayerScore_WaveClear();

    void CheckForAsteroidWaveSpawn();

    //inline int GetPlayerLives() const { return CurrentPlayerLives; }
    inline void AddPlayerLife() { CurrentPlayerLives += 1; }
    inline void RemovePlayerLife() { CurrentPlayerLives -= 1; }

    /////////////////////////////////////////////////////////////////////////
    // State transitions

    void SetState(EGameState InState);
    void RevertToPrevState();

private:

    EGameState CurrentState;
    EGameState PreviousState;

    bool bGamePaused;
    bool bInitialized;

    /////////////////////////////////////////////////////////////////////////
    // Game specific

    CPlayer* Player;

    unsigned int DefaultPlayerLives;
    unsigned int CurrentPlayerLives;

    float GameOverDelayDuration;
    float CurrentGameOverDelayTime;

    unsigned int MinAsteroidCountForRespawn;
    unsigned int CurrentAliveAsteroidCount;

    unsigned int ScorePerSecond;
    unsigned int ScorePerWaveClear;
    unsigned int ScorePerLargeAsteroid;
    unsigned int ScorePerMediumAsteroid;
    unsigned int ScorePerSmallAsteroid;
    unsigned int CurrentPlayerScore;
    float CurrentTimeForScoring;

    std::string GameAmbientSoundName;
    std::string PlayerExplosionSoundName;
    std::string WaveClearSoundName;
    std::string AsteroidExplosionSoundName;

    CGameManager* GameManager;
    
};
