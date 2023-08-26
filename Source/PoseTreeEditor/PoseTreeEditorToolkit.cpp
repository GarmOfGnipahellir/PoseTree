// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeEditorToolkit.h"

#include "AdvancedPreviewSceneModule.h"
#include "EditorViewportTabContent.h"
#include "PoseTree/PoseTree.h"
#include "GraphEditorActions.h"
#include "IPersonaToolkit.h"
#include "IPersonaViewport.h"
#include "PersonaModule.h"
#include "PoseTreeEditorMode.h"
#include "PoseTreeGraph.h"
#include "PoseTreeSchema.h"
#include "SPoseTreeEditorViewport.h"
#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "FPoseTreeEditor"

class UIKRigDefinition;
const FName FPoseTreeEditorToolkit::ViewportTabId("Viewport");
const FName FPoseTreeEditorToolkit::GraphCanvasTabId("GraphCanvas");
const FName FPoseTreeEditorToolkit::AssetDetailsTabId("AssetDetails");
const FName FPoseTreeEditorToolkit::PreviewSceneSettingsTabId("PreviewSceneSettings");

FPoseTreeEditorToolkit::FPoseTreeEditorToolkit() : PoseTree(nullptr) {}

void FPoseTreeEditorToolkit::InitPoseTreeEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UPoseTree* InPoseTree)
{
	PoseTree = InPoseTree;
	if (!PoseTree->EdGraph)
	{
		PoseTree->EdGraph = CastChecked<UEdGraph>(
			FBlueprintEditorUtils::CreateNewGraph(
				PoseTree,
				NAME_None,
				UPoseTreeGraph::StaticClass(),
				UPoseTreeSchema::StaticClass()
			)
		);
	}

	FPersonaModule& PersonaModule = FModuleManager::GetModuleChecked<FPersonaModule>("Persona");

	FPersonaToolkitArgs PersonaToolkitArgs;
	PersonaToolkitArgs.OnPreviewSceneCreated = FOnPreviewSceneCreated::FDelegate::CreateSP(
		this,
		&FPoseTreeEditorToolkit::HandlePreviewSceneCreated
	);
	PersonaToolkitArgs.bPreviewMeshCanUseDifferentSkeleton = true;
	PersonaToolkit = PersonaModule.CreatePersonaToolkit(PoseTree, PersonaToolkitArgs);

	FPersonaViewportArgs ViewportArgs(PersonaToolkit->GetPreviewScene());
	ViewportArgs.bAlwaysShowTransformToolbar = true;
	ViewportArgs.bShowStats = false;
	ViewportArgs.bShowTurnTable = false;
	ViewportArgs.ContextName = TEXT("IKRigEditor.Viewport");
	ViewportArgs.OnViewportCreated = FOnViewportCreated::CreateSP(
		this,
		&FPoseTreeEditorToolkit::HandleViewportCreated
	);
	TabFactories.RegisterFactory(PersonaModule.CreatePersonaViewportTabFactory(SharedThis(this), ViewportArgs));

	GraphEditor = CreateGraphEditorWidget();
	AssetDetailsEditor = CreateAssetDetailsEditorWidget();

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout(
		"Standalone_PoseTreeEditor_Layout_v1"
	)->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->Split(
				FTabManager::NewStack()->AddTab(GraphCanvasTabId, ETabState::OpenedTab)
			)
		)
	);

	constexpr bool bCreateDefaultStandaloneMenu = true;
	constexpr bool bCreateDefaultToolbar = true;
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		"PoseTreeEditorApp",
		FTabManager::FLayout::NullLayout,
		bCreateDefaultStandaloneMenu,
		bCreateDefaultToolbar,
		PoseTree
	);

	TSharedRef<FPoseTreeEditorMode> PoseTreeEditorMode = MakeShareable(
		new FPoseTreeEditorMode(SharedThis(this), PersonaToolkit->GetPreviewScene())
	);
	AddApplicationMode("PoseTreeEditorMode", PoseTreeEditorMode);
	
	SetCurrentMode("PoseTreeEditorMode");
}

void FPoseTreeEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	TSharedRef<FWorkspaceItem> WorkspaceMenuCategoryRef = InTabManager->AddLocalWorkspaceMenuCategory(
		LOCTEXT("WorkspaceMenu_PoseTreeEditor", "Pose Tree Editor")
	);

	InTabManager->RegisterTabSpawner(
			ViewportTabId,
			FOnSpawnTab::CreateSP(this, &FPoseTreeEditorToolkit::SpawnTab_Viewport)
		).SetDisplayName(LOCTEXT("ViewportTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewport"));
	InTabManager->RegisterTabSpawner(
			GraphCanvasTabId,
			FOnSpawnTab::CreateSP(this, &FPoseTreeEditorToolkit::SpawnTab_GraphCanvas)
		).SetDisplayName(LOCTEXT("GraphCanvasTab", "Graph"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.EventGraph_16x"));
	InTabManager->RegisterTabSpawner(
			AssetDetailsTabId,
			FOnSpawnTab::CreateSP(this, &FPoseTreeEditorToolkit::SpawnTab_AssetDetails)
		).SetDisplayName(LOCTEXT("AssetDetailsTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
	InTabManager->RegisterTabSpawner(
			PreviewSceneSettingsTabId,
			FOnSpawnTab::CreateSP(this, &FPoseTreeEditorToolkit::SpawnTab_PreviewSceneSettings)
		).SetDisplayName(LOCTEXT("PreviewSceneSettingsTab", "Preview Scene Settings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FPoseTreeEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(ViewportTabId);
	InTabManager->UnregisterTabSpawner(GraphCanvasTabId);
	InTabManager->UnregisterTabSpawner(AssetDetailsTabId);
	InTabManager->UnregisterTabSpawner(PreviewSceneSettingsTabId);
}

FName FPoseTreeEditorToolkit::GetToolkitFName() const
{
	return "FPoseTreeEditor";
}

FText FPoseTreeEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("PoseTreeEditorAppLabel", "Pose Tree Editor");
}

FString FPoseTreeEditorToolkit::GetWorldCentricTabPrefix() const
{
	return "PoseTreeEditor";
}

FLinearColor FPoseTreeEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

void FPoseTreeEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(PoseTree);
	Collector.AddReferencedObject(PoseTree->EdGraph);
}

void FPoseTreeEditorToolkit::HandleDetailsCreated(const TSharedRef<IDetailsView>& InDetailsView) const {}

void FPoseTreeEditorToolkit::HandleViewportCreated(const TSharedRef<IPersonaViewport>& InViewport) {}

TSharedRef<SDockTab> FPoseTreeEditorToolkit::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> DockableTab = SNew(SDockTab);
	ViewportEditor = MakeShareable(new FEditorViewportTabContent);

	TWeakPtr<FPoseTreeEditorToolkit> WeakSharedThis = SharedThis(this);
	const FString LayoutId = FString("PoseTreeEditorViewport");
	ViewportEditor->Initialize(
		[this, WeakSharedThis](const FAssetEditorViewportConstructionArgs& InConstructionArgs)
		{
			return SAssignNew(ViewportEditorWidget, SPoseTreeEditorViewport).PoseTreeEditorToolkit(WeakSharedThis);
		},
		DockableTab,
		LayoutId
	);

	return DockableTab;
}

TSharedRef<SDockTab> FPoseTreeEditorToolkit::SpawnTab_GraphCanvas(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab).Label(LOCTEXT("Graph_TabTitle", "Graph"))[GraphEditor.ToSharedRef()];
}

TSharedRef<SDockTab> FPoseTreeEditorToolkit::SpawnTab_AssetDetails(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == AssetDetailsTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("AssetDetails_TabTitle", "Details"))
		[
			AssetDetailsEditor->AsShared()
		];
}

TSharedRef<SDockTab> FPoseTreeEditorToolkit::SpawnTab_PreviewSceneSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == PreviewSceneSettingsTabId);

	TSharedRef<SWidget> InWidget = SNullWidget::NullWidget;
	if (ViewportEditorWidget.IsValid())
	{
		FAdvancedPreviewSceneModule& AdvancedPreviewSceneModule = FModuleManager::LoadModuleChecked<
			FAdvancedPreviewSceneModule>("AdvancedPreviewScene");
		InWidget = AdvancedPreviewSceneModule.CreateAdvancedPreviewSceneSettingsWidget(
			ViewportEditorWidget->GetPreviewScene()
		);
	}

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("PreviewSceneSettingsTab", "Preview Scene Settings"))
		[
			SNew(SBox)
			[
				InWidget
			]
		];

	return SpawnedTab;
}

void FPoseTreeEditorToolkit::DeleteNodes() {}

bool FPoseTreeEditorToolkit::CanDeleteNodes()
{
	return true;
}

void FPoseTreeEditorToolkit::OnSelectedNodesChanged(const TSet<UObject*>& NewSelection) {}
void FPoseTreeEditorToolkit::OnNodeDoubleClicked(UEdGraphNode* Node) {}

TSharedRef<SGraphEditor> FPoseTreeEditorToolkit::CreateGraphEditorWidget()
{
	//FGenericCommands::Register();
	//FGraphEditorCommands::Register();
	//FPoseTreeEditorCommands::Register();
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList);
		// TODO: map actions
		GraphEditorCommands->MapAction(
			FGenericCommands::Get().Delete,
			FExecuteAction::CreateRaw(this, &FPoseTreeEditorToolkit::DeleteNodes),
			FCanExecuteAction::CreateRaw(this, &FPoseTreeEditorToolkit::CanDeleteNodes)
		);
	}

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("Graph_AppearanceCornerText", "Pose Tree");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(
		this,
		&FPoseTreeEditorToolkit::OnSelectedNodesChanged
	);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FPoseTreeEditorToolkit::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.Appearance(AppearanceInfo)
		.GraphToEdit(PoseTree->EdGraph)
		.GraphEvents(InEvents);
}

TSharedPtr<IDetailsView> FPoseTreeEditorToolkit::CreateAssetDetailsEditorWidget()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(
		TEXT("PropertyEditor")
	);

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
		DetailsViewArgs.NotifyHook = this;
	}

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(PoseTree);
	return DetailsView;
}

void FPoseTreeEditorToolkit::HandlePreviewSceneCreated(
	const TSharedRef<IPersonaPreviewScene, ESPMode::ThreadSafe>& Shared) {}

#undef LOCTEXT_NAMESPACE
