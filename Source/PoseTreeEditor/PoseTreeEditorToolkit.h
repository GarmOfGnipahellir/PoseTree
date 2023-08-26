// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"
#include "IHasPersonaToolkit.h"
#include "PersonaAssetEditorToolkit.h"

class IPersonaPreviewScene;
class SPoseTreeEditorViewport;
class FEditorViewportTabContent;
class IPersonaToolkit;
class UPoseTree;
class IPersonaViewport;

// reference https://github.com/jinyuliao/GenericGraph

class FPoseTreeEditorToolkit :
	public FPersonaAssetEditorToolkit,
	public IHasPersonaToolkit,
	public FNotifyHook,
	public FGCObject
{
public:
	friend SPoseTreeEditorViewport;

	FPoseTreeEditorToolkit();

	virtual TSharedRef<IPersonaToolkit> GetPersonaToolkit() const override { return PersonaToolkit.ToSharedRef(); }

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

	void HandleDetailsCreated(const TSharedRef<IDetailsView>& InDetailsView) const;
	void HandleViewportCreated(const TSharedRef<IPersonaViewport>& InViewport);

protected:
	TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_GraphCanvas(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_AssetDetails(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_PreviewSceneSettings(const FSpawnTabArgs& Args);

	void DeleteNodes();
	bool CanDeleteNodes();

	void OnSelectedNodesChanged(const TSet<UObject*>& NewSelection);
	void OnNodeDoubleClicked(UEdGraphNode* Node);

private:
	FWorkflowAllowedTabSet TabFactories;
	TSharedPtr<IPersonaToolkit> PersonaToolkit;
	TObjectPtr<UPoseTree> PoseTree;

	static const FName ViewportTabId;
	TSharedPtr<FEditorViewportTabContent> ViewportEditor;
	TSharedPtr<SPoseTreeEditorViewport> ViewportEditorWidget;

	static const FName GraphCanvasTabId;
	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<FUICommandList> GraphEditorCommands;
	TSharedRef<SGraphEditor> CreateGraphEditorWidget();

	static const FName AssetDetailsTabId;
	TSharedPtr<IDetailsView> AssetDetailsEditor;
	TSharedPtr<IDetailsView> CreateAssetDetailsEditorWidget();

	static const FName PreviewSceneSettingsTabId;
	void HandlePreviewSceneCreated(const TSharedRef<IPersonaPreviewScene, ESPMode::ThreadSafe>& Shared);
};
