// CityBuilderTestWork. All Rights Reserved


#include "PlayerControllers/DefaultPlayerController.h"
#include "Actors/BaseBridgeActor.h"
#include "Actors/BaseBuildingActor.h"
#include "Actors/BasePlatformActor.h"
#include "Camera/CameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameStates/DefaultGameState.h"

DEFINE_LOG_CATEGORY_STATIC(LogDefaultPlayerController, All, All);

ADefaultPlayerController::ADefaultPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

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

void ADefaultPlayerController::CreatePreviewBuilding()
{
	if (!SelectedBuildingType)
	{
		UE_LOG(LogDefaultPlayerController, Warning, TEXT("SelectedBuildingType is empty. Check button click logic!"));
		return;
	}

	if (PreviewBuilding)
	{
		PreviewBuilding->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	PreviewBuilding = GetWorld()->SpawnActor<ABaseBuildingActor>(SelectedBuildingType, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	auto RootPrimitive = Cast<UPrimitiveComponent>(PreviewBuilding->GetRootComponent());

	if (RootPrimitive)
	{
		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ADefaultPlayerController::ConnectBuildings()
{
	if (!BridgeClass)
	{
		UE_LOG(LogDefaultPlayerController, Warning, TEXT("Doesn't set BridgeClass in default!"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	auto Bridge = GetWorld()->SpawnActor<ABaseBridgeActor>(BridgeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (!Bridge)
	{
		UE_LOG(LogDefaultPlayerController, Warning, TEXT("Failed to create Bridge in ConnectBuildings method!"));
		return;
	}

	if (FirstSelectedBuilding->bIsConnected)
	{
		Bridge->CreateBridge(FirstSelectedBuilding, SecondSelectedBuilding);
		FirstSelectedBuilding->ConnectBridge(Bridge);
		SecondSelectedBuilding->ConnectBridge(Bridge);
		SecondSelectedBuilding->ActivateBuilding();		
	}
	else
	{
		Bridge->CreateBridge(SecondSelectedBuilding, FirstSelectedBuilding);
		SecondSelectedBuilding->ConnectBridge(Bridge);
		FirstSelectedBuilding->ConnectBridge(Bridge);
		FirstSelectedBuilding->ActivateBuilding();
	}

	OnExitFromAnyMode();
}

void ADefaultPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentModeType == EModeType::Build)
	{
		UpdatePreviewBuildingLocation();
	}
}

void ADefaultPlayerController::UpdatePreviewBuildingLocation()
{
	if (!PreviewBuilding)
	{
		UE_LOG(LogDefaultPlayerController, Warning, TEXT("Couldn't find PreviewBuilding. Check creating PreviewBuilding"));
		return;
	}

	float MouseX, MouseY;
	if (GetMousePosition(MouseX, MouseY))
	{
		FVector WorldLocation, WorldDirection;
		if (DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
		{
			FVector NewPosition = WorldLocation + WorldDirection * DistanceFromCameraToPreviewBuilding;
			PreviewBuilding->SetActorLocation(NewPosition);
		}
	}
}

bool ADefaultPlayerController::CanBeConnected() const
{
	return (FirstSelectedBuilding && SecondSelectedBuilding) 
		&& (FirstSelectedBuilding->bIsConnected || SecondSelectedBuilding->bIsConnected) 
		&& !(FirstSelectedBuilding->bIsConnected && SecondSelectedBuilding->bIsConnected)
		&& (!FirstSelectedBuilding->bIsFullConnected && !SecondSelectedBuilding->bIsFullConnected);
}

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("ExitFromAnyMode", IE_Pressed, this, &ADefaultPlayerController::OnExitFromAnyMode);
		InputComponent->BindAction("StartBreakdownMode", IE_Pressed, this, &ADefaultPlayerController::OnStartBreakdownMode);
		InputComponent->BindAction("MouseLeftButtonClick", IE_Pressed, this, &ADefaultPlayerController::OnMouseLeftButtonClick);
	}
}

void ADefaultPlayerController::HandleMouseLeftButtonClick()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);

	if (!HitResult.bBlockingHit)
		return;

	auto SelectedBuilding = Cast<ABaseBuildingActor>(HitResult.GetActor());

	switch (GetCurrentModeType())
	{
	case EModeType::Default:
	{
		if (!SelectedBuilding)
			return;

		SetModeType(EModeType::Connect);
		FirstSelectedBuilding = SelectedBuilding;
		FirstSelectedBuilding->ChangeColorToSelected();

		break;
	}
	case EModeType::Build:
	{
		if (!SelectedBuildingType)
		{
			UE_LOG(LogDefaultPlayerController, Warning, TEXT("SelectedBuildingType is empty. Check button click logic!"));
			return;
		}

		auto SelectedPlatform = Cast<ABasePlatformActor>(HitResult.GetActor());

		if (!SelectedPlatform)
			return;

		SpawnBuildingAtPlatform(SelectedPlatform);

		break;
	}
	case EModeType::Connect:
	{
		if (!SelectedBuilding || SelectedBuilding == FirstSelectedBuilding)
			return;

		if (SecondSelectedBuilding)
		{
			SecondSelectedBuilding->ChangeColorToDefault();
		}

		SecondSelectedBuilding = SelectedBuilding;
		SecondSelectedBuilding->ChangeColorToSelected();

		break;
	}
	case EModeType::Break:
	{
		if (SelectedBuilding)
		{
			if (!SelectedBuilding->bIsConnected)
				return;

			SelectedBuilding->Breakdown();
			return;
		}

		auto SelectedBridge = Cast<ABaseBridgeActor>(HitResult.GetActor());

		if (SelectedBridge)
		{
			SelectedBridge->Breakdown();
		}

		break;
	}
	default:
		break;
	}
}

void ADefaultPlayerController::SpawnBuildingAtPlatform(ABasePlatformActor* SelectedPlatform)
{
	if (SelectedPlatform->AttachBuilding(PreviewBuilding))
	{
		PreviewBuilding = nullptr;
		OnExitFromAnyMode();
	}
}

void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	/*if (GetSpectatorPawn())
	{
		GetSpectatorPawn()->bAddDefaultMovementBindings = false;
	}*/
}

void ADefaultPlayerController::OnExitFromAnyMode()
{
	if (CurrentModeType == EModeType::Default)
		return;

	SetModeType(EModeType::Default);

	SelectedBuildingType = nullptr;

	if (FirstSelectedBuilding)
	{
		FirstSelectedBuilding->ChangeColorToDefault();
		FirstSelectedBuilding = nullptr;
	}
	
	if (SecondSelectedBuilding)
	{
		SecondSelectedBuilding->ChangeColorToDefault();
		SecondSelectedBuilding = nullptr;
	}

	if (PreviewBuilding)
		PreviewBuilding->Destroy();
}

void ADefaultPlayerController::OnStartBreakdownMode()
{
	SetModeType(EModeType::Break);
}

void ADefaultPlayerController::OnMouseLeftButtonClick()
{
	HandleMouseLeftButtonClick();
}
