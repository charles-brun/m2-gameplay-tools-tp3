#include "CustomAssetAppMode.h"
#include "CustomAssetEditorApp.h"
#include "CustomAssetPrimaryTabFactory.h"
#include "CustomAssetPropertiesTabFactory.h"

FCustomAssetAppMode::FCustomAssetAppMode(TSharedPtr<class FCustomAssetEditorApp> App) :
	FApplicationMode(TEXT("CustomAssetAppMode"))
{
	this->App = App;
	Tabs.RegisterFactory(MakeShareable(new FCustomAssetPrimaryTabFactory(App)));
	Tabs.RegisterFactory(MakeShareable(new FCustomAssetPropertiesTabFactory(App)));

	TabLayout = FTabManager::NewLayout("CustomAssetAppMode_Layout_v1")
	->AddArea(FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split(
			FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split(
					FTabManager::NewStack()
								->SetSizeCoefficient(.75f)
								->AddTab(FName(TEXT("CustomAssetPrimaryTab")), ETabState::OpenedTab)
				)
				->Split(
					FTabManager::NewStack()
							->SetSizeCoefficient(.25f)
							->AddTab(FName(TEXT("CustomAssetPropertiesTab")), ETabState::OpenedTab)
				)
		)); 
}

void FCustomAssetAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FCustomAssetEditorApp> AppPtr = this->App.Pin();
	AppPtr->PushTabFactories(Tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FCustomAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FCustomAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
