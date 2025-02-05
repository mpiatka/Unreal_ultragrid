// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraGridCoreModule.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "IMediaModule.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Logging/StructuredLog.h"
#if PLATFORM_WINDOWS
#include "Windows.h"
#endif

#define LOCTEXT_NAMESPACE "FUltraGridCoreModule"

DEFINE_LOG_CATEGORY(LogUltraGridMediaCore);

void UgHandleDeleter::operator()(libug_handle *h){ if(h) libug_destroy_handle(h); }

void* UltraGridLib::UltraGridLibHandle = nullptr;

static void LogCallback(const char* msg) {

	UE_LOGFMT(LogUltraGridMediaCore, Log, "{0}", msg);
}

bool UltraGridLib::Init() {
	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("UltraGrid")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/UltraGridLibrary/Win64/libultragrid.dll"));

	FString DllSearchPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/UltraGridLibrary/Win64/Bundled")));
	auto DllSearchPathW = StringCast<WIDECHAR>(*DllSearchPath);
	UE_LOGFMT(LogUltraGridMediaCore, Log, "Setting Dll search path: {0}", DllSearchPathW);
	if (SetDllDirectoryW(DllSearchPathW.Get())) {
		UE_LOGFMT(LogUltraGridMediaCore, Log, "Search path set successfully");
	}
#elif PLATFORM_MAC
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/UltraGridLibrary/Mac/Release/libExampleLibrary.dylib"));
#elif PLATFORM_LINUX
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/LibUltraGrid/libultragrid.so"));
#endif // PLATFORM_WINDOWS


	UE_LOGFMT(LogUltraGridMediaCore, Log, "Load ug");
	UltraGridLibHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (UltraGridLibHandle) {
		UE_LOGFMT(LogUltraGridMediaCore, Log, "Loaded libultragrid");
		UE_LOGFMT(LogUltraGridMediaCore, Log, "Header version: {0} Dll version: {1}", LIBULTRAGRID_HEADER_VERSION, libug_get_version());
	}

	libug_set_log_callback(LogCallback);

	return UltraGridLibHandle != nullptr;
}

void UltraGridLib::Shutdown() {
	if (UltraGridLibHandle != nullptr) {
		UE_LOGFMT(LogUltraGridMediaCore, Log, "Freeing UltraGrid DLL");
		FPlatformProcess::FreeDllHandle(UltraGridLibHandle);
		UltraGridLibHandle = nullptr;
	}
}

bool UltraGridLib::IsInitialized() {
	return UltraGridLibHandle != nullptr;
}

void FUltraGridCoreModule::StartupModule()
{
	UE_LOGFMT(LogUltraGridMediaCore, Log, "UltraGrid Core StartupModule");
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (!UltraGridLib::Init()) {
		UE_LOGFMT(LogUltraGridMediaCore, Warning, "Failed to load UltraGrid library");
		return;
	}


	auto MediaModule = FModuleManager::LoadModulePtr<IMediaModule>("Media");
	if (MediaModule != nullptr)
	{
		MediaModule->RegisterPlayerFactory(mediaPlayerFactory);
	}

}

void FUltraGridCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	//FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	//ExampleLibraryHandle = nullptr;

	UltraGridLib::Shutdown();
}

FGuid FUltraGridCoreModule::GetPlayerPluginGUID(){
	static FGuid UltraGridMediaPlayerGuid(
			0x6b69415d,
			0x02ac35ee,
			0x728c7a30,
			0x7aff6549);

	return UltraGridMediaPlayerGuid;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUltraGridCoreModule, UltraGridCore)
