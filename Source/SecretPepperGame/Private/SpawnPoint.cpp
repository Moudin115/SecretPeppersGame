// SpawnPoint.cpp

#include "SpawnPoint.h"
#include "Components/SceneComponent.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

bool ASpawnPoint::IsOccupied() const
{
	return CurrentSpawn != nullptr && IsValid(CurrentSpawn);
}

AActor* ASpawnPoint::GetCurrentSpawn() const
{
	return CurrentSpawn;
}

void ASpawnPoint::SetCurrentSpawn(AActor* Spawned)
{
	CurrentSpawn = Spawned;
}
