// Tav ernSubsystem.cpp
#include "TavernSubsystem.h"
#include "Math/UnrealMathUtility.h"

void UTavernSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ResetRun(State.Coins <= 0 ? 10 : State.Coins);
}

void UTavernSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FTavernState UTavernSubsystem::GetState() const
{
	return State;
}

void UTavernSubsystem::ResetRun(int32 StartCoins)
{
	State = FTavernState{};
	State.Coins = FMath::Max(0, StartCoins);
	State.TimeOfDay = ETavernTimeOfDay::Day;
	State.Cycle = 1;
	State.Vibe = 0;
	State.Cleanliness = 0;
	State.bShopOpen = false;
	State.NightEvent = ENightEventType::None;
	State.Hand.Reset();

	State.EnsureDoors();

	State.DayActionsLeft = RandRangeInclusive(State.DayActionsMin, State.DayActionsMax);
	State.NightActionsLeft = 0;

	BroadcastAll();
}

void UTavernSubsystem::FlipTime()
{
	if (State.TimeOfDay == ETavernTimeOfDay::Day)
	{
		State.TimeOfDay = ETavernTimeOfDay::Night;

		State.bShopOpen = false;
		State.NightActionsLeft = RandRangeInclusive(1, 3);

		RollNightEvent();
	}
	else
	{
		State.TimeOfDay = ETavernTimeOfDay::Day;
		State.Cycle += 1;

		State.bShopOpen = false;
		State.DayActionsLeft = RandRangeInclusive(State.DayActionsMin, State.DayActionsMax);

		State.NightEvent = ENightEventType::None;
	}

	OnTimeChanged.Broadcast(State.TimeOfDay);
	OnStateChanged.Broadcast(State);
}

void UTavernSubsystem::AddVibe(int32 Delta)
{
	State.Vibe += Delta;
	ClampVibeInternal();
	OnVibeChanged.Broadcast(State.Vibe);
	OnStateChanged.Broadcast(State);
}

void UTavernSubsystem::AddCleanliness(int32 Delta)
{
	State.Cleanliness += Delta;
	ClampCleanlinessInternal();
	OnStateChanged.Broadcast(State);
}

bool UTavernSubsystem::SpendCoins(int32 Cost)
{
	if (Cost <= 0) return true;
	if (State.Coins < Cost) return false;
	State.Coins -= Cost;
	OnStateChanged.Broadcast(State);
	return true;
}

void UTavernSubsystem::AddCoins(int32 Amount)
{
	State.Coins += FMath::Max(0, Amount);
	OnStateChanged.Broadcast(State);
}

bool UTavernSubsystem::SpendIngredients(int32 Cost)
{
	if (Cost <= 0) return true;
	if (State.Ingredients < Cost) return false;
	State.Ingredients -= Cost;
	OnStateChanged.Broadcast(State);
	return true;
}

void UTavernSubsystem::AddIngredients(int32 Amount)
{
	State.Ingredients += FMath::Max(0, Amount);
	OnStateChanged.Broadcast(State);
}

bool UTavernSubsystem::AddPreparedFood(int32 Delta)
{
	const int32 NewVal = State.PreparedFood + Delta;
	if (NewVal < 0) return false;
	State.PreparedFood = NewVal;
	OnStateChanged.Broadcast(State);
	return true;
}

bool UTavernSubsystem::TrySpendDayAction(int32 Amount)
{
	if (Amount <= 0) return true;
	if (State.TimeOfDay != ETavernTimeOfDay::Day) return false;
	if (State.DayActionsLeft < Amount) return false;
	State.DayActionsLeft -= Amount;
	OnStateChanged.Broadcast(State);
	return true;
}

bool UTavernSubsystem::TrySpendNightAction(int32 Amount)
{
	if (Amount <= 0) return true;
	if (State.TimeOfDay != ETavernTimeOfDay::Night) return false;
	if (State.NightActionsLeft < Amount) return false;
	State.NightActionsLeft -= Amount;
	OnStateChanged.Broadcast(State);
	return true;
}

FName UTavernSubsystem::GetDoorCard(ETavernDoorDir Dir) const
{
	const FName* Found = State.DoorCards.Find(Dir);
	return Found ? *Found : NAME_None;
}

bool UTavernSubsystem::SlotDoorCard(ETavernDoorDir Dir, FName CardId)
{
	if (CardId.IsNone()) return false;
	if (!IsMajorCardId(CardId)) return false;

	State.DoorCards.FindOrAdd(Dir) = CardId;

	OnDoorBoardChanged.Broadcast();
	OnStateChanged.Broadcast(State);
	return true;
}

void UTavernSubsystem::ClearDoor(ETavernDoorDir Dir)
{
	State.DoorCards.FindOrAdd(Dir) = NAME_None;
	OnDoorBoardChanged.Broadcast();
	OnStateChanged.Broadcast(State);
}

FName UTavernSubsystem::DrawCustomerCard()
{
	// Tutorial unlock, exactly once
	if (!State.bKitchenGiven && State.Vibe >= State.TutorialVibeThreshold)
	{
		State.bKitchenGiven = true;
		State.bKitchenHintShown = true;
		OnStateChanged.Broadcast(State);
		return State.KitchenCardId;
	}

	// Otherwise only resource cards
	if (State.ResourceCardIds.Num() <= 0)
	{
		return NAME_None;
	}

	const int32 Idx = FMath::RandRange(0, State.ResourceCardIds.Num() - 1);
	const FName Pick = State.ResourceCardIds[Idx];

	State.Hand.Add(Pick);
	OnStateChanged.Broadcast(State);
	return Pick;
}

ENightEventType UTavernSubsystem::RollNightEvent()
{
	// 75 percent scavenge, 25 percent npc, same as your prototype feel
	const float Roll = Rand01();
	State.NightEvent = (Roll < 0.75f) ? ENightEventType::Scavenge : ENightEventType::Npc;

	OnNightEventRolled.Broadcast(State.NightEvent);
	OnStateChanged.Broadcast(State);
	return State.NightEvent;
}

void UTavernSubsystem::ClearNightEvent()
{
	State.NightEvent = ENightEventType::None;
	OnStateChanged.Broadcast(State);
}

void UTavernSubsystem::SetCauldromatOwned(bool bOwned)
{
	State.bCauldromatOwned = bOwned;
	OnStateChanged.Broadcast(State);
}

void UTavernSubsystem::SetShopOpen(bool bOpen)
{
	State.bShopOpen = bOpen;
	OnStateChanged.Broadcast(State);
}

void UTavernSubsystem::BroadcastAll()
{
	OnTimeChanged.Broadcast(State.TimeOfDay);
	OnVibeChanged.Broadcast(State.Vibe);
	OnDoorBoardChanged.Broadcast();
	OnStateChanged.Broadcast(State);
}

void UTavernSubsystem::ClampVibeInternal()
{
	State.Vibe = FMath::Clamp(State.Vibe, State.VibeFloor, State.VibeCap);
}

void UTavernSubsystem::ClampCleanlinessInternal()
{
	State.Cleanliness = FMath::Clamp(State.Cleanliness, 0, 10);
}

int32 UTavernSubsystem::RandRangeInclusive(int32 Min, int32 Max) const
{
	return FMath::RandRange(Min, Max);
}

float UTavernSubsystem::Rand01() const
{
	return FMath::FRand();
}

bool UTavernSubsystem::IsMajorCardId(FName CardId) const
{
	// For now, treat anything that is not a resource id as a major.
	// Later you will swap this for CardDefinition assets.
	if (CardId.IsNone()) return false;
	for (const FName& ResId : State.ResourceCardIds)
	{
		if (ResId == CardId) return false;
	}
	return true;
}
