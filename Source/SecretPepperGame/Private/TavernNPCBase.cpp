// TavernNPCBase.cpp
#include "TavernNPCBase.h"

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
		TavernSubsystem->OnTimeChanged.AddUniqueDynamic(this, &ATavernNPCBase::HandleTimeChanged);
		TavernSubsystem->OnVibeChanged.AddUniqueDynamic(this, &ATavernNPCBase::HandleVibeChanged);

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

	const FTavernState& S = TavernSubsystem->GetState();

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
	return NSLOCTEXT("TavernNPC", "InteractTalk", "Talk");
}

void ATavernNPCBase::Interact_Implementation(APawn* Interactor)
{
	BP_OnInteract(Interactor);
}


