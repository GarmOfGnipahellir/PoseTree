// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"

class FEditorViewportTabContent;
class IPersonaToolkit;
class UPoseTree;

// reference https://github.com/jinyuliao/GenericGraph

class FPoseTreeEditorToolkit : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:
	FPoseTreeEditorToolkit();

	void InitPoseTreeEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UPoseTree* InPoseTree);

	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

protected:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_AssetDetails(const FSpawnTabArgs& Args);

	void DeleteNodes();
	bool CanDeleteNodes();
	
	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);

private:
	TSharedPtr<IPersonaToolkit> PersonaToolkit;
	TObjectPtr<UPoseTree> PoseTree;
	
	static const FName ViewportTabId;
	TSharedPtr<FEditorViewportTabContent> ViewportEditor;
	
	static const FName GraphCanvasTabId;
	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TSharedRef<SGraphEditor> CreateGraphEditorWidget();
	
	static const FName AssetDetailsTabId;
	TSharedPtr<IDetailsView> AssetDetailsEditor;
	TSharedPtr<IDetailsView> CreateAssetDetailsEditorWidget();
};
