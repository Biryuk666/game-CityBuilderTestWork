// CityBuilderTestWork. All Rights Reserved

#include "Actors/BaseBuildingActor.h"

ABaseBuildingActor::ABaseBuildingActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

FResourcesUsageInfo ABaseBuildingActor::GetResourcesUsageInfo() const
{
	return ResourcesUsageInfo;
}

void ABaseBuildingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseBuildingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

