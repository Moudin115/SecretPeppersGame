#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeckWallWidget.generated.h"

class UDeckWallComponent;
class UTextBlock;
class UButton;
class UHorizontalBox;

// Functional prototype UI made entirely in C++. It can be replaced by a
// designer-authored Widget Blueprint once the wall interaction is confirmed.
UCLASS()
class SECRETPEPPERGAME_API UDeckWallWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWallComponent(UDeckWallComponent* InWall);
	void SetPreviousMouseCursor(bool bWasVisible);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UDeckWallComponent> WallComponent;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> CardLabel;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> SlotLabel;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> WallOverview;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> StatusLabel;

	TArray<FName> AvailableCards;
	int32 SelectedCardIndex = 0;
	int32 SelectedSlotIndex = 0;
	bool bPreviousMouseCursor = false;

	void BuildInterface();
	UTextBlock* AddText(class UVerticalBox* Parent, const FString& Value);
	UButton* AddButton(UHorizontalBox* Parent, const FString& Label);

	UFUNCTION()
	void RefreshView();

	UFUNCTION()
	void PreviousCard();

	UFUNCTION()
	void NextCard();

	UFUNCTION()
	void PreviousSlot();

	UFUNCTION()
	void NextSlot();

	UFUNCTION()
	void PlaceSelectedCard();

	UFUNCTION()
	void RemoveSelectedCard();

	UFUNCTION()
	void CloseWall();
};
