// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "UltraGridMediaPlayerFactory.h"
#include "Templates/UniquePtr.h"
#include "libultragrid/libultragrid.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUltraGridMediaCore, Log, All);

struct UgHandleDeleter{ void operator()(libug_handle *h); };
using UltraGridHandleUniq = TUniquePtr<libug_handle, UgHandleDeleter>;

class UltraGridLib {
public:
	static bool Init();
	static void Shutdown();

	static bool IsInitialized();

private:
	static void* UltraGridLibHandle;
};

class FUltraGridCoreModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FGuid GetPlayerPluginGUID();

private:
	FUltraGridMediaPlayerFactory mediaPlayerFactory;
};
