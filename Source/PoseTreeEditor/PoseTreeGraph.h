// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PoseTreeGraph.generated.h"

UCLASS()
class POSETREEEDITOR_API UPoseTreeGraph : public UEdGraph
{
	GENERATED_BODY()

public:
	virtual bool Modify(bool bAlwaysMarkDirty) override;
};
