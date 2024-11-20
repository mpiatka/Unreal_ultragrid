#include "UltraGridMediaSource.h"

/*
UUltraGridMediaSource::UUltraGridMediaSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

*/

FString UUltraGridMediaSource::GetUrl() const{
	return FString("ultragridrecv://") + FString::FromInt(Port);
}
