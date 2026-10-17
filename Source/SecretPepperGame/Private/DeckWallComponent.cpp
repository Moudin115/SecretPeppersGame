#include "DeckWallComponent.h"

#include "TavernLayoutSubsystem.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

UDeckWallComponent::UDeckWallComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDeckWallComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			LayoutSubsystem = GameInstance->GetSubsystem<UTavernLayoutSubsystem>();
		}
	}

	if (LayoutSubsystem)
	{
		LayoutSubsystem->OnDeckChanged.AddUniqueDynamic(this, &UDeckWallComponent::HandleDeckChanged);
	}
}

void UDeckWallComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(LayoutSubsystem.Get()))
	{
		LayoutSubsystem->OnDeckChanged.RemoveDynamic(this, &UDeckWallComponent::HandleDeckChanged);
	}

	Super::EndPlay(EndPlayReason);
}

TArray<FName> UDeckWallComponent::GetDefinedMajorArcanaIds() const
{
	return MajorArcanaTable ? MajorArcanaTable->GetRowNames() : TArray<FName>();
}

bool UDeckWallComponent::IsValidSlot(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < SlotCount;
}

bool UDeckWallComponent::PlaceMajorArcanaInSlot(int32 SlotIndex, FName CardId)
{
	if (!LayoutSubsystem || !IsValidSlot(SlotIndex) || CardId.IsNone() || !MajorArcanaTable)
	{
		return false;
	}

	if (!MajorArcanaTable->GetRowNames().Contains(CardId))
	{
		return false;
	}

	// This changes the deck only. Rooms are regenerated at the next night,
	// not immediately while the player may be inside a temporary room.
	return LayoutSubsystem->PlaceCardInSlot(SlotIndex, CardId);
}

bool UDeckWallComponent::RemoveCardFromWallSlot(int32 SlotIndex)
{
	if (!LayoutSubsystem || !IsValidSlot(SlotIndex))
	{
		return false;
	}

	return LayoutSubsystem->RemoveCardFromSlot(SlotIndex);
}

FName UDeckWallComponent::GetCardInWallSlot(int32 SlotIndex) const
{
	return LayoutSubsystem && IsValidSlot(SlotIndex)
		? LayoutSubsystem->GetCardInSlot(SlotIndex)
		: NAME_None;
}

TArray<FName> UDeckWallComponent::GetWallCardsInSlotOrder() const
{
	TArray<FName> Cards;
	Cards.Init(NAME_None, FMath::Max(0, SlotCount));

	if (LayoutSubsystem)
	{
		for (const TPair<int32, FName>& Pair : LayoutSubsystem->GetPlacedCards())
		{
			if (IsValidSlot(Pair.Key))
			{
				Cards[Pair.Key] = Pair.Value;
			}
		}
	}

	return Cards;
}

void UDeckWallComponent::HandleDeckChanged()
{
	OnWallChanged.Broadcast();
}
