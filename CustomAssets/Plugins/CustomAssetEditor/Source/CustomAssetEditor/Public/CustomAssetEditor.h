// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

class FCustomAssetEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FSlateStyleSet> StyleSet = nullptr;
	TSharedPtr<struct FCustomPinFactory> CustomPinFactory = nullptr;
};

class SCustomGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SCustomGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPin)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InGraphPin);	
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FSlateColor(FLinearColor::Yellow);
	}
};

struct FCustomPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InGraphPin) const override
	{
		if (FName(TEXT("CustomPin")) == InGraphPin->PinType.PinSubCategory)
		{
			return SNew(SCustomGraphPin, InGraphPin);
		}
		return nullptr;
	}
};