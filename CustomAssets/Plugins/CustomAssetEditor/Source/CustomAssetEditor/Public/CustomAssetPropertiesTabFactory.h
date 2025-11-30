#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FCustomAssetPropertiesTabFactory : public FWorkflowTabFactory
{
public:
	FCustomAssetPropertiesTabFactory(TSharedPtr<class FCustomAssetEditorApp> App);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<class FCustomAssetEditorApp> App;
};