#include "InputComponent.h"

#include "../App/App.h"

CInputComponent::CInputComponent()
{
}

CInputComponent::~CInputComponent()
{
}

void CInputComponent::Update(float DeltaTime)
{
    // Uncomment when needed
    //Super::Update(DeltaTime);
    if (!bActive)
    {
        return;
    }
    
    // Clear old input data every frame
    InputData.ResetData();

    // Movement input
    if (App::GetController().GetLeftTrigger() > 0.5f)
    {
        InputData.bDeceleration = true;
    }

    if (App::GetController().GetRightTrigger() > 0.5f)
    {
        InputData.bAcceleration = true;
    }

    // Rotation input
    if (App::GetController().GetLeftThumbStickX() > 0.5f)
    {
        InputData.RotationInput.X = 1.0f;
    }
    if (App::GetController().GetLeftThumbStickX() < -0.5f)
    {
        InputData.RotationInput.X = -1.0f;
    }
    if (App::GetController().GetLeftThumbStickY() > 0.5f)
    {
        InputData.RotationInput.Y = 1.0f;
    }
    if (App::GetController().GetLeftThumbStickY() < -0.5f)
    {
        InputData.RotationInput.Y = -1.0f;
    }

    // Normalize the inputs
    InputData.NormalizeData();

    // Shooting
    if (App::GetController().CheckButton(XINPUT_GAMEPAD_A))
    {
        InputData.bShoot = true;
    }
}

