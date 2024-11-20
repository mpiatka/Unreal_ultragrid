#pragma once

#include "CoreMinimal.h"
#include "MediaIOCorePlayerBase.h"
#include "MediaIOCoreTextureSampleBase.h"
#include "UltraGridCoreModule.h"
#include "libultragrid/libultragrid.h"

class FUltraGridMediaTextureSamplePool : public TMediaObjectPool<FMediaIOCoreTextureSampleBase> {  };

class ULTRAGRIDCORE_API FUltraGridMediaPlayer : public FMediaIOCorePlayerBase{
public:
	FUltraGridMediaPlayer(IMediaEventSink& sink);
	~FUltraGridMediaPlayer();

	virtual FGuid GetPlayerPluginGUID() const override;

	virtual bool Open(const FString& Url, const IMediaOptions* Options) override;
	virtual void Close() override;

	virtual bool IsHardwareReady() const override;
	virtual void SetupSampleChannels() override;

	virtual void TickFetch(FTimespan DeltaTime, FTimespan Timecode) override;
	virtual void TickInput(FTimespan DeltaTime, FTimespan Timecode) override;

	void frame_arrived(struct video_frame *f);

	virtual TSharedPtr<FMediaIOCoreTextureSampleBase> AcquireTextureSample_AnyThread() const override;

#if WITH_EDITOR
	const FSlateBrush* GetDisplayIcon() const{
		return nullptr;
	}
#endif

private:
	TUniquePtr<FUltraGridMediaTextureSamplePool> TextureSamplePool;
	UltraGridHandleUniq UgHandle;
};
