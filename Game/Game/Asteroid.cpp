#include "Asteroid.h"

#include "../App/App.h"
#include "GameUtil.h"

//////////////////////////////////////////////////////////////////////////
// Asteroid

CAsteroid::CAsteroid()
{
	DefaultInitialization();
}

CAsteroid::~CAsteroid()
{
}

void CAsteroid::Init()
{
	RespawnInitialization();

	bAlive = true;
}

void CAsteroid::Update(float deltaTime)
{
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

	// Collision check with bullet
	if (bIntersectsBullet)
	{
		bAlive = false;
		return;
	}
}

void CAsteroid::Render()
{
	RenderAsteroid();
}

void CAsteroid::Shutdown()
{
	bAlive = false;
	CleanupVBs();
}

void CAsteroid::SetHeading(SVector2D InVec)
{
	SVector2D::Normalize(InVec);

	Heading = InVec;
	Side = Heading.GetPerpendicular();
}

void CAsteroid::SetAsteroidType(EAsteroidType InType)
{
	if (MyType != InType)
	{
		MyType = InType;

		CleanupVBs();
		CreateAsteroidVBs();
	}
}

void CAsteroid::DefaultInitialization()
{
	Position = SVector2D::Zero();
	Velocity = SVector2D::Zero();
	Scale = SVector2D::Identity();

	MaxSpeed = 1.0f;
	MaxForce = 1.0f;
	MaxTurnRate = 0.01f;
	MinRadius = 30.0f;
	MaxRadius = 50.0f;

	Granularity = 16.0f;
	MinVariance = 0.25f;
	MaxVariance = 0.75f;

	Heading = SVector2D::Normalize(SVector2D(0.0f, 1.0f));
	Side = Heading.GetPerpendicular();

	MyType = EAsteroidType::Large;

	bIntersectsBullet = false;

	bAlive = false;
}

void CAsteroid::RespawnInitialization()
{
	CleanupVBs();
	CreateAsteroidVBs();

	bIntersectsBullet = false;

	MaxSpeed = 3.0f;
}

void CAsteroid::CleanupVBs()
{
	AsteroidVB.clear();
	AsteroidVBTransformed.clear();
}

void CAsteroid::CreateAsteroidVBs()
{
	const float AngleInc = PI * 2.0f / Granularity;
	for (float CurrentAngle = 0.0f; CurrentAngle < (PI * 2.0f); CurrentAngle += AngleInc)
	{
		const float RandVary = GetRandomValue(MinVariance, MaxVariance);
		const float AngleVary = AngleInc * RandVary;
		const float AngleFin = CurrentAngle + AngleVary - (AngleInc * 0.5f);
		const float RandRadius = GetRandomValue(MinRadius / static_cast<float>(MyType), MaxRadius / static_cast<float>(MyType));

		SVector2D Pt = SVector2D(std::cosf(AngleFin), std::sinf(AngleFin));
		Pt *= RandRadius;

		AsteroidVB.push_back(Pt);
	}

	AsteroidVBTransformed = GetWorldTransform(AsteroidVB, Position, Heading, Side, Scale);
}

void CAsteroid::RenderAsteroid()
{
	AsteroidVBTransformed = GetWorldTransform(AsteroidVB, Position, Heading, Side, Scale);

	unsigned int i;
	for (i = 0; i < AsteroidVBTransformed.size(); ++i)
	{
		if (i != AsteroidVBTransformed.size() - 1)
		{
			App::RenderLine(AsteroidVBTransformed[i].X, AsteroidVBTransformed[i].Y, AsteroidVBTransformed[i + 1].X, AsteroidVBTransformed[i + 1].Y, 0.6f, 0.3f, 0.0f);
		}
	}

	App::RenderLine(AsteroidVBTransformed[i - 1].X, AsteroidVBTransformed[i - 1].Y, AsteroidVBTransformed[0].X, AsteroidVBTransformed[0].Y, 0.6f, 0.3f, 0.0f);
}

