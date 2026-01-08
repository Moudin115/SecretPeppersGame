#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UCameraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SECRETPEPPERGAME_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float TraceDistance = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_GameTraceChannel2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	bool bDrawDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	bool bLogToScreen = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	FName InteractionCameraTag = TEXT("InteractionCamera");

	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool TryInteract();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool TraceForInteractable(FHitResult& OutHit) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CachedInteractionCamera;

	bool ResolveInteractionCamera();
	bool GetTraceStartEnd(FVector& OutStart, FVector& OutEnd) const;

	void ScreenLog(const FString& Msg, float Time = 1.5f) const;
};
