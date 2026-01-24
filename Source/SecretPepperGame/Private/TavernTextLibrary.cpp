#include "TavernTextLibrary.h"

static const TArray<FText>& WarmLines()
{
	static const TArray<FText> Lines = {
		NSLOCTEXT("TavernText", "Warm_01", "They linger near the counter, pretending to study the bottles."),
		NSLOCTEXT("TavernText", "Warm_02", "They smile too quickly, then look away, embarrassed by it."),
		NSLOCTEXT("TavernText", "Warm_03", "Their shoulders relax a little when you notice them."),
		NSLOCTEXT("TavernText", "Warm_04", "They speak your name carefully, like it matters."),
		NSLOCTEXT("TavernText", "Warm_05", "They arrive like someone stepping out of the cold.")
	};
	return Lines;
}

static const TArray<FText>& NeutralLines()
{
	static const TArray<FText> Lines = {
		NSLOCTEXT("TavernText", "Neutral_01", "They seem focused, already halfway elsewhere."),
		NSLOCTEXT("TavernText", "Neutral_02", "They speak plainly, no more than needed."),
		NSLOCTEXT("TavernText", "Neutral_03", "Their voice is even, measured."),
		NSLOCTEXT("TavernText", "Neutral_04", "They meet your eyes without expectation."),
		NSLOCTEXT("TavernText", "Neutral_05", "They seem fine with silence.")
	};
	return Lines;
}

static const TArray<FText>& SharpLines()
{
	static const TArray<FText> Lines = {
		NSLOCTEXT("TavernText", "Sharp_01", "They watch the room carefully before approaching."),
		NSLOCTEXT("TavernText", "Sharp_02", "Their posture suggests they expect resistance."),
		NSLOCTEXT("TavernText", "Sharp_03", "They smile without warmth."),
		NSLOCTEXT("TavernText", "Sharp_04", "They speak crisply, testing boundaries."),
		NSLOCTEXT("TavernText", "Sharp_05", "They arrive armored in composure.")
	};
	return Lines;
}

static const TArray<FText>& PatronArchetypes()
{
	static const TArray<FText> Lines = {
		NSLOCTEXT("TavernText", "Patron_01", "A traveler with tired boots"),
		NSLOCTEXT("TavernText", "Patron_02", "A regular you do not remember meeting"),
		NSLOCTEXT("TavernText", "Patron_03", "Someone who smells of rain"),
		NSLOCTEXT("TavernText", "Patron_04", "A quiet figure nursing a secret"),
		NSLOCTEXT("TavernText", "Patron_05", "A stranger with familiar eyes")
	};
	return Lines;
}

TArray<FText> UTavernTextLibrary::GetPatronTensionLines(ETavernMood Mood)
{
	switch (Mood)
	{
	case ETavernMood::Warm:    return WarmLines();
	case ETavernMood::Neutral: return NeutralLines();
	case ETavernMood::Sharp:   return SharpLines();
	default:                   return NeutralLines();
	}
}

FText UTavernTextLibrary::GetRandomPatronTensionLine(ETavernMood Mood)
{
	const TArray<FText> Lines = GetPatronTensionLines(Mood);
	if (Lines.Num() == 0) return FText::GetEmpty();
	return Lines[FMath::RandRange(0, Lines.Num() - 1)];
}

TArray<FText> UTavernTextLibrary::GetPatronArchetypes()
{
	return PatronArchetypes();
}

FText UTavernTextLibrary::GetRandomPatronArchetype()
{
	const auto& Lines = PatronArchetypes();
	if (Lines.Num() == 0) return FText::GetEmpty();
	return Lines[FMath::RandRange(0, Lines.Num() - 1)];
}

