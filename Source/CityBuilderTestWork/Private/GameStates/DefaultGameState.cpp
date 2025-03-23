// CityBuilderTestWork. All Rights Reserved

#include "GameStates/DefaultGameState.h"

void ADefaultGameState::UpdateCurrentResourcesAmount()
{
	CurrentResourcesAmount.Electricity += CurrentResourcesGrowth.Electricity / 60;
	CurrentResourcesAmount.Water += CurrentResourcesGrowth.Water / 60;
	CurrentResourcesAmount.Oxygen += CurrentResourcesGrowth.Oxygen / 60;
	CurrentResourcesAmount.Food += CurrentResourcesGrowth.Food / 60;
}

void ADefaultGameState::AddResourcesUsage(const FResourcesUsageInfo& BuildingResoursesUsage)
{
	CurrentResourcesGrowth.Electricity += BuildingResoursesUsage.Electricity;
	CurrentResourcesGrowth.Water += BuildingResoursesUsage.Water;
	CurrentResourcesGrowth.Oxygen += BuildingResoursesUsage.Oxygen;
	CurrentResourcesGrowth.Food += BuildingResoursesUsage.Food;
}

void ADefaultGameState::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(ResourceUpdateTimerHandle, this, &ADefaultGameState::UpdateCurrentResourcesAmount, 1, true);
}
