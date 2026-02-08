#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TavernTextLibrary.generated.h"

UENUM(BlueprintType)
enum class ETavernMood : uint8
{
	Warm    UMETA(DisplayName = "Warm"),
	Neutral UMETA(DisplayName = "Neutral"),
	Sharp   UMETA(DisplayName = "Sharp")
};

UCLASS()
class SECRETPEPPERGAME_API UTavernTextLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Patron tension flavor
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static TArray<FText> GetPatronTensionLines(ETavernMood Mood);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static FText GetRandomPatronTensionLine(ETavernMood Mood);


	// Patron archetypes
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static TArray<FText> GetPatronArchetypes();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static FText GetRandomPatronArchetype();


	// Rumors, single shared pool, mood kept for Blueprint stability
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static TArray<FText> GetRumorLines(ETavernMood Mood);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static FText GetRandomRumor(ETavernMood Mood);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static FText GetRandomRumorNoRepeat(ETavernMood Mood, FText LastRumor, bool& bOutChanged);
};
