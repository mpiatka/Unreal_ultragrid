#pragma once

#include "BaseMediaSource.h"
#include "UltraGridMediaSource.generated.h"

UCLASS()
class ULTRAGRIDCORE_API UUltraGridMediaSource : public UBaseMediaSource{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Network")
	int Port = 5004;

	virtual bool Validate() const override { return true; }
	virtual FString GetUrl() const override;
};
