#include "InteractionComponent.h"

#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include "Interactable.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	ResolveInteractionCamera();
}

void UInteractionComponent::ScreenLog(const FString& Msg, float Time) const
{
	if (!bLogToScreen) return;
	if (!GEngine) return;

	GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Cyan, Msg);
}

bool UInteractionComponent::ResolveInteractionCamera()
{
	CachedInteractionCamera = nullptr;

	AActor* Owner = GetOwner();
	if (!Owner) return false;

	TArray<UCameraComponent*> Cameras;
	Owner->GetComponents<UCameraComponent>(Cameras);

	for (UCameraComponent* Cam : Cameras)
	{
		if (Cam && Cam->ComponentHasTag(InteractionCameraTag))
		{
			CachedInteractionCamera = Cam;
			UE_LOG(LogTemp, Log, TEXT("InteractionComponent, using camera %s (tag %s)"),
				*Cam->GetName(), *InteractionCameraTag.ToString());
			ScreenLog(FString::Printf(TEXT("Interaction camera, %s"), *Cam->GetName()));
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("InteractionComponent, no camera with tag %s found, will fall back to player view"),
		*InteractionCameraTag.ToString());
	ScreenLog(FString::Printf(TEXT("No tagged camera, using player view")));
	return false;
}

bool UInteractionComponent::GetTraceStartEnd(FVector& OutStart, FVector& OutEnd) const
{
	if (CachedInteractionCamera)
	{
		OutStart = CachedInteractionCamera->GetComponentLocation();
		OutEnd = OutStart + (CachedInteractionCamera->GetForwardVector() * TraceDistance);
		return true;
	}

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner) return false;

	APlayerController* PC = Cast<APlayerController>(PawnOwner->GetController());
	FRotator ViewRot;

	if (PC)
	{
		PC->GetPlayerViewPoint(OutStart, ViewRot);
		OutEnd = OutStart + (ViewRot.Vector() * TraceDistance);
		return true;
	}

	PawnOwner->GetActorEyesViewPoint(OutStart, ViewRot);
	OutEnd = OutStart + (ViewRot.Vector() * TraceDistance);
	return true;
}

bool UInteractionComponent::TraceForInteractable(FHitResult& OutHit) const
{
	FVector Start;
	FVector End;
	if (!GetTraceStartEnd(Start, End)) return false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), true, GetOwner());
	Params.bReturnPhysicalMaterial = false;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, Params);

	if (bDrawDebug)
	{
		const FColor Col = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, Col, false, 1.0f, 0, 1.5f);
		if (bHit)
		{
			DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 8.0f, 12, Col, false, 1.0f);
		}
	}

	return bHit && (OutHit.GetActor() != nullptr);
}

bool UInteractionComponent::TryInteract()
{
	UE_LOG(LogTemp, Log, TEXT("Interact pressed, owner %s"), GetOwner() ? *GetOwner()->GetName() : TEXT("None"));
	ScreenLog(TEXT("Interact pressed"));

	FHitResult Hit;
	if (!TraceForInteractable(Hit))
	{
		UE_LOG(LogTemp, Log, TEXT("Interact trace, no hit"));
		ScreenLog(TEXT("No hit"));
		return false;
	}

	AActor* HitActor = Hit.GetActor();
	UPrimitiveComponent* HitComp = Hit.GetComponent();

	UE_LOG(LogTemp, Log, TEXT("Interact hit, actor %s, component %s"),
		HitActor ? *HitActor->GetName() : TEXT("None"),
		HitComp ? *HitComp->GetName() : TEXT("None"));

	if (HitActor && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		APawn* Interactor = Cast<APawn>(GetOwner());
		const bool bCan = IInteractable::Execute_CanInteract(HitActor, Interactor);
		if (!bCan)
		{
			UE_LOG(LogTemp, Log, TEXT("CanInteract false on actor %s"), *HitActor->GetName());
			ScreenLog(TEXT("CanInteract false"));
			return false;
		}

		IInteractable::Execute_Interact(HitActor, Interactor);
		UE_LOG(LogTemp, Log, TEXT("Interact called on actor %s"), *HitActor->GetName());
		ScreenLog(FString::Printf(TEXT("Interacted with, %s"), *HitActor->GetName()));
		return true;
	}

	if (HitComp && HitComp->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		APawn* Interactor = Cast<APawn>(GetOwner());
		const bool bCan = IInteractable::Execute_CanInteract(HitComp, Interactor);
		if (!bCan)
		{
			UE_LOG(LogTemp, Log, TEXT("CanInteract false on component %s"), *HitComp->GetName());
			ScreenLog(TEXT("CanInteract false"));
			return false;
		}

		IInteractable::Execute_Interact(HitComp, Interactor);
		UE_LOG(LogTemp, Log, TEXT("Interact called on component %s"), *HitComp->GetName());
		ScreenLog(FString::Printf(TEXT("Interacted with component, %s"), *HitComp->GetName()));
		return true;
	}

	UE_LOG(LogTemp, Log, TEXT("Hit something, but it does not implement Interactable"));
	ScreenLog(TEXT("Hit non interactable"));
	return false;
}
