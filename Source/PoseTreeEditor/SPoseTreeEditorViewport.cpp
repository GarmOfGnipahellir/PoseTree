// Fill out your copyright notice in the Description page of Project Settings.


#include "SPoseTreeEditorViewport.h"

#include "AdvancedPreviewScene.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SPoseTreeEditorViewport::SPoseTreeEditorViewport()
{
	PreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
	PreviewScene->SetFloorVisibility(false);
}

void SPoseTreeEditorViewport::Construct(const FArguments& InArgs)
{
	PoseTreeEditorToolkitPtr = InArgs._PoseTreeEditorToolkit;
	TSharedPtr<FPoseTreeEditorToolkit> PoseTreeEditorToolkit = PoseTreeEditorToolkitPtr.Pin();
	check(PoseTreeEditorToolkitPtr.IsValid());

	SEditorViewport::Construct(SEditorViewport::FArguments());

	FBoxSphereBounds SphereBounds = FBoxSphereBounds(EForceInit::ForceInitToZero);
	// CustomDataflowActor = CastChecked<ADataflowActor>(PreviewScene->GetWorld()->SpawnActor(ADataflowActor::StaticClass()));

	// ViewportClient->SetDataflowActor(CustomDataflowActor);
	ViewportClient->FocusViewportOnBox(SphereBounds.GetBox());
}

TSharedRef<FEditorViewportClient> SPoseTreeEditorViewport::MakeEditorViewportClient()
{
	ViewportClient = MakeShareable(new FEditorViewportClient(nullptr, PreviewScene.Get(), SharedThis(this)));
	return ViewportClient.ToSharedRef();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
