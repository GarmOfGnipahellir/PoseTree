// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PoseTree.generated.h"

class UControlRig;
class UPoseTreeNode;

UCLASS()
class POSETREE_API UPoseTree : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="PoseTree")
	TSubclassOf<UControlRig> ControlRigClass;
	
	UPROPERTY(Transient)
	TObjectPtr<UControlRig> ControlRig;
	
	UPROPERTY(VisibleAnywhere, Category="PoseTree")
	TArray<TObjectPtr<UPoseTreeNode>> Nodes;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UEdGraph> EdGraph;
#endif
};
