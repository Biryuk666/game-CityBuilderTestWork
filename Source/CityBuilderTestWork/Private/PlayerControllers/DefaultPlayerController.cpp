// CityBuilderTestWork. All Rights Reserved


#include "PlayerControllers/DefaultPlayerController.h"
#include "Actors/BaseBuildingActor.h"
#include "GameFramework/SpectatorPawn.h"

void ADefaultPlayerController::SetModeType(const EModeType& NewModeType)
{
	CurrentModeType = NewModeType;
}

EModeType ADefaultPlayerController::GetCurrentModeType() const
{
	return CurrentModeType;
}

void ADefaultPlayerController::SelectBuildingType(TSubclassOf<ABaseBuildingActor> BuildingType)
{
	SelectedBuildingType = BuildingType;
}

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("ExitFromAnyMode", IE_Pressed, this, &ADefaultPlayerController::OnExitFromAnyMode);
		InputComponent->BindAction("StartBreakdownMode", IE_Pressed, this, &ADefaultPlayerController::OnStartBreakdownMode);
	}
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	if (GetSpectatorPawn())
	{
		GetSpectatorPawn()->bAddDefaultMovementBindings = false;
	}
}

void ADefaultPlayerController::OnExitFromAnyMode()
{
	if (CurrentModeType == EModeType::Default)
		return;

	SetModeType(EModeType::Default);

	if (SelectedBuildingType)
		SelectedBuildingType = nullptr;
}

void ADefaultPlayerController::OnStartBreakdownMode()
{
	SetModeType(EModeType::Break);
}
