#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FCustomAssetEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& ToolkitHost, UObject* Object);

	class UCustomAsset* GetWorkingAsset() const { return WorkingAsset; };
	class UEdGraph* GetWorkingGraph() const { return WorkingGraph; };
	
	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("CustomAssetEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("CustomAssetEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("CustomAssetEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Gray; }
	virtual FString GetDocumentationLink() const override { return TEXT(""); }
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override {}

	virtual void OnClose() override;
	void OnGraphChanged(const FEdGraphEditAction& EditAction) const; 

protected:
	void UpdateWorkingAssetFromGraph() const;
	void UpdateEditorGraphFromWorkingAsset() const;
	
private:
	UPROPERTY()
	class UCustomAsset* WorkingAsset = nullptr;

	UPROPERTY()
	class UEdGraph* WorkingGraph = nullptr;

	FDelegateHandle GraphChangeListenerHandler;
};
