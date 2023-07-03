// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoseTreeEditorModule.h"

#include "AssetToolsModule.h"
#include "PoseTreeAssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FPoseTreeEditorModule"

void FPoseTreeEditorModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FPoseTreeEditorModule::OnPostEngineInit);
}

void FPoseTreeEditorModule::ShutdownModule()
{
	FCoreDelegates::OnPostEngineInit.RemoveAll(this);

	if (const FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools"))
	{
		for (TSharedRef<IAssetTypeActions> RegisteredAssetTypeAction : RegisteredAssetTypeActions)
		{
			AssetToolsModule->Get().UnregisterAssetTypeActions(RegisteredAssetTypeAction);
		}
	}
}

void FPoseTreeEditorModule::OnPostEngineInit()
{
	auto RegisterAssetTypeAction = [this](const TSharedRef<IAssetTypeActions>& InAssetTypeAction)
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		RegisteredAssetTypeActions.Add(InAssetTypeAction);
		AssetTools.RegisterAssetTypeActions(InAssetTypeAction);
	};
	RegisterAssetTypeAction(MakeShareable(new FPoseTreeAssetTypeActions));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPoseTreeEditorModule, PoseTreeEditor)
