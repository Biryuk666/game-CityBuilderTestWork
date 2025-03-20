// CityBuilderTestWork. All Rights Reserved

#include "GameModes/DefaultGameMode.h"
#include "PlayerControllers/DefaultPlayerController.h"
#include "UI/DefaultHUD.h"

ADefaultGameMode::ADefaultGameMode()
{
	PlayerControllerClass = ADefaultPlayerController::StaticClass();
	HUDClass = ADefaultHUD::StaticClass();
}
