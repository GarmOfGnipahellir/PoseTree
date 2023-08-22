// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeFactory.h"

#include "PoseTreeSchema.h"
#include "Animation/PoseAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "PoseTree/PoseTree.h"

UPoseTreeFactory::UPoseTreeFactory()
{
	SupportedClass = UPoseTree::StaticClass();
	bCreateNew = true;
}

UObject* UPoseTreeFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	UPoseTree* NewPoseTree = NewObject<UPoseTree>(InParent, InClass, InName, Flags);
	return NewPoseTree;
}
