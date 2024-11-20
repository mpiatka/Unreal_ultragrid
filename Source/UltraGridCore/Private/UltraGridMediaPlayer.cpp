#include "UltraGridMediaPlayer.h"
#include "IMediaEventSink.h"
#include "UltraGridCoreModule.h"
#include "Logging/StructuredLog.h"
#include "Engine/Engine.h"
#include "libultragrid/libultragrid.h"
#include "libultragrid/types.h"
#include "libultragrid/video_codec.h"
#include <vector>

FUltraGridMediaPlayer::FUltraGridMediaPlayer(IMediaEventSink& sink):
	FMediaIOCorePlayerBase(sink),
	TextureSamplePool(MakeUnique<FUltraGridMediaTextureSamplePool>())
{
}

FUltraGridMediaPlayer::~FUltraGridMediaPlayer(){

}

FGuid FUltraGridMediaPlayer::GetPlayerPluginGUID() const {
	return FUltraGridCoreModule::GetPlayerPluginGUID();
}

void FUltraGridMediaPlayer::frame_arrived(struct video_frame *f)
{
	auto TextureSample = TextureSamplePool->AcquireShared();
	const FTimespan DecodedTime = FTimespan::FromSeconds(GetPlatformSeconds());
	const TOptional<FTimecode> DecodedTimecode;
	FFrameRate FrameRate(f->fps * 1000, 1000);

	auto tile = f->tiles[0];

	int lineB = vc_get_linesize(tile.width, f->color_spec);
	int dataSize = lineB * tile.height;

	//TODO: Currently assuming RGBA codec

	TextureSample->Initialize(
			tile.data,
			dataSize,
			lineB,
			tile.width,
			tile.height,
			EMediaTextureSampleFormat::CharRGBA,
			DecodedTime,
			FrameRate,
			DecodedTimecode,
			true
			);

	Samples->AddVideo(TextureSample);
}

static void ug_recv_callback(void *user, struct video_frame *f){
	auto player = static_cast<FUltraGridMediaPlayer *>(user);

	player->frame_arrived(f);
}

bool FUltraGridMediaPlayer::Open(const FString& Url, const IMediaOptions* Options) {

	FString Scheme;
	FString Location;

	UE_LOGFMT(LogUltraGridMediaCore, Log, "Player open with url {0}", Url);

	if (!Url.Split(TEXT("://"), &Scheme, &Location, ESearchCase::CaseSensitive))
	{
		UE_LOGFMT(LogUltraGridMediaCore, Warning, "Failed to split URL");
		return false;
	}

	UE_LOGFMT(LogUltraGridMediaCore, Log, "Split url into {0} and {1}", Scheme, Location);

	if (Scheme != FString("ultragridrecv")) {
		UE_LOGFMT(LogUltraGridMediaCore, Warning, "Wrong scheme {0}", Scheme);
		return false;
	}

	int Port = FCString::Atoi(*Location);
	if (Port <= 0 || Port >= (1 << 16)) {
		UE_LOGFMT(LogUltraGridMediaCore, Warning, "Wrong port {0}", Port);
		return false;
	}

	UE_LOGFMT(LogUltraGridMediaCore, Log, "Player port {0}", Port);

	if (!UltraGridLib::IsInitialized()) {
		UE_LOG(LogUltraGridMediaCore, Warning, TEXT("The Ug library is not loaded"));
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("UltraGrid library not loaded, capture will not work"));
		}
		return false;
	}

	UE_LOGFMT(LogUltraGridMediaCore, Log, "Get Ug handle");
	libug_conf ugConf{};
	libug_init_conf(&ugConf);
	ugConf.mode = LIBUG_MODE_RECV;
	ugConf.user_ptr = this;
	ugConf.rx_port = Port;
	ugConf.frame_callback = ug_recv_callback;
	UgHandle.Reset(libug_create_handle(&ugConf));

	if (!UgHandle) {
		UE_LOGFMT(LogUltraGridMediaCore, Warning, "Failed to create Ug handle");
		return false;

	}

	UE_LOGFMT(LogUltraGridMediaCore, Log, "Start recv");
	libug_start_recv(UgHandle.Get());
	
	EventSink.ReceiveMediaEvent(EMediaEvent::MediaConnecting);

	//TODO: Probably would be better to change state based on whether we are
	//currently receiving something
	EventSink.ReceiveMediaEvent(EMediaEvent::TracksChanged);
	EventSink.ReceiveMediaEvent(EMediaEvent::MediaOpened);
	EventSink.ReceiveMediaEvent(EMediaEvent::PlaybackResumed);
	CurrentState = EMediaState::Playing;

	SetRate(1);
	UE_LOGFMT(LogUltraGridMediaCore, Log, "opened");


	return true;
}

void FUltraGridMediaPlayer::Close(){
	UgHandle.Reset();
}

bool FUltraGridMediaPlayer::IsHardwareReady() const{
	return CurrentState == EMediaState::Playing;
}

void FUltraGridMediaPlayer::SetupSampleChannels(){
	
}


void FUltraGridMediaPlayer::TickFetch(FTimespan DeltaTime, FTimespan Timecode)
{
	FMediaIOCorePlayerBase::TickFetch(DeltaTime, Timecode);
}

void FUltraGridMediaPlayer::TickInput(FTimespan DeltaTime, FTimespan Timecode)
{
	FMediaIOCorePlayerBase::TickInput(DeltaTime, Timecode);

	if (CurrentState != EMediaState::Playing)
	{
		return;
	}

	TickTimeManagement();
}

TSharedPtr<FMediaIOCoreTextureSampleBase> FUltraGridMediaPlayer::AcquireTextureSample_AnyThread() const
{
	return TextureSamplePool->AcquireShared();
}
