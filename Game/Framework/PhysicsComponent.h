#pragma once

#include "ActorComponent.h"

#include "Vector2D.h"

class CPhysicsComponent : public CActorComponent
{
    DECLARE_CLASS(CPhysicsComponent, CActorComponent)
public:
    CPhysicsComponent();
    virtual ~CPhysicsComponent();

    // Overrides
    //virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    //virtual void Render() override;
    //virtual void Shutdown() override;

    // Getters/Setters
    inline SVector2D GetVelocity() const { return Velocity; }
    inline void SetVelocity(const SVector2D& InVel) { Velocity = InVel; }

    inline float GetSpeed() const { return Velocity.GetLength(); }
    inline float GetSpeedSq() const { return Velocity.GetLength(); }

    inline float GetMaxSpeed() const { return MaxSpeed; }
    inline void SetMaxSpeed(const float InVal) { MaxSpeed = InVal; }

    inline float GetAcceleration() const { return Acceleration; }
    inline void SetAcceleration(const float InVal) { Acceleration = InVal; }

    inline float GetDeceleration() const { return Deceleration; }
    inline void SetDeceleration(const float InVal) { Deceleration = InVal; }

    inline float GetDragFactor() const { return DragFactor; }
    inline void SetDragFactor(const float InVal)
    {
        if (InVal > 1.0f)
        {
            DragFactor = 1.0f;
        }
        else if (InVal < 0.0f)
        {
            DragFactor = 0.0f;
        }
        else
        {
            DragFactor = InVal;
        }
    }

    inline float GetMaxTurnRate() const { return MaxTurnRate; }
    inline void SetMaxTurnRate(const float InVal) { MaxTurnRate = InVal; }

    bool IsAccelerating() const { return bAccelerating; }
    inline void SetAccelerating(const bool bValue)
    {
        if (bAccelerating != bValue)
        {
            bAccelerating = bValue;
        }
    }

    bool IsDecelerating() const { return bDecelerating; }
    inline void SetDecelerating(const bool bValue)
    {
        if (bDecelerating != bValue)
        {
            bDecelerating = bValue;
        }
    }

protected:
    SVector2D Velocity;
    float MaxSpeed;
    float Acceleration;
    float Deceleration;
    float DragFactor;
    float MaxTurnRate;

    uint8_t bAccelerating : 1;
    uint8_t bDecelerating : 1;
    
};
