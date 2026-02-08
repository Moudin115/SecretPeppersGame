// TavernTextLibrary.cpp

#include "TavernTextLibrary.h"
#include "Math/UnrealMathUtility.h"

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

// Single rumor pool, includes tarot foreshadow and shy, hungry ghost
static const TArray<FText>& RumorLines()
{
	static const TArray<FText> Lines = {
		NSLOCTEXT("TavernText", "Rumor_01", "Nobody here talks about the outside, like the world ends at your door."),
		NSLOCTEXT("TavernText", "Rumor_02", "Some guests arrive soaked, but their boots are dry."),
		NSLOCTEXT("TavernText", "Rumor_03", "They say the roads forget themselves, you can walk out sure, and walk back lost."),
		NSLOCTEXT("TavernText", "Rumor_04", "A regular swears the same song starts every night, even when nobody plays it."),
		NSLOCTEXT("TavernText", "Rumor_05", "Someone keeps checking their pockets like they are counting paper instead of coin."),
		NSLOCTEXT("TavernText", "Rumor_06", "There is talk of a thin gift, left where a tip should be, ink black, edges sharp."),
		NSLOCTEXT("TavernText", "Rumor_07", "They whisper about a third morning, like it is a line you cross, not a date you reach."),
		NSLOCTEXT("TavernText", "Rumor_08", "A patron muttered, third time is when it starts, then refused to explain."),
		NSLOCTEXT("TavernText", "Rumor_09", "Some people leave nothing behind, others leave something that feels like a message."),
		NSLOCTEXT("TavernText", "Rumor_10", "They say a card has been showing up lately, face down, like it is listening."),
		NSLOCTEXT("TavernText", "Rumor_11", "At night, someone sits at the far table, very still, like they do not want to be noticed."),
		NSLOCTEXT("TavernText", "Rumor_12", "A bowl left out after closing is always empty by morning, though the door stays locked."),
		NSLOCTEXT("TavernText", "Rumor_13", "The candles lean toward the corner, gentle, the way flames lean toward breathing."),
		NSLOCTEXT("TavernText", "Rumor_14", "They say the ghost here is polite, it only comes when the room is quiet enough to feel safe."),
		NSLOCTEXT("TavernText", "Rumor_15", "Sometimes you hear a soft thank you with no voice attached, only warmth fading slowly.")
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

// Header declares mood-based rumors, but we intentionally use a single pool.
// Mood is ignored so your Blueprint calls stay stable.
TArray<FText> UTavernTextLibrary::GetRumorLines(ETavernMood Mood)
{
	(void)Mood;
	return RumorLines();
}

FText UTavernTextLibrary::GetRandomRumor(ETavernMood Mood)
{
	(void)Mood;

	const auto& Lines = RumorLines();
	if (Lines.Num() == 0) return FText::GetEmpty();
	return Lines[FMath::RandRange(0, Lines.Num() - 1)];
}

FText UTavernTextLibrary::GetRandomRumorNoRepeat(ETavernMood Mood, FText LastRumor, bool& bOutChanged)
{
	(void)Mood;
	bOutChanged = false;

	const auto& Lines = RumorLines();
	const int32 N = Lines.Num();
	if (N == 0) return FText::GetEmpty();
	if (N == 1)
	{
		bOutChanged = !Lines[0].EqualTo(LastRumor);
		return Lines[0];
	}

	int32 Tries = 6;
	while (Tries-- > 0)
	{
		const FText Pick = Lines[FMath::RandRange(0, N - 1)];
		if (!Pick.EqualTo(LastRumor))
		{
			bOutChanged = true;
			return Pick;
		}
	}

	return Lines[FMath::RandRange(0, N - 1)];
}
