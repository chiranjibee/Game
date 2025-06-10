#ifndef GE_INPUTCOMPONENT_H
#define GE_INPUTCOMPONENT_H

#include "ActorComponent.h"

#include "Vector2D.h"

/**
 * SAGInputData
 *
 * Asteroid Game Input Data
 */
struct SAGInputData
{
public:
    SAGInputData()
    {
        ResetData();
    }

    void ResetData()
    {
        bAcceleration = false;
        bDeceleration = false;
        bShoot = false;
        RotationInput = SVector2D::Zero();
    }

    void NormalizeData()
    {
        RotationInput.Normalize();
    }
    
public:
    uint8_t bAcceleration : 1;
    uint8_t bDeceleration : 1;
    uint8_t bShoot : 1;

    SVector2D RotationInput; 
};

/**
 * CInputComponent
 *
 * The input component handles the gamepad and keyboard/mouse support for the player
 */
class CInputComponent : public CActorComponent
{
    DECLARE_CLASS(CInputComponent, CActorComponent)
    
public:
    CInputComponent();
    virtual ~CInputComponent();

    // Overrides
    virtual void Update(float DeltaTime) override;

    const SAGInputData& GetInputData() const { return InputData; }
    
private:
    SAGInputData InputData;
};

#endif
