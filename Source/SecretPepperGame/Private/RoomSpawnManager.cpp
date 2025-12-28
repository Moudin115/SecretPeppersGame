// RoomSpawnManager.cpp

#include "RoomSpawnManager.h"

// Sets default values
ARoomSpawnManager::ARoomSpawnManager()
{
	// This actor does not need Tick
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARoomSpawnManager::BeginPlay()
{
	Super::BeginPlay();
}

// Blueprint-callable test function
void ARoomSpawnManager::TestSpawn()
{
	UE_LOG(LogTemp, Warning, TEXT("RoomSpawnManager is alive."));
}
