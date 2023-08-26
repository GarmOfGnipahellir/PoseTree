// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FPoseTreeEditorToolkit;
class IPersonaPreviewScene;

class POSETREEEDITOR_API FPoseTreeEditorMode : public FApplicationMode
{
public:
	FPoseTreeEditorMode(
		TSharedRef<FWorkflowCentricApplication> InHostingApp,
		TSharedRef<IPersonaPreviewScene> InPreviewScene);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;

protected:
	TWeakPtr<FPoseTreeEditorToolkit> PoseTreeEditorToolkitPtr;
	FWorkflowAllowedTabSet TabFactories;
};
