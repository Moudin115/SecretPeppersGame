#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomSpawnManager.generated.h"

UCLASS(Blueprintable)
class SECRETPEPPERGAME_API ARoomSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	ARoomSpawnManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Surface")
	bool bSnapToSurface = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Surface")
	bool bAlignToSurfaceNormal = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Surface", meta = (ClampMin = "0.0"))
	float SurfaceOffset = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Surface", meta = (ClampMin = "0.0"))
	float TraceUp = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Surface", meta = (ClampMin = "0.0"))
	float TraceDown = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Surface")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnForRoom();

protected:
	virtual void BeginPlay() override;
};
