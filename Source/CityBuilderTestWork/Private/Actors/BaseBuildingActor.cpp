// CityBuilderTestWork. All Rights Reserved

#include "Actors/BaseBuildingActor.h"
#include "Actors/BaseBridgeActor.h"
#include "Actors/BasePlatformActor.h"
#include "GameStates/DefaultGameState.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseBuildingActor, All, All);

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

FResourcesUsageInfo ABaseBuildingActor::GetResourcesUsageInfo() const
{
	return ResourcesUsageInfo;
}

void ABaseBuildingActor::BeginPlay()
{
	Super::BeginPlay();

	check(DefaultColor);
	check(ColorWhenSelected);

}

void ABaseBuildingActor::Breakdown()
{
	if (!bIsConnected)
		return;

	bIsConnected = false;

	auto CurrentGameState = GetWorld()->GetGameState<ADefaultGameState>();

	if (!CurrentGameState)
	{
		UE_LOG(LogBaseBuildingActor, Warning, TEXT("Couldn't find CurrentGameState in Breakdown method!"));
		return;
	}

	CurrentGameState->AddResourcesUsage(GetResourcesUsageInfo().Reverse());

	if (ConnectedFromThis)
	{
		ConnectedFromThis->Breakdown();
		ConnectedFromThis = nullptr;
	}

	if (IsValid(ConnectedToThis))
	{
		ConnectedToThis->Breakdown();
	}

	ABasePlatformActor* Platform = Cast<ABasePlatformActor>(GetOwner());
	Platform->bIsBusy = false;
}

void ABaseBuildingActor::ActivateBuilding()
{
	auto CurrentGameState = GetWorld()->GetGameState<ADefaultGameState>();

	if (!CurrentGameState)
	{
		UE_LOG(LogBaseBuildingActor, Warning, TEXT("Couldn't find CurrentGameState in ConnectToBuilding method!"));
		return;
	}

	CurrentGameState->AddResourcesUsage(GetResourcesUsageInfo());
	bIsConnected = true;
}

void ABaseBuildingActor::DettachBridge()
{
	ConnectedFromThis = nullptr;
}

//void ABaseBuildingActor::ConnectToBuilding(ABaseBuildingActor* ConnectingBuilding)
//{
//	auto CurrentGameState = GetWorld()->GetGameState<ADefaultGameState>();
//
//	if (!CurrentGameState)
//	{
//		UE_LOG(LogBaseBuildingActor, Warning, TEXT("Couldn't find CurrentGameState in ConnectToBuilding method!"));
//		return;
//	}
//
//	CurrentGameState->AddResourcesUsage(GetResourcesUsageInfo());
//
//	ConnectedTo = ConnectingBuilding;
//	bIsConnected = true;
//}

//void ABaseBuildingActor::ConnectBuildingToThis(ABaseBuildingActor* ConnectingBuilding)
//{
//	ConnectedToThis = ConnectingBuilding;
//	bIsFullConnected = true;
//}

void ABaseBuildingActor::ConnectBridge(ABaseBridgeActor* ConnectingBridge)
{
	if (!bIsConnected)
	{
		ConnectedToThis = ConnectingBridge;
	}
	else 
	{
		ConnectedFromThis = ConnectingBridge;
	}
}

void ABaseBuildingActor::ChangeColorToSelected()
{
	auto RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());

	if (RootPrimitive)
	{
		RootPrimitive->SetMaterial(0, ColorWhenSelected);
	}
}

void ABaseBuildingActor::ChangeColorToDefault()
{
	auto RootPrimitive = Cast<UPrimitiveComponent>(GetRootComponent());

	if (RootPrimitive)
	{
		RootPrimitive->SetMaterial(0, DefaultColor);
	}
}

FResourcesUsageInfo FResourcesUsageInfo::Reverse()
{
	FResourcesUsageInfo Result;

	Result.Electricity = Electricity * -1;
	Result.Water = Water * -1;
	Result.Oxygen = Oxygen * -1;
	Result.Food = Food * -1;

	return Result;
}
