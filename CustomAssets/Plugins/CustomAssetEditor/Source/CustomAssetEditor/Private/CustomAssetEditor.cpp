// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetEditor.h"
#include "CustomAssetAction.h"
#include "IAssetTools.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FCustomAssetEditorModule"

void FCustomAssetEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("CustomAssets")), FText::FromString("Custom Assets"));
	TSharedPtr<FCustomAssetAction> CustomAssetAction = MakeShareable(new FCustomAssetAction(AssetType));
	AssetToolsModule.RegisterAssetTypeActions(CustomAssetAction.ToSharedRef());

	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("CustomAssetEditorStyle")));
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("CustomAssetEditor");
	FString ContentDir = Plugin->GetContentDir();
	StyleSet->SetContentRoot(ContentDir);

	FSlateImageBrush* ThumbnailBrush = new FSlateImageBrush(
		StyleSet->RootToContentDir(TEXT("CustomAssetThumbnail"),
		TEXT(".png")),
		FVector2D(128.f, 128.f));
	FSlateImageBrush* IconBrush = new FSlateImageBrush(
		StyleSet->RootToContentDir(TEXT("CustomAssetIcon"),
		TEXT(".png")),
		FVector2D(128.f, 128.f));
	FSlateImageBrush* NodeAddPinIcon = new FSlateImageBrush(
		StyleSet->RootToContentDir(TEXT("NodeAddPinIcon"),
		TEXT(".png")),
		FVector2D(128.f, 128.f));
	FSlateImageBrush* NodeDeletePinIcon = new FSlateImageBrush(
		StyleSet->RootToContentDir(TEXT("NodeDeletePinIcon"),
		TEXT(".png")),
		FVector2D(128.f, 128.f));
	FSlateImageBrush* NodeDeleteNodeIcon = new FSlateImageBrush(
		StyleSet->RootToContentDir(TEXT("NodeDeleteNodeIcon"),
		TEXT(".png")),
		FVector2D(128.f, 128.f));
	
	StyleSet->Set(TEXT("ClassThumbnail.CustomAsset"), ThumbnailBrush);
	StyleSet->Set(TEXT("ClassIcon.CustomAsset"), IconBrush);
	StyleSet->Set(TEXT("CustomAssetEditor.NodeAddPinIcon"), NodeAddPinIcon);
	StyleSet->Set(TEXT("CustomAssetEditor.NodeDeletePinIcon"), NodeDeletePinIcon);
	StyleSet->Set(TEXT("CustomAssetEditor.NodeDeleteNodeIcon"), NodeDeleteNodeIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);

	CustomPinFactory = MakeShareable(new FCustomPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory((CustomPinFactory));
}

void FCustomAssetEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory((CustomPinFactory));
	CustomPinFactory = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomAssetEditorModule, CustomAssetEditor)