// CityBuilderTestWork. All Rights Reserved

#include "GameModes/DefaultGameMode.h"
#include "GameStates/DefaultGameState.h"
#include "Pawns/BasePawn.h"
#include "PlayerControllers/DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

ADefaultGameMode::ADefaultGameMode()
{
	PlayerControllerClass = ADefaultPlayerController::StaticClass();
	HUDClass = ADefaultHUD::StaticClass();
	GameStateClass = ADefaultGameState::StaticClass();
	DefaultPawnClass = ABasePawn::StaticClass();
}
