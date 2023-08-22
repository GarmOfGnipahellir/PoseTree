// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNode_CustomProperty.h"
#include "AnimNode_PoseTree.generated.h"

class UPoseTree;
class UAnimGraphNode_PoseTree;

/**
 * 
 */
USTRUCT()
struct POSETREE_API FAnimNode_PoseTree : public FAnimNode_CustomProperty
{
	GENERATED_BODY()

	friend UAnimGraphNode_PoseTree;

	FAnimNode_PoseTree();

	virtual void OnInitializeAnimInstance(
		const FAnimInstanceProxy* InProxy,
		const UAnimInstance* InAnimInstance) override;
	virtual void InitializeProperties(const UObject* InSourceInstance, UClass* InTargetClass) override;

	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	
protected:
	UPROPERTY(EditAnywhere, Category="PoseTree")
	TObjectPtr<UPoseTree> PoseTree;

	virtual UClass* GetTargetClass() const override;
};
