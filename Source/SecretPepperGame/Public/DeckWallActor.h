#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "DeckWallActor.generated.h"

class UBoxComponent;
class UDeckWallComponent;
class UDeckWallWidget;
class APawn;

// Minimal placeable actor for testing the Deck Wall with the existing
// interaction trace. Designers can subclass it to add wall meshes and effects.
UCLASS(Blueprintable)
class SECRETPEPPERGAME_API ADeckWallActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ADeckWallActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tavern|Deck Wall")
	TObjectPtr<UBoxComponent> InteractionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tavern|Deck Wall")
	TObjectPtr<UDeckWallComponent> DeckWallComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tavern|Deck Wall")
	TSubclassOf<UDeckWallWidget> WidgetClass;

	virtual bool CanInteract_Implementation(APawn* Interactor) const override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual void Interact_Implementation(APawn* Interactor) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UDeckWallWidget> ActiveWidget;
};
