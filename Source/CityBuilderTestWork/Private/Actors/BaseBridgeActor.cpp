// CityBuilderTestWork. All Rights Reserved

#include "Actors/BaseBridgeActor.h"
#include "Actors/BaseBuildingActor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameStates/DefaultGameState.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseBridgeActor, All, All);

ABaseBridgeActor::ABaseBridgeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	SetRootComponent(SplineComponent);
}

void ABaseBridgeActor::BeginPlay()
{
	Super::BeginPlay();

	check(SplineComponent);	
}

void ABaseBridgeActor::CreateBridge(ABaseBuildingActor* FromBuilding, ABaseBuildingActor* ToBuildung)
{
	if (!BridgeMesh)
	{
		UE_LOG(LogBaseBridgeActor, Warning, TEXT("BridgeMesh doesn't set in defaults!"));
		return;
	}

	if (!FromBuilding || !ToBuildung)
		return;

	From = FromBuilding;
	To = ToBuildung;

	SplineComponent->SetLocationAtSplinePoint(0, From->GetActorLocation(), ESplineCoordinateSpace::World);
	SplineComponent->SetLocationAtSplinePoint(1, To->GetActorLocation(), ESplineCoordinateSpace::World);

	float SplineLength = SplineComponent->GetSplineLength();
	FVector MeshSize = BridgeMesh->GetBoundingBox().GetSize();
	float MeshLength = MeshSize.X * BridgeMeshScale;

	auto SegmentsNum = FMath::CeilToInt32(SplineLength / MeshLength);

	for (int32 i = 0; i < SegmentsNum; ++i)
	{
		float DistanceAlongSpline = (i * MeshLength) + (MeshLength / 2.0f);

		FVector MeshLocation = SplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator MeshRotator = SplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

		auto MeshComponent = NewObject<UStaticMeshComponent>(this);
		MeshComponent->SetStaticMesh(BridgeMesh);
		MeshComponent->SetRelativeScale3D(FVector(BridgeMeshScale));
		MeshComponent->SetRelativeLocation(MeshLocation);
		MeshComponent->SetRelativeRotation(MeshRotator);
		MeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		MeshComponent->RegisterComponent();

		BridgeMeshes.Add(MeshComponent);
	}

	auto CurrentGameState = GetWorld()->GetGameState<ADefaultGameState>();

	if (!CurrentGameState)
	{
		UE_LOG(LogBaseBridgeActor, Warning, TEXT("Couldn't find CurrentGameState in CreateBridge method!"));
		return;
	}

	CurrentGameState->AddResourcesUsage({ElectricityUsage, 0.0f, 0.0f, 0.0f});
}

void ABaseBridgeActor::Breakdown()
{
	if (!bIsActive)
		return;

	bIsActive = false;

	To->Breakdown();
	To = nullptr;

	From->DettachBridge();

	auto CurrentGameState = GetWorld()->GetGameState<ADefaultGameState>();

	if (!CurrentGameState)
	{
		UE_LOG(LogBaseBridgeActor, Warning, TEXT("Couldn't find CurrentGameState in Breakdown method!"));
		return;
	}

	CurrentGameState->AddResourcesUsage({ ElectricityUsage * -1, 0.0f, 0.0f, 0.0f });

	Destroy();
}
