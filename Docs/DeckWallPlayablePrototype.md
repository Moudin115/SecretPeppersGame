# Deck Wall playable prototype (Cleanup #10)

This is a small, functional C++ UI for testing card placement without changing or deleting any of the existing Blueprint widgets.

## Quick test in UE 5.5

1. Fetch and pull the current cleanup branch with GitHub Desktop. Rebuild the project in Unreal or the IDE.
2. Create a Blueprint subclass of the C++ class `DeckWallActor`, e.g. `BP_DeckWall_Test`.
3. Select its `DeckWallComponent`. Assign `DT_MajorArcana` from `/Game/_GameContent/Blueprints/Systems/CardWalls` to **Major Arcana Table** and set **Slot Count**.
4. Place `BP_DeckWall_Test` in the permanent tavern or a test map. Its root `InteractionBox` is traceable on the Visibility channel. Add an appropriate wall mesh if desired. **Check that your player's InteractionComponent TraceChannel matches the box's blocked channel.**
5. In Play In Editor, look at the box and press your normal interaction input (E in the current setup). A dark temporary UI opens.
6. Click Previous/Next Card to select a defined Major Arcana Data Table row, Previous/Next Slot to choose the destination, and Place Card or Remove from Slot. The Current Wall list should change immediately.
7. Click Close to restore game-only input. Reopen the wall to confirm card placement remains. The deck also survives the temporary-room clear invoked when the player sleeps.

## Behavior and safety

- Row names from the Data Table are shown alphabetically. This does not implement card inventory/unlocking. All rows are selectable for this **prototype only**.
- Placing a card into another slot moves it rather than duplicating it. Placing onto an occupied slot replaces the previous occupant.
- The UI controls **deck data only**. It does not spawn rooms, rearrange the layout, or automatically execute the sleep transition.
- The included widget is intentionally plain and generated in C++. The designer's `WBP_MasterWall` and other widgets are unchanged. A custom derived Widget Blueprint can be assigned later via the actor's `WidgetClass` property.
- The wall actor has a simple interaction box but no automatically generated mesh. It must be placed in a playable level.
- The prototype assumes one local player and restores game-only input when closed. Integrating with an existing pause/menu/input-mode manager will require an explicit handoff.
- The **DeckWallActor** handles the same `Interactable` interface used by `InteractionComponent`; verify a competing actor or mesh collision is not intercepting the interaction trace.
- No existing Blueprints or game maps are removed or modified. Test and compile in UE 5.5 before merging.
