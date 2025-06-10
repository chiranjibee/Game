#include "AsteroidGame.h"

#include "../App/App.h"
#include "GameUtil.h"
#include "GameManager.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "GameData.h"
#include "AsteroidManager.h"
#include "BulletManager.h"

//////////////////////////////////////////////////////////////////////////
// ASTEROID GAME

CAsteroidGame::CAsteroidGame() 
{
	GameManager = new CGameManager();

	DefaultInitialization();
}

CAsteroidGame::~CAsteroidGame()
{
}

void CAsteroidGame::Init()
{
	SetState(EGameState::MainMenu);

	// Play game ambient sound
	if (!App::IsSoundPlaying(GameAmbientSoundName.c_str()))
	{
		App::PlaySound(GameAmbientSoundName.c_str(), true);
	}

	if (!bInitialized)
	{
		RestartInitialization();
	}
}

void CAsteroidGame::Update(float deltaTime)
{
	if (CurrentState == EGameState::MainMenu)
	{
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A))
		{
			SetState(EGameState::GameRun);
		}

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_Y))
		{
			SetState(EGameState::HelpMenu);
		}
	}
	else if (CurrentState == EGameState::HelpMenu)
	{
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_B))
		{
			RevertToPrevState();
		}
	}
	else if (CurrentState == EGameState::GameRun)
	{
		if (!bInitialized)
		{
			RestartInitialization();
		}

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_BACK))
		{
			bGamePaused = true;
			SetState(EGameState::Paused);
		}

		// Update game objects
		if (Player)
		{
			Player->Update(deltaTime);
		}

		if (GameManager)
		{
			GameManager->Update(deltaTime);
		}

		// Check player-asteroid collision
		CheckPlayerAsteroidCollision();

		// Check bullet-asteroid collision
		CheckBulletAsteroidCollision();

		// Check for spawning subsequent asteroids
		CheckForAsteroidWaveSpawn();

		// Check for game over
		if (CurrentPlayerLives <= 0)
		{
			Player->SetCanRespawn(false);

			// Game over delay
			CurrentGameOverDelayTime += (deltaTime * 0.001f);
			if (CurrentGameOverDelayTime >= GameOverDelayDuration)
			{
				CurrentGameOverDelayTime = 0.0f;
				SetState(EGameState::GameOver);
			}
		}

		// Add score
		if (Player && Player->IsAlive())
		{
			CurrentTimeForScoring += (deltaTime * 0.001f);
			if (CurrentTimeForScoring >= 1.0f)
			{
				CurrentTimeForScoring = 0.0f;
				CurrentPlayerScore += ScorePerSecond;
			}
		}
		
	}
	else if (CurrentState == EGameState::Paused)
	{
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A))
		{
			bGamePaused = false;
			RevertToPrevState();
		}

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_B))
		{
			bInitialized = false;
			CleanupGame();
			SetState(EGameState::MainMenu);
		}
	}
	else if (CurrentState == EGameState::GameOver)
	{
		bInitialized = false;
		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A))
		{
			SetState(EGameState::GameRun);
		}

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_B))
		{
			CleanupGame();
			SetState(EGameState::MainMenu);
		}
	}
}

void CAsteroidGame::Render()
{
	std::string DebugGameStateText;

	if (CurrentState == EGameState::MainMenu)
	{
		DebugGameStateText = "Main_Menu";

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 50.0f, APP_VIRTUAL_HEIGHT * 0.5f + 100.0f, "ASTEROIDS", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24);

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 75.0f, APP_VIRTUAL_HEIGHT * 0.5f, "Press A to Start Game", 0.23f, 0.85f, 0.3f, GLUT_BITMAP_9_BY_15);

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 75.0f, APP_VIRTUAL_HEIGHT * 0.5f - 50.0f, "Press Y for Info", 0.92f, 0.85f, 0.2f, GLUT_BITMAP_9_BY_15);
	}
	else if (CurrentState == EGameState::HelpMenu)
	{
		DebugGameStateText = "Help_Menu";

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 50.0f, APP_VIRTUAL_HEIGHT * 0.5f + 150.0f, "CONTROLS", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18);
		
		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 175.0f, APP_VIRTUAL_HEIGHT * 0.5f + 100.0f, "Use Left Thumbstick to control the ship", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_9_BY_15);

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 100.0f, APP_VIRTUAL_HEIGHT * 0.5f + 50.0f, "Use A to shoot aseroids", 1.0f, 1.0f, 1.0f, GLUT_BITMAP_9_BY_15);

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 75.0f, APP_VIRTUAL_HEIGHT * 0.5f - 50.0f, "Press B to Return", 0.81f, 0.25f, 0.25f, GLUT_BITMAP_9_BY_15);
	}
	else if (CurrentState == EGameState::GameRun)
	{
		DebugGameStateText = "Game_Menu";

		if (Player)
		{
			Player->Render();
		}

		if (GameManager)
		{
			GameManager->Render();
		}

		// TODO: Add more hud elements
		const int PlayerLivesInfo = CurrentPlayerLives < 0 ? 0 : static_cast<int>(CurrentPlayerLives);
		const std::string PlayerLivesText = "Lives: " + std::to_string(PlayerLivesInfo);
		App::RenderText(APP_VIRTUAL_WIDTH * 0.2f, APP_VIRTUAL_HEIGHT - 20.0f, PlayerLivesText.c_str());

		const std::string PlayerScoreInfo = "Score: " + std::to_string(CurrentPlayerScore);
		App::RenderText(APP_VIRTUAL_WIDTH * 0.8f, APP_VIRTUAL_HEIGHT - 20.0f, PlayerScoreInfo.c_str());
	}
	else if (CurrentState == EGameState::Paused)
	{
		DebugGameStateText = "Pause_Menu";

		// Render game screen in background when paused
		if (Player)
		{
			Player->Render();
		}

		if (GameManager)
		{
			GameManager->Render();
		}

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 100.0f, APP_VIRTUAL_HEIGHT * 0.5f, "Press A to Resume");

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 100.0f, APP_VIRTUAL_HEIGHT * 0.5f - 50.0f, "Press B to Exit to Main Menu");
	}
	else if (CurrentState == EGameState::GameOver)
	{
		DebugGameStateText = "GameOver_Menu";

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 100.0f, APP_VIRTUAL_HEIGHT * 0.5f, "Press A to Restart Game");

		App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 100.0f, APP_VIRTUAL_HEIGHT * 0.5f - 50.0f, "Press B to Exit to Main Menu");
	}

	// Print state info
#if defined(DEBUG) | defined(_DEBUG)
	App::RenderText(APP_VIRTUAL_WIDTH * 0.5f - 50.0f, APP_VIRTUAL_HEIGHT - 50.0f, DebugGameStateText.c_str());
#endif
}

void CAsteroidGame::Shutdown()
{
	// Cleanup for when window closed while in game run state
	CleanupGame();

	if (GameManager)
	{
		GameManager->Shutdown();
	}

	delete GameManager;
	GameManager = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Helper methods

void CAsteroidGame::DefaultInitialization()
{
	Player = nullptr;
	
	bGamePaused = true;

	CurrentState = EGameState::None;
	PreviousState = CurrentState;

	DefaultPlayerLives = GameManager->GetGameData()->DefaultPlayerLives;
	CurrentPlayerLives = DefaultPlayerLives;

	MinAsteroidCountForRespawn = GameManager->GetGameData()->MinAsteroidCountForRespawn;
	ScorePerSecond = GameManager->GetGameData()->ScorePerSecond;
	ScorePerWaveClear = GameManager->GetGameData()->ScorePerWaveClear;
	ScorePerLargeAsteroid = GameManager->GetGameData()->ScorePerLargeAsteroid;
	ScorePerMediumAsteroid = GameManager->GetGameData()->ScorePerMediumAsteroid;
	ScorePerSmallAsteroid = GameManager->GetGameData()->ScorePerSmallAsteroid;

	CurrentPlayerScore = 0;

	GameOverDelayDuration = GameManager->GetGameData()->GameOverDelayDuration;

	GameAmbientSoundName = ".\\Assets\\GameAmbient.wav";
	PlayerExplosionSoundName = ".\\Assets\\Explosion.wav";
	WaveClearSoundName = ".\\Assets\\WaveClear.wav";
	AsteroidExplosionSoundName = ".\\Assets\\SmallExplosion.wav";

	bInitialized = false;
}

void CAsteroidGame::RestartInitialization()
{
	CleanupGame();
	CreatePlayer();

	SpawnDefaultAsteroidWave();

	CurrentGameOverDelayTime = 0.0f;

	CurrentAliveAsteroidCount = 0;

	CurrentPlayerScore = 0;

	CurrentPlayerLives = DefaultPlayerLives;

	bInitialized = true;
}

void CAsteroidGame::CleanupGame()
{
	if (Player)
	{
		Player->Shutdown();
		Player->SetGameOwner(nullptr);
	}

	SafeDelete(&Player);

	GameManager->GetAsteroidManager()->Cleanup();
	GameManager->GetBulletManager()->Cleanup();
}

void CAsteroidGame::CreatePlayer()
{
	Player = new CPlayer();
	Player->SetGameOwner(this);
	Player->Init();
}

void CAsteroidGame::SpawnDefaultAsteroidWave()
{
	for (unsigned int i = 0; i < GameManager->GetGameData()->FirstWaveAsteroidCount; ++i)
	{
		const float PosX = GetRandomValue(0.0f, APP_VIRTUAL_WIDTH);
		const float PosY = GetRandomValue(0.0f, APP_VIRTUAL_HEIGHT);
		const SVector2D RandPos = SVector2D(PosX, PosY);

		const float HeadX = GetRandomValue(-1.0f, 1.0f);
		const float HeadY = GetRandomValue(-1.0f, 1.0f);
		SVector2D RandHeading = SVector2D::Normalize(SVector2D(HeadX, HeadY));

		const SVector2D RandVelocity = RandHeading * 1.5f;
		GameManager->GetAsteroidManager()->SpawnAsteroid(EAsteroidType::Large, RandPos, RandHeading, RandVelocity);
	}
}

void CAsteroidGame::SpawnChildAsteroidWave(EAsteroidType InPrevType, SVector2D InPrevPos, SVector2D InPrevHead, SVector2D InPrevVel)
{
	// TODO: @CS, Remove casts and setup values
	if (InPrevType == EAsteroidType::Small)
	{
		return;
	}

	EAsteroidType NewType = EAsteroidType::None;
	if (InPrevType == EAsteroidType::Large)
	{
		NewType = EAsteroidType::Medium;
	}
	
	if (InPrevType == EAsteroidType::Medium)
	{
		NewType = EAsteroidType::Small;
	}

	const int NewTypeInt = static_cast<int>(NewType);
	
	SVector2D NewHeading = SVector2D::Normalize(InPrevHead);
	SVector2D NewVelocity = NewHeading * static_cast<float>(NewTypeInt) * 0.75f;
	GameManager->GetAsteroidManager()->SpawnAsteroid(NewType, InPrevPos, NewHeading, NewVelocity);

	NewHeading = NewHeading.GetPerpendicular();
	NewVelocity = NewVelocity.GetPerpendicular();
	GameManager->GetAsteroidManager()->SpawnAsteroid(NewType, InPrevPos, NewHeading, NewVelocity);

	NewHeading = NewHeading.GetReverse();
	NewVelocity = NewVelocity.GetReverse();
	GameManager->GetAsteroidManager()->SpawnAsteroid(NewType, InPrevPos, NewHeading, NewVelocity);
}

//void CAsteroidGame::SpawnSubsequentAsteroidWave()
//{
//	// TODO: @CS Maybe try spawn away from player
//	float PosX = GetRandomValue(0.0f, APP_VIRTUAL_WIDTH);
//	float PosY = GetRandomValue(0.0f, APP_VIRTUAL_HEIGHT);
//	Vector2D RandPos = Vector2D(PosX, PosY);
//
//	float HeadX = GetRandomValue(-1.0f, 1.0f);
//	float HeadY = GetRandomValue(-1.0f, 1.0f);
//	Vector2D RandHeading = Vector2D::Normalize(Vector2D(HeadX, HeadY));
//
//	Vector2D RandVelocity = RandHeading * 6.0f;
//
//	AsteroidManager::GetInstance().SpawnAsteroid(AsteroidType::Large, RandPos, RandHeading, RandVelocity);
//}

void CAsteroidGame::CheckPlayerAsteroidCollision()
{
	const std::vector<CAsteroid*> GameAsteroids = GameManager->GetAsteroidManager()->GetAsteroids();
	for (auto& InAsteroid : GameAsteroids)
	{
		// TODO: @CS, remove sound/scoring info out of here
		if (Player && Player->IsAlive() && InAsteroid->IsAlive())
		{
			if (ObjectIntersection(Player->GetObjectVertices(), InAsteroid->GetObjectVertices()))
			{
				Player->SetIntersectsAsteroid(true);
				RemovePlayerLife();

				// Play explosion sound
				if (!App::IsSoundPlaying(PlayerExplosionSoundName.c_str()))
				{
					App::PlaySound(PlayerExplosionSoundName.c_str());
				}

				break;
			}
		}
	}
}

void CAsteroidGame::CheckBulletAsteroidCollision()
{
	const std::vector<CBullet*> GameBullets = GameManager->GetBulletManager()->GetBullets();
	const std::vector<CAsteroid*> GameAsteroids = GameManager->GetAsteroidManager()->GetAsteroids();
	for (auto& InBullet : GameBullets)
	{
		for (auto& InAsteroid : GameAsteroids)
		{
			// TODO: @CS, remove sound/scoring info out of here
			if (InBullet->IsAlive() && InAsteroid->IsAlive())
			{
				if (ObjectIntersection(InBullet->GetObjectVertices(), InAsteroid->GetObjectVertices()))
				{
					InBullet->SetIntersectsAsteroid(true);

					SpawnChildAsteroidWave(InAsteroid->GetAsteroidType(), InAsteroid->GetPosition(), InBullet->GetHeading(), InAsteroid->GetVelocity());

					// Add score
					AddPlayerScore_KillAsteroid(InAsteroid->GetAsteroidType());

					InAsteroid->SetIntersectsBullet(true);

					// Play asteroid explosion sound
					//if (!App::IsSoundPlaying(AsteroidExplosionSoundName.c_str()))
					{
						App::PlaySound(AsteroidExplosionSoundName.c_str());
					}
				}
			}
		}
	}
}

void CAsteroidGame::AddPlayerScore_KillAsteroid(EAsteroidType InType)
{
	switch (InType)
	{
	case EAsteroidType::Large:
		CurrentPlayerScore += ScorePerLargeAsteroid;
		break;
	case EAsteroidType::Medium:
		CurrentPlayerScore += ScorePerMediumAsteroid;
		break;
	case EAsteroidType::Small:
		CurrentPlayerScore += ScorePerSmallAsteroid;
		break;
	case EAsteroidType::None:
	default:
		break;
	}
}

void CAsteroidGame::AddPlayerScore_WaveClear()
{
	CurrentPlayerScore += ScorePerWaveClear;
}

void CAsteroidGame::CheckForAsteroidWaveSpawn()
{
	CurrentAliveAsteroidCount = 0;
	const std::vector<CAsteroid*> GameAsteroids = GameManager->GetAsteroidManager()->GetAsteroids();
	for (auto& InAsteroid : GameAsteroids)
	{
		if (InAsteroid->IsAlive())
		{
			++CurrentAliveAsteroidCount;
		}
	}

	if (CurrentAliveAsteroidCount <= MinAsteroidCountForRespawn)
	{
		//SpawnSubsequentAsteroidWave();

		// Adding score for wave complete
		AddPlayerScore_WaveClear();

		// Play wave clear sound
		if (!App::IsSoundPlaying(WaveClearSoundName.c_str()))
		{
			App::PlaySound(WaveClearSoundName.c_str());
		}

		// Add player life
		AddPlayerLife();
		
		SpawnDefaultAsteroidWave();
	}
}

//////////////////////////////////////////////////////////////////////////
// State transitions

void CAsteroidGame::SetState(EGameState InState)
{
	PreviousState = CurrentState;
	CurrentState = InState;
}

void CAsteroidGame::RevertToPrevState()
{
	SetState(PreviousState);
}
