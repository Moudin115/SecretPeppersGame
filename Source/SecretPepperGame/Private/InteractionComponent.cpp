#include "InteractionComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Interactable.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

APawn* UInteractionComponent::GetOwningPawn() const
{
	return Cast<APawn>(GetOwner());
}

bool UInteractionComponent::GetViewPoint(FVector& OutLoc, FRotator& OutRot) const
{
	APawn* Pawn = GetOwningPawn();
	if (!Pawn) return false;

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (PC)
	{
		PC->GetPlayerViewPoint(OutLoc, OutRot);
		return true;
	}

	Pawn->GetActorEyesViewPoint(OutLoc, OutRot);
	return true;
}

bool UInteractionComponent::TraceForInteractable(FHitResult& OutHit) const
{
	FVector ViewLoc;
	FRotator ViewRot;
	if (!GetViewPoint(ViewLoc, ViewRot)) return false;

	const FVector Start = ViewLoc;
	const FVector End = Start + (ViewRot.Vector() * TraceDistance);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), true, GetOwner());
	Params.bReturnPhysicalMaterial = false;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, Params);

	if (bDrawDebug)
	{
		const FColor Col = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Col, false, 1.0f, 0, 1.5f);
		if (bHit)
		{
			DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 8.f, 12, Col, false, 1.0f);
		}
	}

	return bHit && OutHit.GetActor() != nullptr;
}

bool UInteractionComponent::TryInteract()
{
	APawn* Pawn = GetOwningPawn();
	if (!Pawn) return false;

	UE_LOG(LogTemp, Log, TEXT("IA_Interact pressed, owner %s"), *GetOwner()->GetName());

	FHitResult Hit;
	if (!TraceForInteractable(Hit))
	{
		UE_LOG(LogTemp, Log, TEXT("Interact trace, NO HIT"));
		return false;
	}

	AActor* HitActor = Hit.GetActor();
	UPrimitiveComponent* HitComp = Hit.GetComponent();

	UE_LOG(LogTemp, Log, TEXT("Interact trace, HIT Actor %s, Comp %s"),
		HitActor ? *HitActor->GetName() : TEXT("None"),
		HitComp ? *HitComp->GetName() : TEXT("None"));

	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		const bool bCan = IInteractable::Execute_CanInteract(HitActor, Pawn);
		UE_LOG(LogTemp, Log, TEXT("Hit actor implements Interactable, CanInteract %s"), bCan ? TEXT("true") : TEXT("false"));

		if (!bCan) return false;

		IInteractable::Execute_Interact(HitActor, Pawn);
		UE_LOG(LogTemp, Log, TEXT("Interact called on %s"), *HitActor->GetName());
		return true;
	}

	if (HitComp && HitComp->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		const bool bCan = IInteractable::Execute_CanInteract(HitComp, Pawn);
		UE_LOG(LogTemp, Log, TEXT("Hit component implements Interactable, CanInteract %s"), bCan ? TEXT("true") : TEXT("false"));

		if (!bCan) return false;

		IInteractable::Execute_Interact(HitComp, Pawn);
		UE_LOG(LogTemp, Log, TEXT("Interact called on component %s"), *HitComp->GetName());
		return true;
	}

	UE_LOG(LogTemp, Log, TEXT("Hit something, but it does not implement Interactable"));
	return false;
}
