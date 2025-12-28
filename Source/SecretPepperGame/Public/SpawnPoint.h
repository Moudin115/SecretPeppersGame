#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "SpawnPoint.generated.h"

class USceneComponent;
class USpawnTable;

#if WITH_EDITORONLY_DATA
class UBillboardComponent;
class UArrowComponent;
#endif

UCLASS(Blueprintable)
class SECRETPEPPERGAME_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	ASpawnPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	USpawnTable* SpawnTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	FGameplayTagContainer SpawnPointTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (ClampMin = "0.0"))
	float LocalWeightMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bSpawnOnce = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	bool bDisableIfOccupied = true;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	bool IsOccupied() const;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* GetCurrentSpawn() const;

	void SetCurrentSpawn(AActor* Spawned);

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Editor")
	UBillboardComponent* EditorSprite = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Editor")
	UArrowComponent* EditorArrow = nullptr;
#endif

private:
	UPROPERTY()
	AActor* CurrentSpawn = nullptr;
};

