// Tav ernSubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TavernSubsystem.generated.h"

UENUM(BlueprintType)
enum class ETavernTimeOfDay : uint8
{
	Day UMETA(DisplayName = "Day"),
	Night UMETA(DisplayName = "Night"),
};

UENUM(BlueprintType)
enum class ETavernDoorDir : uint8
{
	North UMETA(DisplayName = "North"),
	East  UMETA(DisplayName = "East"),
	South UMETA(DisplayName = "South"),
	West  UMETA(DisplayName = "West"),
};

UENUM(BlueprintType)
enum class ENightEventType : uint8
{
	None     UMETA(DisplayName = "None"),
	Scavenge UMETA(DisplayName = "Scavenge"),
	Npc      UMETA(DisplayName = "NPC"),
};

USTRUCT(BlueprintType)
struct FTavernState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Time")
	ETavernTimeOfDay TimeOfDay = ETavernTimeOfDay::Day;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Time")
	int32 Cycle = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Economy")
	int32 Coins = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Economy")
	int32 Ingredients = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Economy")
	int32 PreparedFood = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Mood")
	int32 Vibe = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Mood")
	int32 Cleanliness = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Actions")
	int32 DayActionsLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Actions")
	int32 NightActionsLeft = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Events")
	ENightEventType NightEvent = ENightEventType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Flags")
	bool bShopOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Flags")
	bool bCauldromatOwned = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Flags")
	bool bKitchenGiven = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Flags")
	bool bKitchenHintShown = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Doors")
	TMap<ETavernDoorDir, FName> DoorCards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Hand")
	TArray<FName> Hand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	int32 VibeCap = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	int32 VibeFloor = -5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	int32 DayActionsMin = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	int32 DayActionsMax = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	int32 TutorialVibeThreshold = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	FName KitchenCardId = "TheMagician";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern|Balance")
	TArray<FName> ResourceCardIds = { "Coin", "Ingredient" };

	void EnsureDoors()
	{
		if (!DoorCards.Contains(ETavernDoorDir::North)) DoorCards.Add(ETavernDoorDir::North, NAME_None);
		if (!DoorCards.Contains(ETavernDoorDir::East))  DoorCards.Add(ETavernDoorDir::East, NAME_None);
		if (!DoorCards.Contains(ETavernDoorDir::South)) DoorCards.Add(ETavernDoorDir::South, NAME_None);
		if (!DoorCards.Contains(ETavernDoorDir::West))  DoorCards.Add(ETavernDoorDir::West, NAME_None);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTavernStateChanged, const FTavernState&, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTavernTimeChanged, ETavernTimeOfDay, NewTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTavernVibeChanged, int32, NewVibe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTavernDoorBoardChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTavernNightEventRolled, ENightEventType, NightEvent);

UCLASS()
class SECRETPEPPERGAME_API UTavernSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable, Category = "Tavern|Events")
	FTavernStateChanged OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Tavern|Events")
	FTavernTimeChanged OnTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Tavern|Events")
	FTavernVibeChanged OnVibeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Tavern|Events")
	FTavernDoorBoardChanged OnDoorBoardChanged;

	UPROPERTY(BlueprintAssignable, Category = "Tavern|Events")
	FTavernNightEventRolled OnNightEventRolled;

	UFUNCTION(BlueprintCallable, Category = "Tavern|State")
	FTavernState GetState() const;

	UFUNCTION(BlueprintCallable, Category = "Tavern|State")
	void ResetRun(int32 StartCoins = 10);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Time")
	void FlipTime();

	UFUNCTION(BlueprintCallable, Category = "Tavern|Time")
	ETavernTimeOfDay GetTimeOfDay() const { return State.TimeOfDay; }

	UFUNCTION(BlueprintCallable, Category = "Tavern|Mood")
	void AddVibe(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Mood")
	int32 GetVibe() const { return State.Vibe; }

	UFUNCTION(BlueprintCallable, Category = "Tavern|Mood")
	void AddCleanliness(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Economy")
	bool SpendCoins(int32 Cost);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Economy")
	void AddCoins(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Economy")
	bool SpendIngredients(int32 Cost);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Economy")
	void AddIngredients(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Economy")
	bool AddPreparedFood(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Actions")
	bool TrySpendDayAction(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Actions")
	bool TrySpendNightAction(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Doors")
	FName GetDoorCard(ETavernDoorDir Dir) const;

	UFUNCTION(BlueprintCallable, Category = "Tavern|Doors")
	bool SlotDoorCard(ETavernDoorDir Dir, FName CardId);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Doors")
	void ClearDoor(ETavernDoorDir Dir);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Cards")
	FName DrawCustomerCard();

	UFUNCTION(BlueprintCallable, Category = "Tavern|Events")
	ENightEventType RollNightEvent();

	UFUNCTION(BlueprintCallable, Category = "Tavern|Events")
	void ClearNightEvent();

	UFUNCTION(BlueprintCallable, Category = "Tavern|Flags")
	void SetCauldromatOwned(bool bOwned);

	UFUNCTION(BlueprintCallable, Category = "Tavern|Flags")
	void SetShopOpen(bool bOpen);

private:
	UPROPERTY()
	FTavernState State;

	void BroadcastAll();
	void ClampVibeInternal();
	void ClampCleanlinessInternal();

	int32 RandRangeInclusive(int32 Min, int32 Max) const;
	float Rand01() const;

	bool IsMajorCardId(FName CardId) const;
};
