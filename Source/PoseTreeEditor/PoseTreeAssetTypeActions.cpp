﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeAssetTypeActions.h"

#include "PoseTreeEditor.h"
#include "PoseTree/PoseTree.h"

#define LOCTEXT_NAMESPACE "FPoseTreeModule"

FText FPoseTreeAssetTypeActions::GetName() const
{
	return LOCTEXT("FPoseTreeAssetTypeActionsName", "Pose Tree");
}

FColor FPoseTreeAssetTypeActions::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FPoseTreeAssetTypeActions::GetSupportedClass() const
{
	return UPoseTree::StaticClass();
}

void FPoseTreeAssetTypeActions::OpenAssetEditor(
	const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		                                ? EToolkitMode::WorldCentric
		                                : EToolkitMode::Standalone;

	for (const auto Object : InObjects)
	{
		const auto PoseTree = Cast<UPoseTree>(Object);
		if (!PoseTree) continue;

		const TSharedRef<FPoseTreeEditor> NewPoseTreeEditor(new FPoseTreeEditor());
		NewPoseTreeEditor->InitPoseTreeEditor(Mode, EditWithinLevelEditor, PoseTree);
	}
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

uint32 FPoseTreeAssetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Animation;
}

// const TArray<FText>& FPoseTreeAssetTypeActions::GetSubMenus() const
// {
// 	static const TArray SubMenus
// 	{
// 		LOCTEXT("AnimPoseTreeSubMenu", "Pose Tree")
// 	};
// 	return SubMenus;
// }

#undef LOCTEXT_NAMESPACE
