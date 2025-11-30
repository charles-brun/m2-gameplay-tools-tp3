#pragma once

#include "CoreMinimal.h" 	
#include "UObject/NameTypes.h"
#include "CustomRuntimeGraph.generated.h"

UCLASS()
class CUSTOMASSETRUNTIME_API UCustomRuntimePin : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FGuid PinId;

	UPROPERTY()
	UCustomRuntimePin* Connection = nullptr;
};

UCLASS()
class CUSTOMASSETRUNTIME_API UCustomRuntimeNode : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY()
	UCustomRuntimePin* InputPin;

	UPROPERTY()
	TArray<UCustomRuntimePin*> OutputPins;

	UPROPERTY()
	FVector2D Position;
};

UCLASS()
class CUSTOMASSETRUNTIME_API UCustomRuntimeGraph : public UObject {
	GENERATED_BODY()

public:
	UPROPERTY();
	TArray<UCustomRuntimeNode*> Nodes;
};