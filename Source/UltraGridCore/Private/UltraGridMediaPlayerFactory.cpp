#include "UltraGridMediaPlayerFactory.h"
#include "UltraGridCoreModule.h"
#include "UltraGridMediaPlayer.h"

bool FUltraGridMediaPlayerFactory::CanPlayUrl(const FString& Url,
		const IMediaOptions *,
		TArray<FText> *,
		TArray<FText>* OutErrors) const
{
	FString Scheme;
	FString Location;

	if (!Url.Split(TEXT("://"), &Scheme, &Location, ESearchCase::CaseSensitive))
	{
		return false;
	}

	if(Scheme != FString("ultragridrecv")){
		return false;
	}

	return true;

}

TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> FUltraGridMediaPlayerFactory::CreatePlayer(IMediaEventSink& EventSink)
{
	return MakeShared<FUltraGridMediaPlayer, ESPMode::ThreadSafe>(EventSink);
}

FText FUltraGridMediaPlayerFactory::GetDisplayName() const{
	return FText::FromString(TEXT("UltraGrid Media Player"));
}

FName FUltraGridMediaPlayerFactory::GetPlayerName() const{
	return FName(TEXT("UltraGrid Player"));
}

FGuid FUltraGridMediaPlayerFactory::GetPlayerPluginGUID() const{
	return FUltraGridCoreModule::GetPlayerPluginGUID();
}

const TArray<FString>& FUltraGridMediaPlayerFactory::GetSupportedPlatforms() const{
	static TArray<FString> platforms = {
		"Windows",
		"Linux"
	};

	return platforms;
}

bool FUltraGridMediaPlayerFactory::SupportsFeature(EMediaFeature Feature) const{
	return (Feature == EMediaFeature::VideoSamples)
		|| (Feature == EMediaFeature::VideoTracks)
		|| (Feature == EMediaFeature::MetadataTracks);
}
