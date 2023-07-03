// Fill out your copyright notice in the Description page of Project Settings.


#include "PoseTreeFactory.h"

#include "Animation/PoseAsset.h"
#include "PoseTree/PoseTree.h"

UPoseTreeFactory::UPoseTreeFactory()
{
	SupportedClass = UPoseTree::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UPoseTreeFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn)
{
	UPoseAsset* NewPoseAsset = NewObject<UPoseAsset>(InParent, InClass, InName, Flags | RF_Transactional);
	return NewPoseAsset;
}
