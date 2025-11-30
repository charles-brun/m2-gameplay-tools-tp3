# Custom Asset Editor (UE 5.5)

Plugin to create a custom asset with its own editor (properties + graph).

## How to use

Create a CustomAsset from the CustomAssets category in the UE editor.

Nodes can be created on the graph, and linked between them with restrictions (input<>output only, nodes cannot link to themselves).
An action menu (right click on the node) can remove the node or add/remove an output pin.

If the asset is saved, the event graph data is persisted after closing it.

## Evolutions

Logic has yet to be implemented behind the editor's properties and nodes.
Different types of nodes and pins should be added.
