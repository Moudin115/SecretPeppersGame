// SpawnTable.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SpawnTable.generated.h"

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TSubclassOf<AActor> ItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0.0"))
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FGameplayTagContainer RoomTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0"))
	int32 MaxPerRoom = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0"))
	int32 MinProgress = 0;
};

UCLASS(BlueprintType)
class SECRETPEPPERGAME_API USpawnTable : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<FSpawnEntry> Entries;
};
