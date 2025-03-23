// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePlatformActor.generated.h"

class ABaseBuildingActor;
class USceneComponent;

UCLASS()
class CITYBUILDERTESTWORK_API ABasePlatformActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* BuildingSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform");
	bool bIsBusy = false;

	ABasePlatformActor();

	UFUNCTION(BlueprintCallable)
	bool AttachBuilding(class ABaseBuildingActor* Building);

	UFUNCTION(BlueprintCallable)
	void ReleasePlatform();
};
