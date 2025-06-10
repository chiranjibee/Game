#include "Player.h"

#include "../App/App.h"
#include "GameUtil.h"
#include "AsteroidGame.h"
#include "GameManager.h"
#include "BulletManager.h"
#include "../Framework/InputComponent.h"
//#include "../Framework/TransformComponent.h"

//////////////////////////////////////////////////////////////////////////
// PLAYER

CPlayer::CPlayer()
{
	GameOwner = nullptr;

	// Spawn components
	InputComp = new CInputComponent();
	InputComp->SetOwner(this);
	InputComp->SetActive(true);

	/*CInputComponent* MyTestComp = GetComponent<CInputComponent>();
	if (MyTestComp)
	{
		// TODO: @CS
	}*/
	
	DefaultInitialization();
}

CPlayer::~CPlayer()
{
}

void CPlayer::Init()
{
	SetState(EPlayerState::Respawn);

	// Init player
	if (!bInitialized)
	{
		RespawnInitialization();
	}
}

void CPlayer::Update(float deltaTime)
{
	if (CurrentState == EPlayerState::Respawn)
	{
		if (!bInitialized)
		{
			RespawnInitialization();
		}

		// Convert delta time to seconds
		CurrentDamageImmuneTime += (deltaTime * 0.001f);
		if (CurrentDamageImmuneTime >= DamageImmuneDuration)
		{
			CurrentDamageImmuneTime = 0.0f;
			bAlive = true;
			SetState(EPlayerState::Alive);
		}
		
		CurrentFlickerCounter++;
	}
	else if (CurrentState == EPlayerState::Alive)
	{
		// Player can now be damaged
		bCanBeDamaged = true;

		//////////////////////////////////////////////////////////////////////////
		// Input

		assert(InputComp != nullptr && "Input Component is nullptr");
		InputComp->Update(deltaTime);
		const SAGInputData& SAGData = InputComp->GetInputData();

		if (SAGData.bShoot && CurrentTimeBetweenShots <= 0.0f)
		{
			CreateBullet();
			CurrentTimeBetweenShots += (1.0f / FireRate);
		}

		if (CurrentTimeBetweenShots > 0.0f)
		{
			CurrentTimeBetweenShots -= (deltaTime * 0.001f);
		}
		else /*if (CurrentTimeBetweenShots <= 0.0f)*/
		{
			CurrentTimeBetweenShots = 0.0f;
		}

		//////////////////////////////////////////////////////////////////////////
		// Physics
		
		// Update velocity
		if (SAGData.bAcceleration)
		{
			Velocity.X += PlayerAcceleration * Heading.X;
			Velocity.Y += PlayerAcceleration * Heading.Y;
		}
		else if (SAGData.bDeceleration)
		{
			Velocity.X -= PlayerDeceleration * Heading.X;
			Velocity.Y -= PlayerDeceleration * Heading.Y;
		}

		// Limit velocity to max
		Velocity.Truncate(MaxSpeed);

		// Apply drag
		Velocity.X *= (1 - PlayerDragFactor);
		Velocity.Y *= (1 - PlayerDragFactor);

		//////////////////////////////////////////////////////////////////////////
		// Transform
		
		// Update position
		Position.X += Velocity.X;
		Position.Y += Velocity.Y;

		// Rotate to target direction
		if (!SAGData.RotationInput.IsZero())
		{
			const SVector2D HeadingTarget = SAGData.RotationInput + Position;
			RotateHeadingToFacePosition(HeadingTarget);
		}

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

		// Collision with asteroid
		if (bIntersectsAsteroid)
		{
			// Dead
			bAlive = false;
			bIntersectsAsteroid = false;
			SetState(EPlayerState::Dead);
		}
	}
	else if (CurrentState == EPlayerState::Dead)
	{
		bCanBeDamaged = false;
		bInitialized = false;

		if (bCanRespawn)
		{
			// Respawn delay
			CurrentRespawnDelayTime += (deltaTime * 0.001f);
			if (CurrentRespawnDelayTime >= RespawnDelayDuration)
			{
				CurrentRespawnDelayTime = 0.0f;
				SetState(EPlayerState::Respawn);
			}
		}
	}
}

void CPlayer::Render()
{
	std::string DebugPlayerStateText;

	if (CurrentState == EPlayerState::Respawn)
	{
		DebugPlayerStateText = "Player_Respawn";
		
		if (CurrentFlickerCounter % 11 == 0)
		{
			RenderPlayer();
		}
	}
	else if (CurrentState == EPlayerState::Alive)
	{
		DebugPlayerStateText = "Player_Alive";

		RenderPlayer();

		assert(InputComp != nullptr && "Input Component is nullptr");
		const SAGInputData& SAGData = InputComp->GetInputData();
		if (SAGData.bAcceleration)
		{
			RenderThruster();
		}

		if (bIntersectsAsteroid)
		{
#if defined(DEBUG) | defined(_DEBUG)
			App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 50.0f, APP_VIRTUAL_HEIGHT * 0.5f, "Asteroid Collision");
#endif
		}
	}
	else if (CurrentState == EPlayerState::Dead)
	{
		DebugPlayerStateText = "Player_Dead";
	}

	// Print state info
#if defined(DEBUG) | defined(_DEBUG)
	App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 50.0f, APP_VIRTUAL_HEIGHT * 0.5f + 200.0f, DebugPlayerStateText.c_str());
#endif
}

void CPlayer::Shutdown()
{
	bAlive = false;
	CleanupVBs();

	// Clear components
	InputComp->SetActive(false);
	InputComp->Shutdown();
	SafeDelete(&InputComp);
}

void CPlayer::SetHeading(SVector2D InVec)
{
	SVector2D::Normalize(InVec);

	Heading = InVec;
	Side = Heading.GetPerpendicular();
}

bool CPlayer::RotateHeadingToFacePosition(SVector2D InTarget)
{
	// Calculate direction to target
	const SVector2D DirToTarget = SVector2D::Normalize(InTarget - Position);

	// Dot product heading and direction to target
	float DotProduct = SVector2D::Dot(Heading, DirToTarget);

	// Clamp dot product to -1.0f to 1.0f
	Clamp(DotProduct, -1.0f, 1.0f);

	// Calculate angle between heading and direction to target
	float Angle = std::acosf(DotProduct);

	// Return if already facing the target
	if (Angle < FLT_EPSILON)
	{
		return true;
	}

	// Limit amount to turn to max turn rate
	if (Angle > MaxTurnRate)
	{
		Angle = MaxTurnRate;
	}

	// Create rotation matrix to rotate player heading vector
	SMatrix3X3 RotMat;
	RotMat.Rotate(Angle * static_cast<float>(Heading.Sign(DirToTarget)));
	RotMat.TransfromVector2D(Heading);
	RotMat.TransfromVector2D(Velocity);

	// Recalculate side vector
	Side = Heading.GetPerpendicular();

	return false;
}

//////////////////////////////////////////////////////////////////////////
// Helper methods

void CPlayer::DefaultInitialization()
{
	CurrentState = EPlayerState::None;
	PreviousState = CurrentState;

	MaxSpeed = 5.0f;
	PlayerAcceleration = 2.0f;
	PlayerDeceleration = 0.3f;
	PlayerDragFactor = 0.07f;
	MaxTurnRate = 0.1f;

	DamageImmuneDuration = 1.5f;
	RespawnDelayDuration = 2.0f;

	bInitialized = true;
	bCanRespawn = true;

	FireRate = 5.0f;

	BulletFireSoundName = ".\\Assets\\Bullet.wav";

	RespawnInitialization();
}

void CPlayer::RespawnInitialization()
{
	Position = SVector2D(APP_VIRTUAL_WIDTH * 0.5f, APP_VIRTUAL_HEIGHT * 0.5f);
	Velocity = SVector2D::Zero();
	Scale = SVector2D::Identity();
	Heading = SVector2D::Normalize(SVector2D(0.0f, 1.0f));
	Side = Heading.GetPerpendicular();

	bCanBeDamaged = false;
	bAlive = false;
	bCanRespawn = true;

	CurrentDamageImmuneTime = 0.0f;
	CurrentFlickerCounter = 0;

	CurrentRespawnDelayTime = 0.0f;

	CurrentTimeBetweenShots = 0.0f;

	bIntersectsAsteroid = false;

	// Vertex buffer reinit
	CleanupVBs();
	CreatePlayerVBs();
	CreateThrusterVBs();

	bInitialized = true;
}

void CPlayer::CleanupVBs()
{
	PlayerVB.clear();
	PlayerVBTransformed.clear();
	ThrusterVB.clear();
	ThrusterVBTransformed.clear();
}

void CPlayer::CreatePlayerVBs()
{
	PlayerVB.push_back(SVector2D(0.0f, -10.5f));
	PlayerVB.push_back(SVector2D(28.0f, 0.0f));
	PlayerVB.push_back(SVector2D(0.0f, 10.5f));
	PlayerVB.push_back(SVector2D(6.0f, 0.0f));

	PlayerVBTransformed = GetWorldTransform(PlayerVB, Position, Heading, Side, Scale);
}

void CPlayer::CreateThrusterVBs()
{
	/*ThrusterVB.push_back(Vector2D(6.0f, 0.0f));
	ThrusterVB.push_back(Vector2D(-5.0f, -4.0f));
	ThrusterVB.push_back(Vector2D(-5.0f, 0.0f));
	ThrusterVB.push_back(Vector2D(-5.0f, 4.0f));

	ThrusterVBTransformed = GetWorldTransform(ThrusterVB, Position, Heading, Side, Scale);*/

	ThrusterVB.push_back(SVector2D(6.0f, 0.0f));
	ThrusterVB.push_back(SVector2D(-5.0f, -4.0f));
	//ThrusterVB.push_back(Vector2D(-5.0f, 0.0f));
	ThrusterVB.push_back(SVector2D(-5.0f, 4.0f));

	ThrusterVBTransformed = GetWorldTransform(ThrusterVB, Position, Heading, Side, Scale);
}

void CPlayer::CreateBullet()
{
	const SVector2D BulletPos = Position + SVector2D::Normalize(Heading) * 30.0f;
	const SVector2D BulletVel = SVector2D::Normalize(Heading) * 10.0f;

	//GameManager::GetInstance().GetBulletManager()->SpawnBullet(BulletPos, Vector2D::Normalize(Heading), BulletVel);
	if (GameOwner)
	{
		GameOwner->GetGameManager()->GetBulletManager()->SpawnBullet(BulletPos, SVector2D::Normalize(Heading), BulletVel);

		// Play bullet fire sound
		if (!App::IsSoundPlaying(BulletFireSoundName.c_str()))
		{
			App::PlaySound(BulletFireSoundName.c_str());
		}
	}
}

void CPlayer::RenderPlayer()
{
	// Render player geometry
	PlayerVBTransformed = GetWorldTransform(PlayerVB, Position, Heading, Side, Scale);

	unsigned int i;
	for (i = 0; i < PlayerVBTransformed.size(); ++i)
	{
		if (i != PlayerVBTransformed.size() - 1)
		{
			App::RenderLine(PlayerVBTransformed[i].X, PlayerVBTransformed[i].Y, PlayerVBTransformed[i + 1].X, PlayerVBTransformed[i + 1].Y, 0.2f, 0.9f, 0.9f);
		}
	}

	App::RenderLine(PlayerVBTransformed[i - 1].X, PlayerVBTransformed[i - 1].Y, PlayerVBTransformed[0].X, PlayerVBTransformed[0].Y, 0.2f, 0.9f, 0.9f);
}

void CPlayer::RenderThruster()
{
	// Render thruster geometry
	/*ThrusterVBTransformed = GetWorldTransform(ThrusterVB, Position, Heading, Side, Scale);
	unsigned int j;
	for (j = 1; j < ThrusterVBTransformed.size(); ++j)
	{
		App::DrawLine(ThrusterVBTransformed[0].X, ThrusterVBTransformed[0].Y, ThrusterVBTransformed[j].X, ThrusterVBTransformed[j].Y, 0.9f, 0.75f, 0.2f);
	}*/

	// Rendering thruster as a close loop body
	ThrusterVBTransformed = GetWorldTransform(ThrusterVB, Position, Heading, Side, Scale);
	unsigned int j;
	for (j = 0; j < ThrusterVBTransformed.size(); ++j)
	{
		if (j != ThrusterVBTransformed.size() - 1)
		{
			App::RenderLine(ThrusterVBTransformed[j].X, ThrusterVBTransformed[j].Y, ThrusterVBTransformed[j + 1].X, ThrusterVBTransformed[j + 1].Y, 0.9f, 0.75f, 0.2f);
		}
	}

	App::RenderLine(ThrusterVBTransformed[j - 1].X, ThrusterVBTransformed[j - 1].Y, ThrusterVBTransformed[0].X, ThrusterVBTransformed[0].Y, 0.9f, 0.75f, 0.2f);
}

//////////////////////////////////////////////////////////////////////////
// State transitions

void CPlayer::SetState(EPlayerState InState)
{
	PreviousState = CurrentState;
	CurrentState = InState;
}

void CPlayer::RevertToPrevState()
{
	SetState(PreviousState);
}
