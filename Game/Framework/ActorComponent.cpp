#include "ActorComponent.h"

#include "Actor.h"

CActorComponent::CActorComponent()
{
    ThisClass::Init();
}

CActorComponent::~CActorComponent()
{
}

void CActorComponent::Init()
{
    Owner = nullptr;
}

void CActorComponent::Update(float DeltaTime)
{
}

void CActorComponent::Render()
{
}

void CActorComponent::Shutdown()
{
    Owner = nullptr;
}

void CActorComponent::SetOwner(CActor* InActor)
{
    if (InActor != nullptr)
    {
        Owner = InActor;
        //InActor->AddComponent(this);
    }
}

void CActorComponent::SetActive(bool bValue)
{
    if (bActive != bValue)
    {
        bActive = bValue;
    }
}
