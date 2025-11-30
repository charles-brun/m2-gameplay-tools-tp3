#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "CustomAssetAppMode.h"
#include "CustomGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "CustomGraphNode.h"

void FCustomAssetEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& TabManagerPtr)
{
	FWorkflowCentricApplication::RegisterTabSpawners(TabManagerPtr);
}

void FCustomAssetEditorApp::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& ToolkitHostPtr,
	UObject* Object)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(Object);

	WorkingAsset = Cast<UCustomAsset>(Object);

	WorkingGraph = FBlueprintEditorUtils::CreateNewGraph(
		WorkingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UCustomGraphSchema::StaticClass()
	);

	InitAssetEditor(
		Mode,
		ToolkitHostPtr,
		TEXT("CustomAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true, // create standalone menu
		true, // create default toolbar
		ObjectsToEdit
		);

	AddApplicationMode(TEXT("CustomAssetAppMode"), MakeShareable(new FCustomAssetAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("CustomAssetAppMode"));

	UpdateEditorGraphFromWorkingAsset();
	GraphChangeListenerHandler = WorkingGraph->AddOnGraphChangedHandler(
		FOnGraphChanged::FDelegate::CreateSP(this, &FCustomAssetEditorApp::OnGraphChanged));
}

void FCustomAssetEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	WorkingGraph->RemoveOnGraphChangedHandler(GraphChangeListenerHandler);
	FAssetEditorToolkit::OnClose();
}

void FCustomAssetEditorApp::OnGraphChanged(const FEdGraphEditAction& EditAction) const
{
	UpdateWorkingAssetFromGraph();
}

void FCustomAssetEditorApp::UpdateWorkingAssetFromGraph() const
{
	if (WorkingAsset == nullptr || WorkingGraph == nullptr)
	{
		return;
	}
	UCustomRuntimeGraph* RuntimeGraph = NewObject<UCustomRuntimeGraph>(WorkingAsset);
	WorkingAsset->Graph = RuntimeGraph;

	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UCustomRuntimePin*> IdToPinMap;

	for (UEdGraphNode* UiNode : WorkingGraph->Nodes) {
		UCustomRuntimeNode* RuntimeNode = NewObject<UCustomRuntimeNode>(RuntimeGraph);
		RuntimeNode->Position = FVector2D(UiNode->NodePosX, UiNode->NodePosY);

		for (UEdGraphPin* UiPin : UiNode->Pins) {
			UCustomRuntimePin* RuntimePin = NewObject<UCustomRuntimePin>(RuntimeNode);
			RuntimePin->PinName = UiPin->PinName;
			RuntimePin->PinId = UiPin->PinId;

			if (UiPin->HasAnyConnections() && UiPin->Direction == EGPD_Output) {
				std::pair<FGuid, FGuid> Connection = std::make_pair(UiPin->PinId, UiPin->LinkedTo[0]->PinId);
				Connections.Add(Connection);
			}
            
			IdToPinMap.Add(UiPin->PinId, RuntimePin);
			if (UiPin->Direction == EGPD_Input) {
				RuntimeNode->InputPin = RuntimePin;
			} else {
				RuntimeNode->OutputPins.Add(RuntimePin);
			}
		}

		RuntimeGraph->Nodes.Add(RuntimeNode);
	}

	for (std::pair<FGuid, FGuid> Connection : Connections) {
		UCustomRuntimePin* Pin1 = IdToPinMap[Connection.first];
		UCustomRuntimePin* Pin2 = IdToPinMap[Connection.second];
		Pin1->Connection = Pin2;
	}
}

void FCustomAssetEditorApp::UpdateEditorGraphFromWorkingAsset() const
{
	if (WorkingAsset->Graph == nullptr) {
		return;
	}
	TArray<std::pair<FGuid, FGuid>> Connections;
	TMap<FGuid, UEdGraphPin*> IdToPinMap;
	for (UCustomRuntimeNode* RuntimeNode : WorkingAsset->Graph->Nodes) {
		UCustomGraphNode* NewNode = NewObject<UCustomGraphNode>(WorkingGraph);
		NewNode->CreateNewGuid();
		NewNode->NodePosX = RuntimeNode->Position.X;
		NewNode->NodePosY = RuntimeNode->Position.Y;

		if (RuntimeNode->InputPin != nullptr) {
			UCustomRuntimePin* Pin = RuntimeNode->InputPin;
			UEdGraphPin* UiPin = NewNode->CreateCustomPin(EGPD_Input, Pin->PinName);
			UiPin->PinId = Pin->PinId;

			if (Pin->Connection != nullptr) {
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}
			IdToPinMap.Add(Pin->PinId, UiPin);
		}

		for (UCustomRuntimePin* Pin : RuntimeNode->OutputPins) {
			UEdGraphPin* UiPin = NewNode->CreateCustomPin(EGPD_Output, Pin->PinName);
			UiPin->PinId = Pin->PinId;
            
			if (Pin->Connection != nullptr) {
				Connections.Add(std::make_pair(Pin->PinId, Pin->Connection->PinId));
			}
			IdToPinMap.Add(Pin->PinId, UiPin);
		}

		WorkingGraph->AddNode(NewNode, true, true);
	}

	for (std::pair<FGuid, FGuid> connection : Connections) {
		UEdGraphPin* FromPin = IdToPinMap[connection.first];
		UEdGraphPin* ToPin = IdToPinMap[connection.second];
		FromPin->LinkedTo.Add(ToPin);
		ToPin->LinkedTo.Add(FromPin);
	}
}
