#pragma once

#include "Modules/ModuleInterface.h"

class IAssetTypeActions;

class FPoseTreeEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void OnPostEngineInit();

private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
};
