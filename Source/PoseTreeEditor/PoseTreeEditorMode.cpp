// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeEditorMode.h"

#include "PoseTreeEditorToolkit.h"
#include "Modules/ModuleManager.h"
#include "IPersonaToolkit.h"
#include "PersonaModule.h"
#include "PersonaTabs.h"

FPoseTreeEditorMode::FPoseTreeEditorMode(
	TSharedRef<FWorkflowCentricApplication> InHostingApp,
	TSharedRef<IPersonaPreviewScene> InPreviewScene) : FApplicationMode("PoseTreeEditorMode")
{
	PoseTreeEditorToolkitPtr = StaticCastSharedRef<FPoseTreeEditorToolkit>(InHostingApp);
	TSharedRef<FPoseTreeEditorToolkit> PoseTreeEditorToolkit =
		StaticCastSharedRef<FPoseTreeEditorToolkit>(InHostingApp);

	FPersonaViewportArgs ViewportArgs(InPreviewScene);
	ViewportArgs.bAlwaysShowTransformToolbar = true;
	ViewportArgs.bShowStats = false;
	ViewportArgs.bShowTurnTable = false;
	ViewportArgs.ContextName = TEXT("IKRigEditor.Viewport");
	ViewportArgs.OnViewportCreated = FOnViewportCreated::CreateSP(
		PoseTreeEditorToolkit,
		&FPoseTreeEditorToolkit::HandleViewportCreated
	);

	FPersonaModule& PersonaModule = FModuleManager::LoadModuleChecked<FPersonaModule>("Persona");
	TabFactories.RegisterFactory(PersonaModule.CreatePersonaViewportTabFactory(InHostingApp, ViewportArgs));
	TabFactories.RegisterFactory(
		PersonaModule.CreateDetailsTabFactory(
			InHostingApp,
			FOnDetailsCreated::CreateSP(&PoseTreeEditorToolkit.Get(), &FPoseTreeEditorToolkit::HandleDetailsCreated)
		)
	);
	TabFactories.RegisterFactory(
		PersonaModule.CreateAdvancedPreviewSceneTabFactory(
			InHostingApp,
			PoseTreeEditorToolkit->GetPersonaToolkit()->GetPreviewScene()
		)
	);

	TabLayout = FTabManager::NewLayout(
		"Standalone_PoseTreeEditor_Layout_v1"
	)->AddArea(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)->Split(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->Split(
				FTabManager::NewStack()->AddTab(FPersonaTabs::PreviewViewportID, ETabState::OpenedTab)
			)
		)
	);
	
	PersonaModule.OnRegisterTabs().Broadcast(TabFactories, InHostingApp);
	LayoutExtender = MakeShared<FLayoutExtender>();
	PersonaModule.OnRegisterLayoutExtensions().Broadcast(*LayoutExtender.Get());
	TabLayout->ProcessExtensions(*LayoutExtender.Get());
}

void FPoseTreeEditorMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FPoseTreeEditorToolkit> PoseTreeEditorToolkit = PoseTreeEditorToolkitPtr.Pin();
	PoseTreeEditorToolkit->RegisterTabSpawners(InTabManager.ToSharedRef());
	PoseTreeEditorToolkit->PushTabFactories(TabFactories);
	FApplicationMode::RegisterTabFactories(InTabManager);
}
