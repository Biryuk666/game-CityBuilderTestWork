// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBridgeActor.generated.h"

class ABaseBuildingActor;
class USplineComponent;
class UStaticMeshComponent;

UCLASS()
class CITYBUILDERTESTWORK_API ABaseBridgeActor : public AActor
{
	GENERATED_BODY()

public:
	ABaseBridgeActor();

	void CreateBridge(ABaseBuildingActor* FromBuilding, ABaseBuildingActor* ToBuilding);
	void Breakdown();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	UStaticMesh* BridgeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float BridgeMeshScale = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float ElectricityUsage = -1.0f;

	virtual void BeginPlay() override;

private:
	bool bIsActive = true;
	ABaseBuildingActor* From;
	ABaseBuildingActor* To;

	TArray<UStaticMeshComponent*> BridgeMeshes;
};
