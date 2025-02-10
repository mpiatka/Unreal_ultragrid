#pragma once

#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"
#include "Templates/UniquePtr.h"

class FUltraGridEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterStyle();

	TUniquePtr<FSlateStyleSet> Style;
};
