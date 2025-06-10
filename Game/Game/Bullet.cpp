#include "Bullet.h"

#include "../App/App.h"
#include "GameUtil.h"

CBullet::CBullet()
{
	DefaultInitialization();
}

CBullet::~CBullet()
{
}

void CBullet::Init()
{
	RespawnInitialization();

	bAlive = true;
}

void CBullet::Update(float deltaTime)
{
	// Lifetime check
	CurrentElapsedLifetime += (deltaTime * 0.001f);
	if (CurrentElapsedLifetime >= Lifetime)
	{
		CurrentElapsedLifetime = 0.0f;
		bAlive = false;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Physics
	// Limit velocity to max
	Velocity.Truncate(MaxSpeed);

	//////////////////////////////////////////////////////////////////////////
	// Transform
	// Update position
	Position.X += Velocity.X;
	Position.Y += Velocity.Y;

	// Wrap around
	if (Position.X < 0.0f)
	{
		Position.X += APP_VIRTUAL_WIDTH;
	}
	if (Position.X >= APP_VIRTUAL_WIDTH)
	{
		Position.X -= APP_VIRTUAL_WIDTH;
	}
	if (Position.Y < 0.0f)
	{
		Position.Y += APP_VIRTUAL_HEIGHT;
	}
	if (Position.Y >= APP_VIRTUAL_HEIGHT)
	{
		Position.Y -= APP_VIRTUAL_HEIGHT;
	}

	// Collision check with asteroid
	if (bIntersectsAsteroid)
	{
		bAlive = false;
		return;
	}
}

void CBullet::Render()
{
	RenderBullet();
}

void CBullet::Shutdown()
{
	bAlive = false;
	CleanupVBs();
}

void CBullet::SetHeading(SVector2D InVec)
{
	SVector2D::Normalize(InVec);

	Heading = InVec;
	Side = Heading.GetPerpendicular();
}

//////////////////////////////////////////////////////////////////////////
// Helper methods

void CBullet::DefaultInitialization()
{
	Position = SVector2D::Zero();
	Velocity = SVector2D::Zero();
	Scale = SVector2D::Identity();
	Heading = SVector2D::Normalize(SVector2D(0.0f, 1.0f));
	Side = Heading.GetPerpendicular();

	MaxSpeed = 10.0f;
	MaxForce = 1.0f;
	MaxTurnRate = 0.01f;

	bAlive = false;

	bIntersectsAsteroid = false;

	Lifetime = 0.75f;
	CurrentElapsedLifetime = 0.0f;
}

void CBullet::RespawnInitialization()
{
	CleanupVBs();
	CreateBulletVBs();

	bIntersectsAsteroid = false;
	CurrentElapsedLifetime = 0.0f;

	MaxSpeed = 10.0f;
}

void CBullet::CleanupVBs()
{
	BulletVB.clear();
	BulletVBTransformed.clear();
}

void CBullet::CreateBulletVBs()
{
	BulletVB.push_back(SVector2D(4.0f, 2.0f));
	BulletVB.push_back(SVector2D(-4.0f, 2.0f));
	BulletVB.push_back(SVector2D(-4.0f, -2.0f));
	BulletVB.push_back(SVector2D(4.0f, -2.0f));

	BulletVBTransformed = GetWorldTransform(BulletVB, Position, Heading, Side, Scale);
}

void CBullet::RenderBullet()
{
	BulletVBTransformed = GetWorldTransform(BulletVB, Position, Heading, Side, Scale);

	unsigned int i;
	for (i = 0; i < BulletVBTransformed.size(); ++i)
	{
		if (i != BulletVBTransformed.size() - 1)
		{
			App::RenderLine(BulletVBTransformed[i].X, BulletVBTransformed[i].Y, BulletVBTransformed[i + 1].X, BulletVBTransformed[i + 1].Y);
		}
	}

	App::RenderLine(BulletVBTransformed[i - 1].X, BulletVBTransformed[i - 1].Y, BulletVBTransformed[0].X, BulletVBTransformed[0].Y);
}

