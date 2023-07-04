// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeEditor.h"

#include "PoseTree/PoseTree.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "FPoseTreeEditor"

FPoseTreeEditor::FPoseTreeEditor() : PoseTree(nullptr) {}

void FPoseTreeEditor::InitPoseTreeEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UPoseTree* InPoseTree)
{
	PoseTree = InPoseTree;
	//CreateEdGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	//FPoseTreeEditorCommands::Register();

	//BindCommands();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout(
		"Standalone_PoseTreeEditor_Layout_v1"
	)->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->AddTab("PoseTreeEditorToolbarID", ETabState::OpenedTab)
			->SetHideTabWell(true)
		)
	);

	constexpr bool bCreateDefaultStandaloneMenu = true;
	constexpr bool bCreateDefaultToolbar = true;
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		"PoseTreeEditorApp",
		StandaloneDefaultLayout,
		bCreateDefaultStandaloneMenu,
		bCreateDefaultToolbar,
		PoseTree
	);
}

FName FPoseTreeEditor::GetToolkitFName() const
{
	return "FPoseTreeEditor";
}

FText FPoseTreeEditor::GetBaseToolkitName() const
{
	return LOCTEXT("PoseTreeEditorAppLabel", "Pose Tree Editor");
}

FString FPoseTreeEditor::GetWorldCentricTabPrefix() const
{
	return "PoseTreeEditor";
}

FLinearColor FPoseTreeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

#undef LOCTEXT_NAMESPACE
