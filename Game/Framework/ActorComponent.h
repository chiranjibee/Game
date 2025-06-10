#ifndef GE_ACTORCOMPONENT_H
#define GE_ACTORCOMPONENT_H

#include "GameObject.h"

class CActor;

class CActorComponent : public CGameObject
{
    DECLARE_CLASS(CActorComponent, CGameObject)
public:
    CActorComponent();
    virtual ~CActorComponent();

    // Overrides
    virtual void Init() override;
    virtual void Update(float DeltaTime) override;
    virtual void Render() override;
    virtual void Shutdown() override;

    void SetOwner(CActor* InActor);
    CActor* GetOwner() const { return Owner; }

    bool IsActive() const { return bActive; }
    void SetActive(bool bValue);
    
protected:
    // Reference to the actor owner of this component
    CActor* Owner;
    
    uint8_t bActive : 1;
};

#endif
