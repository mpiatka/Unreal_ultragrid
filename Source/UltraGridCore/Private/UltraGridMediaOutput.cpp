#include "UltraGridMediaOutput.h"
#include "UltraGridCoreModule.h"
#include "UltraGridMediaCapture.h"

UUltraGridMediaOutput::UUltraGridMediaOutput(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	NumberOfTextureBuffers = 1;
}

	bool UUltraGridMediaOutput::Validate(FString& FailureReason) const{
		if(!Super::Validate(FailureReason)){
			return false;
		}

		return true;
	}

FIntPoint UUltraGridMediaOutput::GetRequestedSize() const 
{
	return OutputSize;
}

EPixelFormat UUltraGridMediaOutput::GetRequestedPixelFormat() const 
{
	return EPixelFormat::PF_B8G8R8A8;
}

EMediaCaptureConversionOperation UUltraGridMediaOutput::GetConversionOperation(EMediaCaptureSourceType InSourceType) const
{
	return EMediaCaptureConversionOperation::RGBA8_TO_YUV_8BIT;
	//return EMediaCaptureConversionOperation::NONE;
}

UMediaCapture* UUltraGridMediaOutput::CreateMediaCaptureImpl(){
	UUltraGridMediaCapture* Result = NewObject<UUltraGridMediaCapture>();
	if (Result)
	{
		UE_LOG(LogUltraGridMediaCore, Log, TEXT("Create UltraGridMediaCapture"));
		Result->SetMediaOutput(this);
		Result->Destination = Destination;
		Result->Compression = Compression;
		Result->VideoPort = VideoPort;
	}
	return Result;
}
