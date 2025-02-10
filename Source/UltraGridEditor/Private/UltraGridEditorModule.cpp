#include "UltraGridEditorModule.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Styling/SlateStyleRegistry.h"
#include "HAL/PlatformProcess.h"

#define LOCTEXT_NAMESPACE "FUltraGridEditorModule"

void FUltraGridEditorModule::StartupModule()
{
	RegisterStyle();
}

void FUltraGridEditorModule::ShutdownModule()
{

}

void FUltraGridEditorModule::RegisterStyle() {
	const FVector2D IconSize(64.f, 64.f);
	const FVector2D ThumbnailSize(20.f, 20.f);

	Style = MakeUnique<FSlateStyleSet>("UltraGridMediaStyle");

	auto Plugin = IPluginManager::Get().FindPlugin(TEXT("UltraGridCore"));

	FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("UltraGrid"))->GetContentDir();

	Style->SetContentRoot(ContentDir);

	Style->Set("ClassIcon.UltraGridMediaSource", new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon64.png")), IconSize));
	Style->Set("ClassThumbnail.UltraGridMediaSource", new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon20.png")), ThumbnailSize));
	Style->Set("ClassIcon.UltraGridMediaOutput", new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon64.png")), IconSize));
	Style->Set("ClassThumbnail.UltraGridMediaOutput", new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon20.png")), ThumbnailSize));




	FSlateStyleRegistry::RegisterSlateStyle(*Style.Get());

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUltraGridEditorModule, UltraGridEditor)
