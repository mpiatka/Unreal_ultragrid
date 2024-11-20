#include "UltraGridEditorModule.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FUltraGridEditorModule"

void FUltraGridEditorModule::StartupModule()
{

}

void FUltraGridEditorModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUltraGridEditorModule, UltraGridEditor)
