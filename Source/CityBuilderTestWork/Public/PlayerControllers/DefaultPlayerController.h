// CityBuilderTestWork. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DefaultPlayerController.generated.h"

class ABaseBuildingActor;

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
	UFUNCTION(BlueprintCallable, Category = "Mode")
	void SetModeType(const EModeType& NewModeType);

	UFUNCTION(BlueprintCallable, Category = "Mode")
	EModeType GetCurrentModeType() const;

	UFUNCTION(BlueprintCallable)
	void SelectBuildingType(TSubclassOf<ABaseBuildingActor> BuildingType);

protected:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;	

private:
	EModeType CurrentModeType = EModeType::Default;
	TSubclassOf<ABaseBuildingActor> SelectedBuildingType = nullptr;

	void OnExitFromAnyMode();
	void OnStartBreakdownMode();
};
