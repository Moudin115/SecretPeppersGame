// RoomSpawnManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomSpawnManager.generated.h"

UCLASS(Blueprintable)
class SECRETPEPPERGAME_API ARoomSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values
	ARoomSpawnManager();

	// Simple proof-of-life function you can call from Blueprint
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void TestSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
