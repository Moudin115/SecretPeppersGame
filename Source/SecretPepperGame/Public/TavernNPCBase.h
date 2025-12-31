// TavernNPCBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TavernSubsystem.h"
#include "Interactable.h"
#include "TavernNPCBase.generated.h"

UENUM(BlueprintType)
enum class ETavernNpcPresence : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Observing UMETA(DisplayName = "Observing"),
	Warm UMETA(DisplayName = "Warm"),
	Guarded UMETA(DisplayName = "Guarded"),
};

UCLASS()
class SECRETPEPPERGAME_API ATavernNPCBase : public ACharacter, public IInteractable
{
	GENERATED_BODY()

public:
	ATavernNPCBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Tavern")
	TObjectPtr<UTavernSubsystem> TavernSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tavern")
	ETavernNpcPresence Presence = ETavernNpcPresence::Observing;

	UFUNCTION()
	void HandleTimeChanged(ETavernTimeOfDay NewTime);

	UFUNCTION()
	void HandleVibeChanged(int32 NewVibe);

	UFUNCTION(BlueprintCallable, Category = "Tavern")
	void RefreshPresenceFromState();

	UFUNCTION(BlueprintImplementableEvent, Category = "Tavern")
	void BP_OnTimeChanged(ETavernTimeOfDay NewTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tavern")
	void BP_OnVibeChanged(int32 NewVibe);

	UFUNCTION(BlueprintImplementableEvent, Category = "Tavern")
	void BP_OnPresenceChanged(ETavernNpcPresence NewPresence);

public:
	virtual bool CanInteract_Implementation(APawn* Interactor) const override;
	virtual FText GetInteractPrompt_Implementation() const override;
	virtual void Interact_Implementation(APawn* Interactor) override;
};
