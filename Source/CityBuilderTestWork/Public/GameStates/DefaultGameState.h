// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "Actors/BaseBuildingActor.h"
#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DefaultGameState.generated.h"

UCLASS()
class CITYBUILDERTESTWORK_API ADefaultGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	FResourcesUsageInfo CurrentResourcesAmount{ 100.0f, 50.0f, 50.0f, 50.0f };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Resources")
	FResourcesUsageInfo CurrentResourcesGrowth;

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentResourcesAmount();

	UFUNCTION(BlueprintCallable)
	void AddResourcesUsage(const FResourcesUsageInfo& BuildingResoursesUsage);

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle ResourceUpdateTimerHandle;
};
