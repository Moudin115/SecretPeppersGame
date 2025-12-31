// Interactable.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(BlueprintType)
class SECRETPEPPERGAME_API UInteractable : public UInterface
{
	GENERATED_BODY()
};

class SECRETPEPPERGAME_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	bool CanInteract(APawn* Interactor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	FText GetInteractPrompt() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void Interact(APawn* Interactor);
};
