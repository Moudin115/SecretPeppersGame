// TavernSubsystem.cpp
#include "TavernSubsystem.h"

void UTavernSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	State.TimeOfDay = ETavernTimeOfDay::Day;
	State.DayNr = 1;
	State.Vibe = 0;
	State.Cleanliness = 0;

	Inventory.Empty();
	Inventory.Add(ETavernIngredient::Generic, 0);
	Inventory.Add(ETavernIngredient::CatFloof, 0);
	Inventory.Add(ETavernIngredient::Herb, 0);
	Inventory.Add(ETavernIngredient::Ash, 0);

	State.Coins = 10;
	State.PreparedFood = 0;
}

void UTavernSubsystem::Deinitialize()
{
	Inventory.Empty();
	Super::Deinitialize();
}

const FTavernState& UTavernSubsystem::GetState() const
{
	return State;
}

int32 UTavernSubsystem::GetDayNr() const
{
	return State.DayNr;
}

void UTavernSubsystem::SetDayNr(int32 NewDayNr)
{
	NewDayNr = FMath::Max(1, NewDayNr);

	if (State.DayNr == NewDayNr)
	{
		return;
	}

	State.DayNr = NewDayNr;
	OnDayChanged.Broadcast(State.DayNr);
}

void UTavernSubsystem::AdvanceDay()
{
	SetDayNr(State.DayNr + 1);
}

int32 UTavernSubsystem::ClampVibe(int32 InVibe) const
{
	return FMath::Clamp(InVibe, VibeMin, VibeMax);
}

int32 UTavernSubsystem::ClampCleanliness(int32 InCleanliness) const
{
	return FMath::Clamp(InCleanliness, CleanlinessMin, CleanlinessMax);
}

void UTavernSubsystem::SetTimeOfDay(ETavernTimeOfDay NewTime)
{
	if (State.TimeOfDay == NewTime)
	{
		return;
	}

	State.TimeOfDay = NewTime;
	OnTimeChanged.Broadcast(State.TimeOfDay);
}

void UTavernSubsystem::FlipTimeOfDay()
{
	if (State.TimeOfDay == ETavernTimeOfDay::Day)
	{
		SetTimeOfDay(ETavernTimeOfDay::Night);
	}
	else
	{
		AdvanceDay();
		SetTimeOfDay(ETavernTimeOfDay::Day);
	}
}

void UTavernSubsystem::SetVibe(int32 NewVibe)
{
	const int32 Clamped = ClampVibe(NewVibe);
	if (State.Vibe == Clamped)
	{
		return;
	}

	State.Vibe = Clamped;
	OnVibeChanged.Broadcast(State.Vibe);
}

void UTavernSubsystem::AddVibe(int32 Delta)
{
	SetVibe(State.Vibe + Delta);
}

void UTavernSubsystem::SetCleanliness(int32 NewCleanliness)
{
	const int32 Clamped = ClampCleanliness(NewCleanliness);
	if (State.Cleanliness == Clamped)
	{
		return;
	}

	State.Cleanliness = Clamped;
	OnCleanlinessChanged.Broadcast(State.Cleanliness);
}

void UTavernSubsystem::AddCleanliness(int32 Delta)
{
	SetCleanliness(State.Cleanliness + Delta);
}

int32 UTavernSubsystem::GetIngredientCount(ETavernIngredient Type) const
{
	if (const int32* Found = Inventory.Find(Type))
	{
		return *Found;
	}
	return 0;
}

bool UTavernSubsystem::HasIngredient(ETavernIngredient Type, int32 Amount) const
{
	return GetIngredientCount(Type) >= Amount;
}

void UTavernSubsystem::AddIngredient(ETavernIngredient Type, int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	const int32 NewValue = GetIngredientCount(Type) + Amount;
	Inventory.FindOrAdd(Type) = NewValue;
}

bool UTavernSubsystem::ConsumeIngredient(ETavernIngredient Type, int32 Amount)
{
	if (Amount <= 0)
	{
		return true;
	}

	const int32 Current = GetIngredientCount(Type);
	if (Current < Amount)
	{
		return false;
	}

	Inventory.FindOrAdd(Type) = Current - Amount;
	return true;
}

int32 UTavernSubsystem::GetPreparedFood() const
{
	return State.PreparedFood;
}

void UTavernSubsystem::AddPreparedFood(int32 Amount)
{
	if (Amount <= 0) return;
	State.PreparedFood += Amount;
}

bool UTavernSubsystem::ConsumePreparedFood(int32 Amount)
{
	if (Amount <= 0) return true;

	if (State.PreparedFood < Amount)
	{
		return false;
	}

	State.PreparedFood -= Amount;
	return true;
}

void UTavernSubsystem::ClearInventory()
{
	for (auto& Pair : Inventory)
	{
		Pair.Value = 0;
	}
}

int32 UTavernSubsystem::GetCoins() const
{
	return State.Coins;
}

void UTavernSubsystem::AddCoins(int32 Amount)
{
	if (Amount <= 0) return;
	State.Coins += Amount;
}

bool UTavernSubsystem::SpendCoins(int32 Amount)
{
	if (Amount <= 0) return true;

	if (State.Coins < Amount)
	{
		return false;
	}

	State.Coins -= Amount;
	return true;
}
