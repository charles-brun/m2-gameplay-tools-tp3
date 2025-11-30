#include "CustomGraphNode.h"
#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"

void UCustomGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Custom Node Actions")));

	UCustomGraphNode* Node = (UCustomGraphNode*)this;
	Section.AddMenuEntry(
		TEXT("AddPinEntry"),
		FText::FromString(TEXT("Add Pin")),
		FText::FromString(TEXT("Adds a new Pin")),
		FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] ()
			{
				Node->CreateCustomPin(
					EGPD_Output,
					TEXT("Output")
				);

				Node->GetGraph()->NotifyGraphChanged();
				Node->GetGraph()->Modify();
			}
		))
	);
	Section.AddMenuEntry(
		TEXT("DeletePinEntry"),
		FText::FromString(TEXT("Delete Pin")),
		FText::FromString(TEXT("Deletes a Pin")),
		FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeDeletePinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] ()
			{
				UEdGraphPin* Pin = Node->GetPinAt(Node->Pins.Num() -1);
				if (Pin->Direction != EGPD_Input)
				{
					Node->RemovePin(Pin);
					Node->GetGraph()->NotifyGraphChanged();
					Node->GetGraph()->Modify();
				}

			}
		))
	);
	Section.AddMenuEntry(
		TEXT("DeleteNodeEntry"),
		FText::FromString(TEXT("Delete Node")),
		FText::FromString(TEXT("Deletes the Node")),
		FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeDeleteNodeIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[Node] ()
			{
				Node->GetGraph()->RemoveNode(Node);
			}
		))
	);
}

UEdGraphPin* UCustomGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName Name)
{
	FName Category = (Direction == EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
	FName SubCategory = TEXT("CustomPin");

	UEdGraphPin* Pin = CreatePin(Direction, Category, Name);
	Pin->PinType.PinSubCategory = SubCategory;

	return Pin;
}
