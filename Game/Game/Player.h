#pragma once

#include <vector>

#include "../Framework/Actor.h"
#include "../Framework/Vector2D.h"

// Forwards
class CAsteroidGame;
class CInputComponent;

class CPlayer : public CActor
{
    DECLARE_CLASS(CPlayer, CActor)
public:
    CPlayer();
    virtual ~CPlayer();

    /////////////////////////////////////////////////////////////////////////
    // Game object overrides

    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Render() override;
    virtual void Shutdown() override;

    //////////////////////////////////////////////////////////////////////////
    // Player state

    enum EPlayerState
    {
        None,
        Respawn,
        Alive,
        Dead
    };

    /////////////////////////////////////////////////////////////////////////
    // Read/Write data

    /////////////////////////////////////////////////////////////////////////
    // Transform
    inline SVector2D GetPosition() const { return Position; }
    inline void SetPosition(SVector2D InVec) { Position = InVec; }

    void SetHeading(SVector2D InVec);

    /////////////////////////////////////////////////////////////////////////
    // Physics
    inline SVector2D GetVelocity() const { return Velocity; }
    inline void SetVelocity(const SVector2D& InVel) { Velocity = InVel; }

    inline SVector2D GetHeading() const { return Heading; }
    inline SVector2D GetSide() const { return Side; }

    inline float GetMaxSpeed() const { return MaxSpeed; }
    inline void SetMaxSpeed(float InVal) { MaxSpeed = InVal; }

    inline float GetAcceleration() const { return PlayerAcceleration; }
    inline void SetAcceleration(float InVal) { PlayerAcceleration = InVal; }

    inline float GetDeceleration() const { return PlayerDeceleration; }
    inline void SetDeceleration(float InVal) { PlayerDeceleration = InVal; }

    inline float GetDragFactor() const { return PlayerDragFactor; }
    inline void SetDragFactor(float InVal) { PlayerDragFactor = InVal; }

    inline float GetSpeed() const { return Velocity.GetLength(); }
    inline float GetSpeedSq() const { return  Velocity.GetLengthSq(); }

    inline float GetMaxTurnRate() const { return MaxTurnRate; }
    inline void SetMaxTurnRate(float InVal) { MaxTurnRate = InVal; }

    /////////////////////////////////////////////////////////////////////////
    // Rendering
    // Returns the transformed vertices
    inline std::vector<SVector2D> GetObjectVertices() const { return PlayerVBTransformed; }

    /////////////////////////////////////////////////////////////////////////
    // Misc
    bool RotateHeadingToFacePosition(SVector2D InTarget);
    
    inline void SetIntersectsAsteroid(bool bVal) { bIntersectsAsteroid = bVal; }

    inline bool IsAlive() const { return bAlive; }

    void SetCanRespawn(bool bVal) { bCanRespawn = bVal; }

    void SetGameOwner(CAsteroidGame* InGame) { GameOwner = InGame; }

    const CInputComponent* GetInputComponent() const { return InputComp; }

private:

    /////////////////////////////////////////////////////////////////////////
    // Helper methods

    void DefaultInitialization();
    void RespawnInitialization();
	
    void CleanupVBs();
    void CreatePlayerVBs();
    void CreateThrusterVBs();

    void CreateBullet();

    void RenderPlayer();
    void RenderThruster();

    /////////////////////////////////////////////////////////////////////////
    // State transitions

    void SetState(EPlayerState InState);
    void RevertToPrevState();

private:

    /////////////////////////////////////////////////////////////////////////
    // Transform

    SVector2D Position;
    SVector2D Heading;
    SVector2D Side;
    SVector2D Scale;

    /////////////////////////////////////////////////////////////////////////
    // Physics

    SVector2D Velocity;
    float MaxSpeed;
    float PlayerAcceleration;
    float PlayerDeceleration;
    float PlayerDragFactor;
    float MaxTurnRate;

    /////////////////////////////////////////////////////////////////////////
    // Rendering

    // Vertex buffer
    std::vector<SVector2D> PlayerVB;
    std::vector<SVector2D> PlayerVBTransformed;

    // Thruster VBs
    std::vector<SVector2D> ThrusterVB;
    std::vector<SVector2D> ThrusterVBTransformed;

    /////////////////////////////////////////////////////////////////////////
    // Input
    CInputComponent* InputComp;

    /////////////////////////////////////////////////////////////////////////
    // Player specific

    EPlayerState CurrentState;
    EPlayerState PreviousState;

    bool bCanBeDamaged;
    bool bAlive;
    bool bCanRespawn;

    float DamageImmuneDuration;
    float CurrentDamageImmuneTime;

    int CurrentFlickerCounter;

    float RespawnDelayDuration;
    float CurrentRespawnDelayTime;

    bool bInitialized;

    bool bIntersectsAsteroid;

    // Firing
    float FireRate;
    float CurrentTimeBetweenShots;

    std::string BulletFireSoundName;

    CAsteroidGame* GameOwner;
};
