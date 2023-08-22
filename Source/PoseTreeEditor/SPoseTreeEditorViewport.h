// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAssetEditorViewport.h"
#include "Widgets/SCompoundWidget.h"

class FPoseTreeEditorToolkit;
class FAdvancedPreviewScene;

class POSETREEEDITOR_API SPoseTreeEditorViewport : public SAssetEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SPoseTreeEditorViewport) {}
		SLATE_ARGUMENT(TWeakPtr<FPoseTreeEditorToolkit>, PoseTreeEditorToolkit)
	SLATE_END_ARGS()

	SPoseTreeEditorViewport();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;
	TSharedPtr<FEditorViewportClient> ViewportClient; // TODO: create PoseTree viewport client
	TWeakPtr<FPoseTreeEditorToolkit> PoseTreeEditorToolkitPtr;

	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
};
