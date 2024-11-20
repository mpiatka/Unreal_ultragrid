#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "UltraGridMediaSourceFactory.generated.h"

UCLASS()
class ULTRAGRIDEDITOR_API UUltraGridMediaSourceFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual uint32 GetMenuCategories() const override;
	virtual bool ShouldShowInNewMenu() const override;
};
