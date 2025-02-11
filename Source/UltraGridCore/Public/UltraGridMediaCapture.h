#pragma once

#include "MediaCapture.h"
#include "Containers/CircularQueue.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Templates/UniquePtr.h"
#include "UltraGridCoreModule.h"
#include "libultragrid/types.h"
#include "UltraGridMediaCapture.generated.h"

struct UgFrameDeleter{ void operator()(struct video_frame *f); };
using UgFrameUniq = TUniquePtr<struct video_frame, UgFrameDeleter>;

class FUgSendThread;

UCLASS()
class ULTRAGRIDCORE_API UUltraGridMediaCapture : public UMediaCapture{
	GENERATED_BODY()

public:
		FString Destination;
		FString Compression;
		FString FecConfig;
		int VideoPort;
		friend FUgSendThread;

protected:
		virtual bool ValidateMediaOutput() const override;
		virtual void OnFrameCaptured_RenderingThread(const FCaptureBaseData& InBaseData,
				TSharedPtr<FMediaCaptureUserData, ESPMode::ThreadSafe> InUserData,
				void* InBuffer, int32 Width, int32 Height, int32 BytesPerRow) override;
		virtual void OnFrameCaptured_AnyThread(const FCaptureBaseData & InBaseData,  
				TSharedPtr<FMediaCaptureUserData, ESPMode::ThreadSafe> InUserData,  
				const FMediaCaptureResourceData& InResourceData) override;
		virtual bool InitializeCapture() override;
		virtual void StopCaptureImpl(bool bAllowPendingFrameToBeProcess) override;

		virtual bool SupportsAnyThreadCapture() const override { return true; }

private:
		void AddVideoFrame(int Width, int Height,
				int BytesPerRow,
				void *Buffer);

		int frames = 0;
		TUniquePtr<FUgSendThread> ugThread;
		FCriticalSection FrameQueueLock;
		TUniquePtr<TCircularQueue<UgFrameUniq>> FrameQueue;
		UltraGridHandleUniq ug_handle;

};

class FUgSendThread : public FRunnable{
public:
	FUgSendThread(UUltraGridMediaCapture *state);

	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	virtual void Exit() override {  }
	virtual void Stop() override { state->FrameQueue->Enqueue(nullptr); }

	TUniquePtr<FRunnableThread> Thread;
	UUltraGridMediaCapture *state;
};
