// CityBuilderTestWork. All Rights Reserved


#include "UI/DefaultHUD.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogDefaultHUD, All, All);

void ADefaultHUD::BeginPlay()
{
	if (!PlayerHUDWidgetClass)
	{
		UE_LOG(LogDefaultHUD, Warning, TEXT("The player HUD widget class is not set!"));
		return;
	}
	const auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);

	if (PlayerHUDWidget)
		PlayerHUDWidget->AddToViewport();
}
