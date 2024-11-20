#include "UltraGridMediaSourceFactory.h"
#include "UltraGridMediaSource.h"
#include "AssetTypeCategories.h"

UUltraGridMediaSourceFactory::UUltraGridMediaSourceFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UUltraGridMediaSource::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


UObject* UUltraGridMediaSourceFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UUltraGridMediaSource>(InParent, InClass, InName, Flags);
}


uint32 UUltraGridMediaSourceFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Media;
}


bool UUltraGridMediaSourceFactory::ShouldShowInNewMenu() const
{
	return true;
}
