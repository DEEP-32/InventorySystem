# Inventory Plugin Architecture Overview

This document summarizes the core classes in the Inventory plugin, how they relate, and where the most important runtime events occur (useful breakpoints). A class diagram (PlantUML) is provided alongside this document: `Plugins/Inventory/Docs/Inventory_UML.puml`.


## Big Picture
- World items are represented by components placed on actors: `UInv_ItemComponent`.
- The data for an item (type, category, fragments like size/icon/stacking) is held in an `FInv_ItemManifest`.
- When a player picks up an item, the item is converted into an inventory entry (`UInv_InventoryItem`) and stored in a replicated fast array: `FInv_InventoryFastArray` inside `UInv_InventoryComponent`.
- The UI (e.g., `UInv_InventoryBase` → `UInv_InventoryGrid`, `UInv_GridSlots`, `UInv_SlottedItems`) queries capacity, renders items into a spatial grid, and updates stacks.
- Multiplayer: replication uses UE’s Fast Array Serializer so that adds/removes and delta updates are efficiently replicated from the server to clients.


## Key Classes and Responsibilities

### UInv_ItemComponent (world-side item component)
- File: `Source/Inventory/Public/Items/Components/Inv_ItemComponent.h`
- Role: Attach to an actor in the world to make it an inventory-pickup item.
- Holds a replicated `FInv_ItemManifest` and a pickup message.
- Important methods:
  - `GetItemManifest()`: returns the manifest data (category/type/fragments).
  - `Pickup()`: triggers `OnPickedUp()` (Blueprint event) and destroys the owner actor.

### FInv_ItemManifest (data defining an item)
- File: `Source/Inventory/Public/Items/Manifest/Inv_ItemManifest.h`
- Role: Data container for item definition.
- Fields: `ItemCategory` (EInv_ItemCategory), `ItemType` (GameplayTag), array of fragments (`TInstancedStruct<FInv_ItemFragment>`).
- Provides typed accessors:
  - `GetFragmentOfType<T>()`, `GetFragmentOfTypeWithTag<T>(Tag)`, and mutable variant.
- `Manifest(UObject* NewOuter)`: builds a runtime `UInv_InventoryItem` from the data (used when adding into the inventory fast array).

### Item Fragments (composable item traits)
- File: `Source/Inventory/Public/Items/Fragments/Inv_ItemFragment.h`
- `FInv_ItemFragment`: base fragment with `FragmentTag` (GameplayTag) for filtering.
- `FInv_GridFragment`: grid footprint/padding for spatial inventory; `GridSize`, `GridPadding`.
- `FInv_ImageFragment`: icon and icon dimensions; `Icon`, `IconDimensions`.
- `FInv_StackableFragment`: stacking rules; `MaxStackSize`, current `StackCount`.

### UInv_InventoryItem (inventory-side item object)
- File: `Source/Inventory/Public/Items/Inv_InventoryItem.h`
- Role: UObject that represents a single item inside the inventory fast array. Contains an `FInstancedStruct` of `FInv_ItemManifest` and a replicated `TotalStackCount`.
- Helpers: `IsSameItemType(Tag)`, `IsStackable()`, `GetTotalStackCount()`, `SetTotalStackCount()`.

### UInv_InventoryComponent (player inventory + replication)
- Files:
  - `Public/InventoryManagement/Components/Inv_InventoryComponent.h`
  - `Private/InventoryManagement/Components/Inv_InventoryComponent.cpp`
- Role: Actor component on the player that owns and replicates the inventory list and runs the UI.
- Holds:
  - `FInv_InventoryFastArray InventoryList` (Replicated)
  - `UInv_InventoryBase* InventoryMenu` (UI root)
- Important API and flow:
  - `TryAddItem(UInv_ItemComponent* ItemComponent)`
    - Asks UI (`InventoryMenu->HasRoomForItem`) for capacity/stacking layout.
    - Locates existing item of same type: `InventoryList.FindFirstItemByType`.
    - Emits events and calls server RPCs to add new or stack existing items.
  - Server RPCs (authority-side):
    - `Server_AddNewItem(ItemComponent, StackCount)`
      - `InventoryList.AddEntry(ItemComponent)` converts and appends a `UInv_InventoryItem`.
      - Sets `TotalStackCount`, broadcasts `OnItemAdded`, calls `ItemComponent->Pickup()`.
    - `Server_AddStackItem(ItemComponent, StackCount, Remainder)`
      - Finds item by type, updates stack counts, and either picks up or updates remainder on the pickup actor.
  - UI management: toggling menu, creating `UInv_InventoryBase` at BeginPlay for local controllers.
  - Delegates: `OnItemAdded`, `OnItemRemoved`, `OnNoRoomInInventory`, `OnStackChange`.

### FInv_InventoryFastArray (FastArraySerializer for items)
- Files:
  - `Public/InventoryManagement/FastArray/Inv_FastArray.h`
  - `Private/InventoryManagement/FastArray/Inv_FastArray.cpp`
- Role: Efficient replication of array changes.
- Structs:
  - `FInv_InventoryEntry : FFastArraySerializerItem` – holds `UInv_InventoryItem* Item`.
  - `FInv_InventoryFastArray : FFastArraySerializer` – owns `TArray<FInv_InventoryEntry> ItemEntries` and the non-replicated `OwningComponent`.
- Key methods:
  - `AddEntry(UInv_ItemComponent*)` – creates a `UInv_InventoryItem` from a manifest and marks dirty.
  - `AddEntry(UInv_InventoryItem*)` – adds an already-made item.
  - `RemovedEntry(...)` – removes and marks array dirty.
  - `FindFirstItemByType(...)` – helper for stacking.
  - Replication hooks: `PreReplicatedRemove`, `PostReplicatedAdd` broadcast component delegates.

### UI: Inventory Menu and Grid
- `UInv_InventoryBase` (abstract base UI)
  - File: `Public/Widgets/Inventory/Base/Inv_InventoryBase.h`
  - Defines virtual `HasRoomForItem(UInv_ItemComponent*)` which concrete widgets implement.

- `UInv_InventoryGrid` (spatial grid UI)
  - Files:
    - `Public/Widgets/Inventory/Spatial/Inv_InventoryGrid.h`
    - `Private/Widgets/Inventory/Spatial/Inv_InventoryGrid.cpp`
  - Role: Grid layout, grid-slot state, determining placement for new items and stacks.
  - Important responsibilities:
    - `HasRoomForItem(...)` overloads: compute available indices, stacking info, remainders based on `FInv_GridFragment` and `FInv_StackableFragment`.
    - `AddItem(...)`, `AddStacks(...)`: add visuals/stack counts and update `GridSlots` internal states.
    - `UpdateGridSlots(...)`: update slot occupancy and state based on item dimensions.
    - Creating slotted widgets: `CreateSlottedItem(...)`, `SetSlottedItemImage(...)`, `AddSlottedItemToCanvas(...)`.
  - Key data:
    - `TArray<UInv_GridSlots*> GridSlots` – the cells.
    - `TMap<int32, UInv_SlottedItems*> SlottedItems` – actual item widgets keyed by the origin index.

- `UInv_GridSlots` (grid cell widget)
  - File: `Public/Widgets/Inventory/GridSlots/Inv_GridSlots.h`
  - Role: Per-cell state: `Unoccupied`, `Occupied`, `Selected`, `GreyedOut`. Holds `Index`, `OriginSlotIndex` (to track the original anchor cell for multi-cell items), optional `InventoryItem`, and `StackCount` for display.

- `UInv_SlottedItems` (rendered item in the grid)
  - Files:
    - `Public/Widgets/Inventory/SlottedItem/Inv_SlottedItems.h`
    - `Private/Widgets/Inventory/SlottedItem/Inv_SlottedItems.cpp`
  - Role: Displays the item’s image and stack count at a given grid index and dimension. Knows whether item is stackable.


## Typical Flow (Pickup → Inventory → UI)
1. Player overlaps an actor with `UInv_ItemComponent`.
2. Game calls `UInv_InventoryComponent::TryAddItem(ItemComponent)` on the player’s inventory.
3. UI capacity check: `InventoryMenu->HasRoomForItem(ItemComponent)` (implemented by e.g., `UInv_InventoryGrid`).
4. If stacking is possible, fire `OnStackChange` and call `Server_AddStackItem`; else call `Server_AddNewItem`.
5. Server adds item to `FInv_InventoryFastArray` via `AddEntry(...)`, marking the fast array dirty for replication.
6. On clients, replicated changes trigger `PostReplicatedAdd`/`PreReplicatedRemove` which broadcast UI update delegates (`OnItemAdded`/`OnItemRemoved`).
7. UI widgets update visuals: create `UInv_SlottedItems`, set images from `FInv_ImageFragment`, size from `FInv_GridFragment`, and update `UInv_GridSlots` states.


## Recommended Breakpoints (most important runtime points)

Server authority (gameplay/replication):
- `UInv_InventoryComponent::TryAddItem`
  - Before room check: verify category/type extraction.
  - After `HasRoomForItem`: inspect `FInv_SlotAvailabilityResult`.
- `UInv_InventoryComponent::Server_AddNewItem`
  - After `AddEntry(ItemComponent)`: item creation from manifest; check `TotalStackCount`.
- `UInv_InventoryComponent::Server_AddStackItem`
  - After finding existing item: verify stack math and `Remainder` behavior.
- `FInv_InventoryFastArray::AddEntry(UInv_ItemComponent*)`
  - At `Manifest(Owner)`: conversion world→inventory item.
  - After `MarkItemDirty(NewEntry)`: ensure replication will happen.
- `FInv_InventoryFastArray::RemovedEntry`
  - After `MarkArrayDirty()`: ensure removal replicates.

Client/UI (layout and rendering):
- `UInv_InventoryGrid::HasRoomForItem` (both overloads)
  - When computing `SlotAvailabilities`: confirm indices and fill amounts.
- `UInv_InventoryGrid::AddStacks`
  - At branch `if (SlotAvailability.bItemAtIndex)`: validate stack increments.
- `UInv_InventoryGrid::AddItem` and `UpdateGridSlots`
  - When adding first visual for a new item; ensures `GridSlots` and `SlottedItems` map are consistent.
- `UInv_InventoryGrid::CreateSlottedItem` / `SetSlottedItemImage` / `AddSlottedItemToCanvas`
  - Validate brush, sizing, and canvas slot position (image/icon issues).
- `UInv_ItemComponent::Pickup`
  - To confirm pickup actor gets destroyed at the right time.

Replication events (client):
- `FInv_InventoryFastArray::PostReplicatedAdd`
  - Confirm `OnItemAdded` is broadcast and UI reacts.
- `FInv_InventoryFastArray::PreReplicatedRemove`
  - Confirm `OnItemRemoved` is broadcast and UI cleans up.


## Multiplayer Notes (Fast Array)
- `FInv_InventoryFastArray` implements `NetDeltaSerialize` and marks entries/items dirty as changes occur.
- The server owns the inventory; clients receive delta updates. UI reacts via the component’s delegates.
- `UInv_InventoryComponent::AddRepSubObj` registers newly created `UInv_InventoryItem` subobjects for replication, required for UObject-in-fast-array patterns.


## How to open the UML
- Install PlantUML support in Rider (or use any PlantUML viewer/site).
- Open `Plugins/Inventory/Docs/Inventory_UML.puml`.
- Render to see classes and their relationships.

