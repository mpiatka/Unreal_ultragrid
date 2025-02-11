#pragma once

#include "MediaOutput.h"
#include "MediaIOCoreDefinitions.h"

#include "UltraGridMediaOutput.generated.h"

UCLASS(BlueprintType)
	class ULTRAGRIDCORE_API UUltraGridMediaOutput : public UMediaOutput
{
	GENERATED_UCLASS_BODY()

public:
	bool Validate(FString& FailureReason) const;

	virtual FIntPoint GetRequestedSize() const override;
	virtual EPixelFormat GetRequestedPixelFormat() const override;
	virtual EMediaCaptureConversionOperation GetConversionOperation(EMediaCaptureSourceType InSourceType) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Video")
		FIntPoint OutputSize = FIntPoint(1280, 720);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Network")
		FString Destination = "localhost";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Network")
		bool FecEnabled = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
		FString FecConfig = "rs:200:220";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Codec")
		FString Compression = "libavcodec:codec=H.264:safe:bitrate=15M";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Network")
		int VideoPort = 5004;

protected:
	virtual UMediaCapture* CreateMediaCaptureImpl() override;
};
