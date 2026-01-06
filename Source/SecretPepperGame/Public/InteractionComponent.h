#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SECRETPEPPERGAME_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	float TraceDistance = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	bool bDrawDebug = true;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool TryInteract();

	UFUNCTION(BlueprintCallable, Category = "Interact")
	bool TraceForInteractable(FHitResult& OutHit) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	TObjectPtr<class UCameraComponent> InteractionCamera;


protected:
	virtual void BeginPlay() override;

private:
	APawn* GetOwningPawn() const;
	bool GetViewPoint(FVector& OutLoc, FRotator& OutRot) const;
};
