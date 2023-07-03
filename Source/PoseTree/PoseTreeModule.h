#pragma once

#include "Modules/ModuleInterface.h"

class IAssetTypeActions;

class FPoseTreeModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
