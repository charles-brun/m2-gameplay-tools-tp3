#include "CustomAssetAction.h"
#include "CustomAsset.h"
#include "CustomAssetEditorApp.h"

FCustomAssetAction::FCustomAssetAction(EAssetTypeCategories::Type Category)
{
	AssetCategory = Category;
}

FText FCustomAssetAction::GetName() const
{
	return FText::FromString("Custom Asset");
}

FColor FCustomAssetAction::GetTypeColor() const
{
	return FColor::Purple;
}

UClass* FCustomAssetAction::GetSupportedClass() const
{
	return UCustomAsset::StaticClass();
}

void FCustomAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* Object : InObjects)
	{
		UCustomAsset* CustomAsset = Cast<UCustomAsset>(Object);
		if (CustomAsset != nullptr)
		{
			TSharedRef<FCustomAssetEditorApp> Editor(new FCustomAssetEditorApp());
			Editor->InitEditor(Mode, EditWithinLevelEditor, CustomAsset);
		}
	}
}

uint32 FCustomAssetAction::GetCategories()
{
	return AssetCategory;
}
