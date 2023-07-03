// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PoseTreeFactory.generated.h"

UCLASS()
class POSETREEEDITOR_API UPoseTreeFactory : public UFactory
{
	GENERATED_BODY()

public:
	UPoseTreeFactory();
	
	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn) override;
};
