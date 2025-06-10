#pragma once

class SAsteroidGameData;
class CAsteroidManager;
class CBulletManager;

class CGameManager
{
public:
public:

    void Init();
    void Update(float InDeltaTime);
    void Render();
    void Shutdown();

    inline SAsteroidGameData* GetGameData() const { return MyGameData; }
    inline CAsteroidManager* GetAsteroidManager() const { return MyAsteroidManager; }
    inline CBulletManager* GetBulletManager() const { return MyBulletManager; }

public:

    // Make sure deletions are visible everywhere, hence public

    CGameManager(const CGameManager&) = delete;				// Copy construct
    CGameManager(CGameManager&&) = delete;					// Move construct
    CGameManager& operator=(const CGameManager&) = delete;	// Copy assign
    CGameManager& operator=(CGameManager&&) = delete;			// Move assign

    //protected:

    CGameManager();
    ~CGameManager() {}

private:

    SAsteroidGameData* MyGameData;
    CAsteroidManager* MyAsteroidManager;
    CBulletManager* MyBulletManager;
    
};
