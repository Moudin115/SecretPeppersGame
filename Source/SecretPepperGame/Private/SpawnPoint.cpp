#include "SpawnPoint.h"
#include "Components/SceneComponent.h"

#if WITH_EDITORONLY_DATA
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#endif

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	EditorSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("EditorSprite"));
	EditorSprite->SetupAttachment(Root);
	EditorSprite->bIsScreenSizeScaled = true;
	EditorSprite->SetHiddenInGame(true);
	EditorSprite->bIsEditorOnly = true;

	EditorArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("EditorArrow"));
	EditorArrow->SetupAttachment(Root);
	EditorArrow->SetHiddenInGame(true);
	EditorArrow->bIsEditorOnly = true;
	EditorArrow->ArrowSize = 1.0f;
#endif
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
