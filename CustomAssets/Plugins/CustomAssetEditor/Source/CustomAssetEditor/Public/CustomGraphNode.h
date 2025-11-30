#pragma once

#include "EdGraph/EdGraphNode.h"
#include "CustomGraphNode.generated.h"

UCLASS()
class UCustomGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Custom Node Title"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Blue; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName Name);
};