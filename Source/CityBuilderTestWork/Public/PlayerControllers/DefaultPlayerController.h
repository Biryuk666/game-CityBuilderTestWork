// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class ABaseBridgeActor;
class ABaseBuildingActor;
class ABasePlatformActor;
class UCameraComponent;
class USpringArmComponent;

UENUM(BlueprintType)
enum class EModeType : uint8
{
	Default,
	Build,
	Connect,
	Break
};

UCLASS()
class CITYBUILDERTESTWORK_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADefaultPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Mode")
	void SetModeType(const EModeType& NewModeType);

	UFUNCTION(BlueprintCallable, Category = "Mode")
	EModeType GetCurrentModeType() const;

	UFUNCTION(BlueprintCallable)
	void SelectBuildingType(TSubclassOf<ABaseBuildingActor> BuildingType);

	UFUNCTION(BlueprintCallable)
	void CreatePreviewBuilding();

	UFUNCTION(BlueprintCallable)
	void ConnectBuildings();

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Building")
	float DistanceFromCameraToPreviewBuilding = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bridge")
	TSubclassOf<ABaseBridgeActor> BridgeClass;

	UFUNCTION(BlueprintCallable)
	bool CanBeConnected() const;

	virtual void SetupInputComponent() override;

	void HandleMouseLeftButtonClick();
	void UpdatePreviewBuildingLocation();
	void SpawnBuildingAtPlatform(ABasePlatformActor* SelectedPlatform);

	virtual void BeginPlay() override;	

private:
	EModeType CurrentModeType = EModeType::Default;
	TSubclassOf<ABaseBuildingActor> SelectedBuildingType;
	ABaseBuildingActor* PreviewBuilding;
	ABaseBuildingActor* FirstSelectedBuilding;
	ABaseBuildingActor* SecondSelectedBuilding;

	void OnExitFromAnyMode();
	void OnStartBreakdownMode();
	void OnMouseLeftButtonClick();
};
