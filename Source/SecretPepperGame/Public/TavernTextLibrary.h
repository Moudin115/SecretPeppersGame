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
class UTavernTextLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static TArray<FText> GetPatronTensionLines(ETavernMood Mood);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static FText GetRandomPatronTensionLine(ETavernMood Mood);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static TArray<FText> GetPatronArchetypes();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tavern|Text")
	static FText GetRandomPatronArchetype();
};

