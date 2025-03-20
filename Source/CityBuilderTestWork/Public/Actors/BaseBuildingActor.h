// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuildingActor.generated.h"

USTRUCT(BlueprintType)
struct FResourcesUsageInfo {

	GENERATED_BODY()

	int32 Electricity = 0;
	int32 Water = 0;
	int32 Oxygen = 0;
	int32 Food = 0;
};

UCLASS()
class CITYBUILDERTESTWORK_API ABaseBuildingActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseBuildingActor();

	FResourcesUsageInfo GetResourcesUsageInfo() const;

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Build")
	FResourcesUsageInfo ResourcesUsageInfo;

	virtual void BeginPlay() override;	
};
