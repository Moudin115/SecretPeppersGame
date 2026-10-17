#include "DeckWallActor.h"

#include "DeckWallComponent.h"
#include "DeckWallWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ADeckWallActor::ADeckWallActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	SetRootComponent(InteractionBox);
	InteractionBox->SetBoxExtent(FVector(75.0f, 12.0f, 100.0f));
	InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	DeckWallComponent = CreateDefaultSubobject<UDeckWallComponent>(TEXT("DeckWallComponent"));
	WidgetClass = UDeckWallWidget::StaticClass();
}

bool ADeckWallActor::CanInteract_Implementation(APawn* Interactor) const
{
	return IsValid(Interactor) && IsValid(DeckWallComponent)
		&& Cast<APlayerController>(Interactor->GetController()) != nullptr;
}

FText ADeckWallActor::GetInteractPrompt_Implementation() const
{
	return NSLOCTEXT("TarotDeckWall", "ArrangeTarotDeck", "Arrange Tarot Deck");
}

void ADeckWallActor::Interact_Implementation(APawn* Interactor)
{
	if (!CanInteract_Implementation(Interactor) || !WidgetClass)
	{
		return;
	}

	APlayerController* Controller = Cast<APlayerController>(Interactor->GetController());
	if (!Controller || !Controller->IsLocalController())
	{
		return;
	}

	if (IsValid(ActiveWidget) && ActiveWidget->IsInViewport())
	{
		return;
	}

	UDeckWallWidget* Widget = CreateWidget<UDeckWallWidget>(Controller, WidgetClass);
	if (!Widget)
	{
		return;
	}

	Widget->SetWallComponent(DeckWallComponent);
	Widget->SetPreviousMouseCursor(Controller->bShowMouseCursor);
	Widget->AddToViewport(50);
	ActiveWidget = Widget;

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(Widget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = true;
}
