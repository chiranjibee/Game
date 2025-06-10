#include "PhysicsComponent.h"

CPhysicsComponent::CPhysicsComponent()
    :   Velocity(SVector2D::Zero()),
        MaxSpeed(0.0f),
        Acceleration(0.0f),
        Deceleration(0.0f),
        DragFactor(0.0f),
        MaxTurnRate(0.0f),
        bAccelerating(false),
        bDecelerating(false)
{
}

CPhysicsComponent::~CPhysicsComponent()
{
}

/*void CPhysicsComponent::Init()
{
    Super::Init();
}*/

void CPhysicsComponent::Update(float DeltaTime)
{
    // Uncomment when needed
    //Super::Update(DeltaTime);

    if (!bActive)
    {
        return;
    }

    // Update velocity
    if (bAccelerating)
    {
        // TODO: @CS, need heading to update velocity
    }
    else if (bDecelerating)
    {
        // TODO: @CS, need heading to update velocity
    }

    // Limit velocity to max
    Velocity.Truncate(MaxSpeed);

    // Apply Drag
    Velocity.X *= (1 - DragFactor);
    Velocity.Y *= (1 - DragFactor);
}

/*void CPhysicsComponent::Render()
{
    Super::Render();
}

void CPhysicsComponent::Shutdown()
{
    Super::Shutdown();
}*/
