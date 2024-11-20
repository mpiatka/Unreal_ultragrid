#include "UltraGridMediaCapture.h"
#include "UltraGridCoreModule.h"

#include "Misc/ScopeLock.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "Logging/StructuredLog.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "libultragrid/video_codec.h"
#include "libultragrid/video_frame.h"

void UgFrameDeleter::operator()(struct video_frame *f){ vf_free(f); }

FUgSendThread::FUgSendThread(UUltraGridMediaCapture *state): state(state) {
	Thread.Reset(FRunnableThread::Create(this, TEXT("UgSendThread")));
};

uint32 FUgSendThread::Run(){
	bool run = true;
	while(run){
		UgFrameUniq f;
		while(!state->FrameQueue->Dequeue(f)){
			FGenericPlatformProcess::ConditionalSleep([&]() -> bool { return !state->FrameQueue->IsEmpty(); }, 0.0f);
		}

		if(!f){
			run = false;
			break;
		}
		libug_send_video_frame(state->ug_handle.Get(), f.Release());
	}

	UE_LOGFMT(LogUltraGridMediaCore, Log, "UltraGrid send thread exit");

	return 0;
}

void UUltraGridMediaCapture::AddVideoFrame(int Width, int Height,
		int BytesPerRow,
		void *Buffer)
{
	video_desc desc = {};
	desc.width = Width * 2; //UYVY
	desc.height = Height;
	desc.color_spec = UYVY; 
	desc.tile_count = 1; 
	desc.fps = 30; //TODO

	auto ug_frame = UgFrameUniq(vf_alloc_desc_data(desc));

	char *dst = ug_frame->tiles[0].data;
	char *src = static_cast<char *>(Buffer);
	int dst_pitch = vc_get_linesize(desc.width, UYVY);

	for(int i = 0; i < Height; i++){
		memcpy(dst + i * dst_pitch, src + i * BytesPerRow, BytesPerRow);
	}

	FScopeLock ScopeLock(&FrameQueueLock);
	FrameQueue->Enqueue(MoveTemp(ug_frame));
}

void UUltraGridMediaCapture::OnFrameCaptured_RenderingThread(
		const FCaptureBaseData& InBaseData,
		TSharedPtr<FMediaCaptureUserData, ESPMode::ThreadSafe> InUserData,
		void* InBuffer,
		int32 Width, int32 Height, int32 BytesPerRow)
{
	frames++;
	//UE_LOGFMT(LogUltraGridMediaCore, Log, "Captured {0}: {1}x{2}", frames, Width, Height);

	AddVideoFrame(Width, Height, BytesPerRow, InBuffer);

	if(frames >= 100 && GEngine){
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("100 frames"));	
		frames = 0;
	}
}

void UUltraGridMediaCapture::OnFrameCaptured_AnyThread(
		const FCaptureBaseData & InBaseData,  
		TSharedPtr<FMediaCaptureUserData, ESPMode::ThreadSafe> InUserData,  
		const FMediaCaptureResourceData& InResourceData)
{

	frames++;
	AddVideoFrame(InResourceData.Width, InResourceData.Height,
			InResourceData.BytesPerRow,
			InResourceData.Buffer);

	if(frames >= 100 && GEngine){
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("100 frames"));	
		frames = 0;
	}
}

bool UUltraGridMediaCapture::ValidateMediaOutput() const{
	UE_LOG(LogUltraGridMediaCore, Log, TEXT("Validate UltraGridMediaCapture"));
	return true;
}

bool UUltraGridMediaCapture::InitializeCapture(){
	UE_LOG(LogUltraGridMediaCore, Log, TEXT("Initialize UltraGridMediaCapture"));
	if (!UltraGridLib::IsInitialized()) {
		UE_LOG(LogUltraGridMediaCore, Log, TEXT("The Ug library is not loaded"));
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("UltraGrid library not loaded, capture will not work"));	
		}
		return false;
	}
	SetState(EMediaCaptureState::Capturing);

	libug_conf ugConf{};
	libug_init_conf(&ugConf);
	ugConf.mode = LIBUG_MODE_SEND;
	auto destinationAnsi = StringCast<ANSICHAR>(*Destination);
	ugConf.send_destination = destinationAnsi.Get();
	ugConf.tx_port = VideoPort;
	auto compressionAnsi = StringCast<ANSICHAR>(*Compression);
	ugConf.compress = compressionAnsi.Get();


	ug_handle.Reset(libug_create_handle(&ugConf));
	FrameQueue = MakeUnique<TCircularQueue<UgFrameUniq>>(4);
	ugThread = MakeUnique<FUgSendThread>(this);

	return true;
}

void UUltraGridMediaCapture::StopCaptureImpl(bool bAllowPendingFrameToBeProcess)
{
	if(ugThread){
		ugThread->Stop();
		ugThread.Reset();
	}
	ug_handle.Reset();
}
