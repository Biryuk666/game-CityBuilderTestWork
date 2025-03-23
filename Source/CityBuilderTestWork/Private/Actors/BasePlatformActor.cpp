// CityBuilderTestWork. All Rights Reserved

#include "Actors/BasePlatformActor.h"
#include "Actors/BaseBuildingActor.h"
#include "Components/SceneComponent.h"
#include "GameStates/DefaultGameState.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePlatformActor, All, All);

ABasePlatformActor::ABasePlatformActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	BuildingSocket = CreateDefaultSubobject<USceneComponent>("BuildingSocket");
	BuildingSocket->SetupAttachment(GetRootComponent());
}

bool ABasePlatformActor::AttachBuilding(ABaseBuildingActor* Building)
{
	if (Building && !bIsBusy)
	{
		auto RootPrimitive = Cast<UPrimitiveComponent>(Building->GetRootComponent());

		if (RootPrimitive)
		{
			RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}

		if (!Building->AttachToComponent(BuildingSocket, FAttachmentTransformRules::SnapToTargetIncludingScale))
		{
			UE_LOG(LogBasePlatformActor, Warning, TEXT("Failed to attach building at platform!"));
			return false;
		}

		bIsBusy = true;

		Building->SetOwner(this);

		if (!Building->bIsConnected)
			return true;

		auto CurrentGameState = GetWorld()->GetGameState<ADefaultGameState>();

		if (!CurrentGameState)
		{
			UE_LOG(LogBasePlatformActor, Warning, TEXT("Couldn't find CurrentGameState in AttachBuilding method!"));
			return false;
		}

		CurrentGameState->AddResourcesUsage(Building->GetResourcesUsageInfo());

		return true;
	}

	return false;
}

void ABasePlatformActor::ReleasePlatform()
{
	bIsBusy = false;
}