#pragma once

#include "ActorComponent.h"
#include "Vector2D.h"

/**
 * CTransformComponent
 *
 * TODO: @CS
 */
class CTransformComponent : public CActorComponent
{
    DECLARE_CLASS(CTransformComponent, CActorComponent)
public:
    CTransformComponent();
    virtual ~CTransformComponent();

    // Overrides
    //virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    //virtual void Render() override;
    //virtual void Shutdown() override;

protected:
    SVector2D Position;
    SVector2D Heading;
    SVector2D Side;
    SVector2D Scale;
    
};
