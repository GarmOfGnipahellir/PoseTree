// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimGraphNode_PoseTree.h"

#define LOCTEXT_NAMESPACE "FPoseTreeEditor"

FText UAnimGraphNode_PoseTree::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("AnimGraphNode_PoseTree_Title", "Pose Tree");
}

FText UAnimGraphNode_PoseTree::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_PoseTree_Tooltip", "Evaluates a pose tree");
}

void UAnimGraphNode_PoseTree::CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const
{
	Super::CustomizePinData(Pin, SourcePropertyName, ArrayIndex);
}

void UAnimGraphNode_PoseTree::CreateCustomPins(TArray<UEdGraphPin*>* OldPins)
{
	Super::CreateCustomPins(OldPins);
}

#undef LOCTEXT_NAMESPACE
