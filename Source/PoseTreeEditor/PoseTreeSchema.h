// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoseTreeEdNode.h"
#include "PoseTreeSchema.generated.h"

USTRUCT()
struct FAssetSchemaAction_PoseTree_CreateNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()

	FAssetSchemaAction_PoseTree_CreateNode() : NodeTemplate(nullptr) {}

	FAssetSchemaAction_PoseTree_CreateNode(
		const FText& InNodeCategory,
		const FText& InMenuDesc,
		const FText& InToolTip,
		const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2D Location,
		bool bSelectNewNode) override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	TObjectPtr<UPoseTreeEdNode> NodeTemplate;
};

UCLASS(MinimalAPI)
class UPoseTreeSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
};
