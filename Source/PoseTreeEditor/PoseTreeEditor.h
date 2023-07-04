// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"

class IPersonaToolkit;
class UPoseTree;

// reference https://github.com/jinyuliao/GenericGraph

class FPoseTreeEditor : public FAssetEditorToolkit
{
public:
	FPoseTreeEditor();

	void InitPoseTreeEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UPoseTree* InPoseTree);

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:
	TSharedPtr<IPersonaToolkit> PersonaToolkit;
	TObjectPtr<UPoseTree> PoseTree;
};
