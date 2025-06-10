#include "TransformComponent.h"

CTransformComponent::CTransformComponent()
    :   Position(SVector2D::Zero()),
        Heading(SVector2D::Normalize(SVector2D(0.0f, 1.0f))),
        Side(Heading.GetPerpendicular()),
        Scale(SVector2D::Identity())
{
}

CTransformComponent::~CTransformComponent()
{
}

/*void CTransformComponent::Init()
{
}*/

void CTransformComponent::Update(float DeltaTime)
{
    // Uncomment when needed
    //Super::Update(DeltaTime);
}

/*void CTransformComponent::Render()
{
}

void CTransformComponent::Shutdown()
{
}*/
