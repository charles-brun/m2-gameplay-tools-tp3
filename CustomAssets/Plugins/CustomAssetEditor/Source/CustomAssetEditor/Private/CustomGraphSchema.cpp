#include "CustomGraphSchema.h"
#include "CustomGraphNode.h"

void UCustomGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FNewNodeAction> NewNodeAction(
		new FNewNodeAction(
			FText::FromString(TEXT("Nodes")),
			FText::FromString(TEXT("New Node")),
			FText::FromString(TEXT("Add a new node")),
			0
		)
	);

	ContextMenuBuilder.AddAction(NewNodeAction);
}

const FPinConnectionResponse UCustomGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pin"));;
	}
	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Input/Output mismatch"));
	}
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Connect to another node"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}

UEdGraphNode* FNewNodeAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location,
                                            bool bSelectNewNode)
{
	UCustomGraphNode* Result = NewObject<UCustomGraphNode>(ParentGraph);
	Result->CreateNewGuid();
	Result->NodePosX = Location.X;
	Result->NodePosY = Location.Y;

	UEdGraphPin* InputPin = Result->CreateCustomPin(EGPD_Input, TEXT("Input"));
	Result->CreateCustomPin(EGPD_Output, TEXT("Output"));
	Result->CreateCustomPin(EGPD_Output, TEXT("Output"));

	if (FromPin != nullptr)
	{
		Result->GetSchema()->TryCreateConnection(FromPin, InputPin);
	}
	
	ParentGraph->Modify();
	ParentGraph->AddNode(Result, true, true);

	return Result;
}
