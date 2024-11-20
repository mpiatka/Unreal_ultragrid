#pragma once

#include "IMediaPlayerFactory.h"

class FUltraGridMediaPlayerFactory : public IMediaPlayerFactory{

	virtual bool CanPlayUrl(const FString& Url, const IMediaOptions *, TArray<FText> *, TArray<FText>* OutErrors) const override;

	virtual TSharedPtr<IMediaPlayer, ESPMode::ThreadSafe> CreatePlayer(IMediaEventSink& EventSink) override;

	virtual FText GetDisplayName() const override;
	virtual FName GetPlayerName() const override;
	virtual FGuid GetPlayerPluginGUID() const override;
	virtual const TArray<FString>& GetSupportedPlatforms() const override;
	virtual bool SupportsFeature(EMediaFeature Feature) const override;

};
