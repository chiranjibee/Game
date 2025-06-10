#pragma once

#include <vector>

#include "../Framework/GameObject.h"
#include "../Framework/Vector2D.h"

class CBullet : public CGameObject
{
    DECLARE_CLASS(CBullet, CGameObject)

    friend class CBulletManager;

    CBullet();

public:
    virtual ~CBullet();

	// Game object overrides
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Shutdown() override;

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

	inline float GetMaxForce() const { return MaxForce; }
	inline void SetMaxForce(float InVal) { MaxForce = InVal; }

	inline float GetSpeed() const { return Velocity.GetLength(); }
	inline float GetSpeedSq() const { return Velocity.GetLengthSq(); }

	inline float GetMaxTurnRate() const { return MaxTurnRate; }
	inline void SetMaxTurnRate(float InVal) { MaxTurnRate = InVal; }

	/////////////////////////////////////////////////////////////////////////
	// Rendering
	inline std::vector<SVector2D> GetObjectVertices() const { return BulletVBTransformed; }

	/////////////////////////////////////////////////////////////////////////
	// Misc
	inline float GetLifetime() const { return Lifetime; }
	
	inline bool IsAlive() const { return bAlive; }

	inline void SetIntersectsAsteroid(bool bVal) { bIntersectsAsteroid = bVal; }

private:

	/////////////////////////////////////////////////////////////////////////
	// Helper methods

	void DefaultInitialization();
	void RespawnInitialization();

	void CleanupVBs();
	void CreateBulletVBs();

	void RenderBullet();

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
	float MaxForce;
	float MaxTurnRate;

	/////////////////////////////////////////////////////////////////////////
	// Rendering

	// Vertex buffer
	std::vector<SVector2D> BulletVB;
	std::vector<SVector2D> BulletVBTransformed;

	/////////////////////////////////////////////////////////////////////////
	// Bullet specific

	float Lifetime;
	float CurrentElapsedLifetime;

	bool bIntersectsAsteroid;

	bool bAlive;
    
    
};
