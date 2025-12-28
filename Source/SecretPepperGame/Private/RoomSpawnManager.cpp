#include "RoomSpawnManager.h"
#include "SpawnPoint.h"
#include "SpawnTable.h"
#include "Engine/World.h"
#include "EngineUtils.h"

ARoomSpawnManager::ARoomSpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnForRoom();
}

void ARoomSpawnManager::SpawnForRoom()
{
	for (TActorIterator<ASpawnPoint> It(GetWorld()); It; ++It)
	{
		ASpawnPoint* Point = *It;
		if (!Point) continue;
		if (Point->IsOccupied()) continue;
		if (!Point->SpawnTable) continue;

		const TArray<FSpawnEntry>& Entries = Point->SpawnTable->Entries;
		if (Entries.Num() == 0) continue;

		float TotalWeight = 0.0f;
		for (const FSpawnEntry& Entry : Entries)
		{
			if (!Entry.ItemClass) continue;
			if (Entry.Weight <= 0.0f) continue;
			TotalWeight += Entry.Weight;
		}
		if (TotalWeight <= 0.0f) continue;

		const float Roll = FMath::FRandRange(0.0f, TotalWeight);
		float Running = 0.0f;

		const FSpawnEntry* Picked = nullptr;
		for (const FSpawnEntry& Entry : Entries)
		{
			if (!Entry.ItemClass) continue;
			if (Entry.Weight <= 0.0f) continue;

			Running += Entry.Weight;
			if (Roll <= Running)
			{
				Picked = &Entry;
				break;
			}
		}
		if (!Picked) continue;

		FTransform SpawnXform = Point->GetActorTransform();

		if (bSnapToSurface)
		{
			const FVector Origin = Point->GetActorLocation();
			const FVector Start = Origin + FVector(0.0f, 0.0f, TraceUp);
			const FVector End = Origin - FVector(0.0f, 0.0f, TraceDown);

			FHitResult Hit;
			FCollisionQueryParams Params(SCENE_QUERY_STAT(SpawnSurfaceTrace), false);
			Params.AddIgnoredActor(Point);

			const bool bHit = GetWorld()->LineTraceSingleByChannel(
				Hit,
				Start,
				End,
				TraceChannel,
				Params
			);

			if (bHit)
			{
				FVector Location = Hit.ImpactPoint;
				Location.Z += SurfaceOffset;
				SpawnXform.SetLocation(Location);

				if (bAlignToSurfaceNormal)
				{
					const FRotator AlignRot = FRotationMatrix::MakeFromZ(Hit.ImpactNormal).Rotator();
					SpawnXform.SetRotation(AlignRot.Quaternion());
				}
			}
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* Spawned = GetWorld()->SpawnActor<AActor>(
			Picked->ItemClass,
			SpawnXform,
			SpawnParams
		);

		if (Spawned)
		{
			Point->SetCurrentSpawn(Spawned);
		}
	}
}
