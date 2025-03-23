// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseBuildingActor.generated.h"

class ABaseBridgeActor;

USTRUCT(BlueprintType)
struct FResourcesUsageInfo {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Electricity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Water = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Oxygen = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Food = 0.0f;

	FResourcesUsageInfo Reverse();
};

UCLASS()
class CITYBUILDERTESTWORK_API ABaseBuildingActor : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseBuildingActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	bool bIsConnected = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	bool bIsFullConnected = false;

	UFUNCTION(BlueprintCallable)
	void Breakdown();

	UFUNCTION(BlueprintCallable)
	void ActivateBuilding();

	UFUNCTION(BlueprintCallable)
	void DettachBridge();

	//UFUNCTION(BlueprintCallable)
	//void ConnectToBuilding(ABaseBuildingActor* ConnectingBuilding);

	//UFUNCTION(BlueprintCallable)
	//void ConnectBuildingToThis(ABaseBuildingActor* ConnectingBuilding);

	void ConnectBridge(ABaseBridgeActor* ConnectingBridge);

	void ChangeColorToSelected();
	void ChangeColorToDefault();

	FResourcesUsageInfo GetResourcesUsageInfo() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	FResourcesUsageInfo ResourcesUsageInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	UMaterialInstance* DefaultColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	UMaterialInstance* ColorWhenSelected;

	virtual void BeginPlay() override;	

private:
	ABaseBridgeActor* ConnectedToThis;
	ABaseBridgeActor* ConnectedFromThis;
};
