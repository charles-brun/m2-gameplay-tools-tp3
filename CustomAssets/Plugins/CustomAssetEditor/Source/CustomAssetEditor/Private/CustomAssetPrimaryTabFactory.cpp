#include "CustomAssetPrimaryTabFactory.h"
#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

FCustomAssetPrimaryTabFactory::FCustomAssetPrimaryTabFactory(TSharedPtr<class FCustomAssetEditorApp> App) :
FWorkflowTabFactory("CustomAssetPrimaryTab", App)
{
	this->App = App;
	TabLabel = FText::FromString(TEXT("Primary"));
	ViewMenuDescription = FText::FromString(TEXT("View Menu"));
	ViewMenuTooltip = FText::FromString(TEXT("View Menu Tooltip"));
}

TSharedRef<SWidget> FCustomAssetPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FCustomAssetEditorApp> AppPtr = App.Pin();

	return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(HAlign_Fill)
				[
					SNew(SGraphEditor)
						.IsEditable(true)
						.GraphToEdit(AppPtr->GetWorkingGraph())
				];
}

FText FCustomAssetPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Text Widget View"));
}
