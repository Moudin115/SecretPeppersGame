// TavernNPCBase.cpp
#include "TavernNPCBase.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"

ATavernNPCBase::ATavernNPCBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATavernNPCBase::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GI = GetGameInstance())
	{
		TavernSubsystem = GI->GetSubsystem<UTavernSubsystem>();
	}

	if (TavernSubsystem)
	{
		TavernSubsystem->OnTimeChanged.AddDynamic(this, &ATavernNPCBase::HandleTimeChanged);
		TavernSubsystem->OnVibeChanged.AddDynamic(this, &ATavernNPCBase::HandleVibeChanged);

		RefreshPresenceFromState();
	}
}

void ATavernNPCBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TavernSubsystem)
	{
		TavernSubsystem->OnTimeChanged.RemoveDynamic(this, &ATavernNPCBase::HandleTimeChanged);
		TavernSubsystem->OnVibeChanged.RemoveDynamic(this, &ATavernNPCBase::HandleVibeChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void ATavernNPCBase::HandleTimeChanged(ETavernTimeOfDay NewTime)
{
	BP_OnTimeChanged(NewTime);
	RefreshPresenceFromState();
}

void ATavernNPCBase::HandleVibeChanged(int32 NewVibe)
{
	BP_OnVibeChanged(NewVibe);
	RefreshPresenceFromState();
}

void ATavernNPCBase::RefreshPresenceFromState()
{
	if (!TavernSubsystem) return;

	const FTavernState S = TavernSubsystem->GetState();

	ETavernNpcPresence NewPresence = ETavernNpcPresence::Observing;

	const bool bNight = (S.TimeOfDay == ETavernTimeOfDay::Night);

	if (bNight && S.Vibe <= 0)
	{
		NewPresence = ETavernNpcPresence::Guarded;
	}
	else if (!bNight && S.Vibe >= 4)
	{
		NewPresence = ETavernNpcPresence::Warm;
	}
	else if (bNight && S.Vibe >= 6)
	{
		NewPresence = ETavernNpcPresence::Warm;
	}
	else
	{
		NewPresence = ETavernNpcPresence::Observing;
	}

	if (Presence != NewPresence)
	{
		Presence = NewPresence;
		BP_OnPresenceChanged(Presence);
	}
}

bool ATavernNPCBase::CanInteract_Implementation(APawn* Interactor) const
{
	return true;
}

FText ATavernNPCBase::GetInteractPrompt_Implementation() const
{
	return FText::FromString(TEXT("Talk"));
}

void ATavernNPCBase::Interact_Implementation(APawn* Interactor)
{
	// Keep this empty in C++ for now, implement in Blueprint by overriding Interact on the NPC blueprint if you want,
	// or call a BP event from here later when you have a dialogue widget.
}


