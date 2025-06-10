#ifndef GE_ACTOR_H
#define GE_ACTOR_H

#include "GameObject.h"

#include <vector>

#include "ActorComponent.h"

class CActor : public CGameObject
{
    DECLARE_CLASS(CActor, CGameObject)
public:
    CActor();
    virtual ~CActor();

    // Overrides
    void Init() override {}
    void Update(float DeltaTime) override {}
    void Render() override {}
    void Shutdown() override {}

    /*template<typename T>
    T* GetComponent() const;

    template<typename T>
    void AddComponent(T* InComp);

    template<typename T>
    void RemoveComponent(T* InComp);*/

protected:
    std::vector<CActorComponent*> Components;
};

#endif
