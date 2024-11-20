#include "UltraGridMediaOutputFactory.h"
#include "UltraGridMediaOutput.h"
#include "AssetTypeCategories.h"

UUltraGridMediaOutputFactory::UUltraGridMediaOutputFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UUltraGridMediaOutput::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


UObject* UUltraGridMediaOutputFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UUltraGridMediaOutput>(InParent, InClass, InName, Flags);
}


uint32 UUltraGridMediaOutputFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Media;
}


bool UUltraGridMediaOutputFactory::ShouldShowInNewMenu() const
{
	return true;
}
