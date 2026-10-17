#include "DeckWallWidget.h"

#include "DeckWallComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/PlayerController.h"

void UDeckWallWidget::SetWallComponent(UDeckWallComponent* InWall)
{
	WallComponent = InWall;
}

void UDeckWallWidget::SetPreviousMouseCursor(bool bWasVisible)
{
	bPreviousMouseCursor = bWasVisible;
}

void UDeckWallWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BuildInterface();
}

void UDeckWallWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(WallComponent.Get()))
	{
		WallComponent->OnWallChanged.AddUniqueDynamic(this, &UDeckWallWidget::RefreshView);
	}

	RefreshView();
}

void UDeckWallWidget::NativeDestruct()
{
	if (IsValid(WallComponent.Get()))
	{
		WallComponent->OnWallChanged.RemoveDynamic(this, &UDeckWallWidget::RefreshView);
	}

	Super::NativeDestruct();
}

UTextBlock* UDeckWallWidget::AddText(UVerticalBox* Parent, const FString& Value)
{
	UTextBlock* Label = WidgetTree->ConstructWidget<UTextBlock>();
	Label->SetText(FText::FromString(Value));
	Label->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	Parent->AddChildToVerticalBox(Label);
	return Label;
}

UButton* UDeckWallWidget::AddButton(UHorizontalBox* Parent, const FString& Label)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>();
	UTextBlock* Caption = WidgetTree->ConstructWidget<UTextBlock>();
	Caption->SetText(FText::FromString(Label));
	Caption->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	Button->SetBackgroundColor(FLinearColor(0.25f, 0.18f, 0.34f, 1.0f));
	Button->AddChild(Caption);
	Parent->AddChildToHorizontalBox(Button);
	return Button;
}

void UDeckWallWidget::BuildInterface()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UBorder* Background = WidgetTree->ConstructWidget<UBorder>();
	Background->SetBrushColor(FLinearColor(0.035f, 0.025f, 0.06f, 0.94f));
	Background->SetPadding(FMargin(28.0f));
	WidgetTree->RootWidget = Background;

	UVerticalBox* Layout = WidgetTree->ConstructWidget<UVerticalBox>();
	Background->SetContent(Layout);

	AddText(Layout, TEXT("THE DECK - TAROT TAVERN"));
	AddText(Layout, TEXT("Choose a card and a wall slot. Changes manifest with the next night's layout."));

	CardLabel = AddText(Layout, TEXT("Card:"));
	UHorizontalBox* CardButtons = WidgetTree->ConstructWidget<UHorizontalBox>();
	Layout->AddChildToVerticalBox(CardButtons);
	AddButton(CardButtons, TEXT("< Previous Card"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::PreviousCard);
	AddButton(CardButtons, TEXT("Next Card >"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::NextCard);

	SlotLabel = AddText(Layout, TEXT("Slot:"));
	UHorizontalBox* SlotButtons = WidgetTree->ConstructWidget<UHorizontalBox>();
	Layout->AddChildToVerticalBox(SlotButtons);
	AddButton(SlotButtons, TEXT("< Previous Slot"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::PreviousSlot);
	AddButton(SlotButtons, TEXT("Next Slot >"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::NextSlot);

	UHorizontalBox* ActionButtons = WidgetTree->ConstructWidget<UHorizontalBox>();
	Layout->AddChildToVerticalBox(ActionButtons);
	AddButton(ActionButtons, TEXT("Place Card"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::PlaceSelectedCard);
	AddButton(ActionButtons, TEXT("Remove from Slot"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::RemoveSelectedCard);
	AddButton(ActionButtons, TEXT("Close"))->OnClicked.AddUniqueDynamic(this, &UDeckWallWidget::CloseWall);

	StatusLabel = AddText(Layout, TEXT(""));
	AddText(Layout, TEXT("CURRENT WALL"));

	USizeBox* ListSize = WidgetTree->ConstructWidget<USizeBox>();
	ListSize->SetHeightOverride(340.0f);
	Layout->AddChildToVerticalBox(ListSize);
	UScrollBox* Scroll = WidgetTree->ConstructWidget<UScrollBox>();
	ListSize->AddChild(Scroll);
	WallOverview = WidgetTree->ConstructWidget<UTextBlock>();
	WallOverview->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	Scroll->AddChild(WallOverview);
}

void UDeckWallWidget::RefreshView()
{
	if (!CardLabel || !SlotLabel || !WallOverview || !StatusLabel)
	{
		return;
	}

	if (!IsValid(WallComponent.Get()))
	{
		CardLabel->SetText(FText::FromString(TEXT("No Deck Wall Component")));
		StatusLabel->SetText(FText::FromString(TEXT("This wall is no longer available.")));
		return;
	}

	AvailableCards = WallComponent->GetDefinedMajorArcanaIds();
	AvailableCards.Sort([](const FName& A, const FName& B)
	{
		return A.ToString() < B.ToString();
	});

	SelectedCardIndex = AvailableCards.Num() > 0
		? FMath::Clamp(SelectedCardIndex, 0, AvailableCards.Num() - 1) : 0;
	const int32 SlotCount = FMath::Max(1, WallComponent->SlotCount);
	SelectedSlotIndex = FMath::Clamp(SelectedSlotIndex, 0, SlotCount - 1);

	const FString CardDisplay = AvailableCards.IsValidIndex(SelectedCardIndex)
		? AvailableCards[SelectedCardIndex].ToString() : TEXT("(No card rows found)");
	CardLabel->SetText(FText::FromString(FString::Printf(TEXT("Selected card: %s"), *CardDisplay)));

	const FName AssignedCard = WallComponent->GetCardInWallSlot(SelectedSlotIndex);
	const FString AssignedDisplay = AssignedCard.IsNone() ? TEXT("(empty)") : AssignedCard.ToString();
	SlotLabel->SetText(FText::FromString(FString::Printf(
		TEXT("Selected wall slot: %d of %d | Current: %s"),
		SelectedSlotIndex + 1, SlotCount, *AssignedDisplay)));

	const TArray<FName> CardsInSlots = WallComponent->GetWallCardsInSlotOrder();
	FString Summary;
	for (int32 Index = 0; Index < CardsInSlots.Num(); ++Index)
	{
		const FName Id = CardsInSlots[Index];
		Summary += FString::Printf(TEXT("Slot %02d : %s\n"), Index + 1,
			Id.IsNone() ? TEXT("(empty)") : *Id.ToString());
	}
	WallOverview->SetText(FText::FromString(Summary));

	if (!WallComponent->MajorArcanaTable)
	{
		StatusLabel->SetText(FText::FromString(TEXT("Assign DT_MajorArcana on the Deck Wall Component first.")));
	}
}

void UDeckWallWidget::PreviousCard()
{
	if (AvailableCards.Num() > 0)
	{
		SelectedCardIndex = (SelectedCardIndex + AvailableCards.Num() - 1) % AvailableCards.Num();
		RefreshView();
	}
}

void UDeckWallWidget::NextCard()
{
	if (AvailableCards.Num() > 0)
	{
		SelectedCardIndex = (SelectedCardIndex + 1) % AvailableCards.Num();
		RefreshView();
	}
}

void UDeckWallWidget::PreviousSlot()
{
	if (IsValid(WallComponent.Get()))
	{
		const int32 Count = FMath::Max(1, WallComponent->SlotCount);
		SelectedSlotIndex = (SelectedSlotIndex + Count - 1) % Count;
		RefreshView();
	}
}

void UDeckWallWidget::NextSlot()
{
	if (IsValid(WallComponent.Get()))
	{
		const int32 Count = FMath::Max(1, WallComponent->SlotCount);
		SelectedSlotIndex = (SelectedSlotIndex + 1) % Count;
		RefreshView();
	}
}

void UDeckWallWidget::PlaceSelectedCard()
{
	if (!IsValid(WallComponent.Get()) || !AvailableCards.IsValidIndex(SelectedCardIndex))
	{
		return;
	}

	const bool bSuccess = WallComponent->PlaceMajorArcanaInSlot(
		SelectedSlotIndex, AvailableCards[SelectedCardIndex]);
	StatusLabel->SetText(FText::FromString(bSuccess
		? TEXT("Card placed on the wall.") : TEXT("Could not place this card.")));
	RefreshView();
}

void UDeckWallWidget::RemoveSelectedCard()
{
	if (!IsValid(WallComponent.Get()))
	{
		return;
	}

	const bool bSuccess = WallComponent->RemoveCardFromWallSlot(SelectedSlotIndex);
	StatusLabel->SetText(FText::FromString(bSuccess
		? TEXT("Card removed from this wall slot.") : TEXT("This slot is already empty.")));
	RefreshView();
}

void UDeckWallWidget::CloseWall()
{
	if (APlayerController* Controller = GetOwningPlayer())
	{
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = bPreviousMouseCursor;
	}

	RemoveFromParent();
}
