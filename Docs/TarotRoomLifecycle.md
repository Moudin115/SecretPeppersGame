# Tarot Tavern room lifecycle, first implementation

This is a deliberately small foundation for the card wall and daily room reset.
It does **not** replace the existing Blueprint room generators or door systems yet.

## Decisions for the current prototype

- The wall holds cards identified by the row name in `DT_MajorArcana`.
- A card can occupy one wall slot at a time. Placing it in another slot moves it.
- Card positions and assignments remain on the wall after the player sleeps.
- Temporary rooms and door connections disappear on sleep, after the player has returned to a safe permanent room.
- A new physical layout can be generated when the next night begins.
- The layout generator decides room types, positions, door connections, and randomness. The deck does not.
- There is no automatic SaveGame persistence or room generation in this commit.

## Blueprint integration

1. Use **Get Game Instance Subsystem**, class `TavernLayoutSubsystem`, from the wall widget or its controlling actor.
2. Call `PlaceCardInSlot(SlotIndex, CardRowName)` or `RemoveCardFromSlot(SlotIndex)`. Refresh the wall UI on `OnDeckChanged`.
3. Have the future layout Blueprint bind to `OnLayoutGenerationRequested`. Read `GetPlacedCards()` to decide which Major Arcana rooms to place. Do not assume the map returns cards in slot order.
4. Register every temporary room actor and any separately spawned temporary door actors with `RegisterTemporaryActor`. Never register the player, the permanent tavern, or permanently placed furniture.
5. For sleeping: FIRST return the player to a safe persistent part of the tavern, THEN call `ClearTemporaryRoomsForSleep()`, THEN perform the existing day and time progression logic. The deck assignments remain unchanged.
6. When entering the following night, call `RequestLayoutGeneration()` to request a fresh arrangement. This event does not generate actors without a listening Blueprint.

## Important limits and future changes

- An active card can be removed from the wall without immediately changing an already generated room. The current layout is cleared when sleeping.
- For level-streamed rooms, a streaming-aware cleanup adapter is needed; registering just a level actor is not sufficient.
- Persisting the deck across application restarts needs a SaveGame implementation.
- The number and visual arrangement of wall slots are still up to the designer.
- The daily reset rule may change without changing the wall data API.
- Keep the existing `BP_RoomGen`, `Dungeon_Generator`, `BP_Door_Master`, and other Blueprint assets until reference checks and functional tests are complete.
