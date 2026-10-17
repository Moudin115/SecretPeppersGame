# Tarot Deck Wall, initial Blueprint integration

Cleanup #9 adds `UDeckWallComponent`, a small adapter between the existing Major Arcana Data Table and the `UTavernLayoutSubsystem` introduced in Cleanup #8. No Blueprint assets were altered.

## Setup in Unreal Editor

1. Open a **permanent** wall actor in the tavern, or create a new `BP_DeckWall` actor. Add **Deck Wall Component**. Do not attach it to a generated temporary room.
2. Select the component, set **Major Arcana Table** to `/Game/_GameContent/Blueprints/Systems/CardWalls/DT_MajorArcana`.
3. Set **Slot Count** to the number of card positions you actually want on the wall. The default 22 is only a starting value, not a required design.
4. Have the wall UI, potentially the existing `WBP_MasterWall`, receive a reference to the wall actor or its component. Verify the widget's current logic before reusing it.
5. Use `GetDefinedMajorArcanaIds` to read the Data Table's row names. These are defined cards, **not** a list of cards unlocked by the player.
6. When a card is placed, call `PlaceMajorArcanaInSlot(SlotIndex, CardRowName)`. When removed, call `RemoveCardFromWallSlot(SlotIndex)`. Read `GetWallCardsInSlotOrder` to draw the current wall arrangement.
7. Bind the widget to `OnWallChanged` to refresh its display. The wall component forwards the deck subsystem's event, including changes made elsewhere.

The wall component validates card IDs against the Data Table and prevents placing cards outside the configured number of slots. The underlying subsystem keeps cards unique, moving a card if placed in another slot.

## Room lifecycle

Placing or removing a card changes the deck, but does not immediately spawn or destroy rooms. When a night starts, the future room generator should receive `OnLayoutGenerationRequested` and use `GetPlacedCards()` from the layout subsystem to generate a new arrangement. When the player sleeps, move them to a safe permanent room and call `ClearTemporaryRoomsForSleep()`. Cards remain on the wall.

## Still to implement

- The actual wall widget visuals and player interaction.
- Player-owned and unlocked cards, if desired. Defined Data Table rows alone are **not** proof of ownership.
- Real room geometry generation, physical door connections, and randomized placement.
- SaveGame persistence across application restarts.
- Explicit new-run cleanup of deck state, if required.
- Blueprint audit before removing legacy room generators or widgets.

Compile in Unreal Engine 5.5 and test with a temporary UI before merging the branch.
