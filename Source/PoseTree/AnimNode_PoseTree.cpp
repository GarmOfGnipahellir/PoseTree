// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNode_PoseTree.h"

#include "PoseTree.h"

FAnimNode_PoseTree::FAnimNode_PoseTree(): FAnimNode_CustomProperty(), PoseTree(nullptr) {}

void FAnimNode_PoseTree::OnInitializeAnimInstance(
	const FAnimInstanceProxy* InProxy,
	const UAnimInstance* InAnimInstance)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()

	FAnimNode_CustomProperty::OnInitializeAnimInstance(InProxy, InAnimInstance);

	InitializeProperties(InAnimInstance, GetTargetClass());
}

void FAnimNode_PoseTree::InitializeProperties(const UObject* InSourceInstance, UClass* InTargetClass)
{
	SourceProperties.Reset(SourcePropertyNames.Num());
	DestProperties.Reset(DestPropertyNames.Num());

	check(SourcePropertyNames.Num() == DestPropertyNames.Num());

	for (int32 Idx = 0; Idx < SourcePropertyNames.Num(); ++Idx)
	{
		FName& SourceName = SourcePropertyNames[Idx];
		UClass* SourceClass = InSourceInstance->GetClass();

		FProperty* SourceProperty = FindFProperty<FProperty>(SourceClass, SourceName);
		SourceProperties.Add(SourceProperty);
		DestProperties.Add(nullptr);
	}
}

void FAnimNode_PoseTree::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	FAnimNode_CustomProperty::Initialize_AnyThread(Context);
}

void FAnimNode_PoseTree::Update_AnyThread(const FAnimationUpdateContext& Context)
{
	FAnimNode_CustomProperty::Update_AnyThread(Context);
}

void FAnimNode_PoseTree::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	FAnimNode_CustomProperty::CacheBones_AnyThread(Context);
}

void FAnimNode_PoseTree::Evaluate_AnyThread(FPoseContext& Output)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()
}

UClass* FAnimNode_PoseTree::GetTargetClass() const
{
	return UPoseTree::StaticClass();
}
