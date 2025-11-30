#include "CustomAssetPropertiesTabFactory.h"
#include "CustomAssetEditorApp.h"
#include "CustomAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"

FCustomAssetPropertiesTabFactory::FCustomAssetPropertiesTabFactory(TSharedPtr<class FCustomAssetEditorApp> App) :
FWorkflowTabFactory("CustomAssetPropertiesTab", App)
{
	this->App = App;
	TabLabel = FText::FromString(TEXT("Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Asset properties"));
	ViewMenuTooltip = FText::FromString(TEXT("Asset properties"));
}

TSharedRef<SWidget> FCustomAssetPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FCustomAssetEditorApp> AppPtr = App.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowOptions = true;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowScrollBar = false;
	}

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(AppPtr->GetWorkingAsset());

	return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(HAlign_Fill)
				[
					DetailsView.ToSharedRef()
				];
}

FText FCustomAssetPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("Asset properties"));
}
