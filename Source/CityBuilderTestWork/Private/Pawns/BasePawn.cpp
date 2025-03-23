// CityBuilderTestWork. All Rights Reserved

#include "Pawns/BasePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	SpringArmComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SetRootComponent(SpringArmComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	check(SpringArmComponent);
	check(CameraComponent);
}

