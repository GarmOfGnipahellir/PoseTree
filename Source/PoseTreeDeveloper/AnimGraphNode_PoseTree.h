// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_CustomProperty.h"
#include "PoseTree/AnimNode_PoseTree.h"
#include "AnimGraphNode_PoseTree.generated.h"

/**
 * 
 */
UCLASS()
class POSETREEDEVELOPER_API UAnimGraphNode_PoseTree : public UAnimGraphNode_CustomProperty
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Settings")
	FAnimNode_PoseTree Node;

private:
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	
	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const override;

	virtual FAnimNode_CustomProperty* GetCustomPropertyNode() override { return &Node; }
	virtual const FAnimNode_CustomProperty* GetCustomPropertyNode() const override { return &Node; }

	virtual void CreateCustomPins(TArray<UEdGraphPin*>* OldPins) override;
};
