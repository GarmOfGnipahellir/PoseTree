// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeSchema.h"

#define LOCTEXT_NAMESPACE "FPoseTreeEditor"

UEdGraphNode* FAssetSchemaAction_PoseTree_CreateNode::PerformAction(
	UEdGraph* ParentGraph,
	UEdGraphPin* FromPin,
	const FVector2D Location,
	bool bSelectNewNode)
{
	if (!ensureMsgf(NodeTemplate, TEXT("NodeTemplate was NULL"))) return nullptr;

	const FScopedTransaction Transaction(
		LOCTEXT("PoseTreeEditorCreateNode_Transaction", "Pose Tree Editor: Create Node")
	);

	ParentGraph->Modify();
	if (FromPin)
	{
		FromPin->Modify();
	}

	NodeTemplate->Rename(nullptr, ParentGraph);
	ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

	NodeTemplate->CreateNewGuid();
	NodeTemplate->PostPlacedNewNode();
	NodeTemplate->AllocateDefaultPins();
	NodeTemplate->AutowireNewNode(FromPin);

	NodeTemplate->NodePosX = Location.X;
	NodeTemplate->NodePosY = Location.Y;

	NodeTemplate->SetFlags(RF_Transactional);

	return NodeTemplate;
}

void FAssetSchemaAction_PoseTree_CreateNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

void UPoseTreeSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

void UPoseTreeSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FAssetSchemaAction_PoseTree_CreateNode> Action(
		new FAssetSchemaAction_PoseTree_CreateNode(
			LOCTEXT("PoseTreeCreateNode", "Pose Tree"),
			FText::FromString("Test Desc"),
			FText::FromString("Test Tooltip"),
			0
		)
	);
	Action->NodeTemplate = NewObject<UPoseTreeEdNode>(ContextMenuBuilder.OwnerOfTemporaries);
	ContextMenuBuilder.AddAction(Action);
}

#undef LOCTEXT_NAMESPACE
