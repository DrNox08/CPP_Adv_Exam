#include "SoLoudAudioComponent.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "SoLoudAudioAsset.h"
#include "SoLoudSubsystem.h"

USoLoudAudioComponent::USoLoudAudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USoLoudAudioComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			CachedSubsystem = GameInstance->GetSubsystem<USoLoudSubsystem>();
		}
	}

	if (bAutoPlay)
	{
		Play();
	}
}

void USoLoudAudioComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Stop();
	Super::EndPlay(EndPlayReason);
}

void USoLoudAudioComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!CachedSubsystem || VoiceHandle <= 0)
	{
		return;
	}

	if (!CachedSubsystem->IsVoiceValid(VoiceHandle))
	{
		VoiceHandle = 0;
		bIsCulled = false;
		return;
	}

	// aggiorna sempre la posizione del suono
	CachedSubsystem->SetSoundPosition(VoiceHandle, GetComponentLocation());

	AActor* Owner = GetOwner();
	if (Owner)
	{
		CachedSubsystem->SetSoundVelocity(VoiceHandle, Owner->GetVelocity());
	}

	if (!bEnableDistanceCulling)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerCameraManager* CameraManager = World->GetFirstPlayerController()->PlayerCameraManager;
	if (!CameraManager)
	{
		return;
	}

	const FVector CameraLocation = CameraManager->GetCameraLocation();
	const float Distance = FVector::Distance(CameraLocation, GetComponentLocation());

	if (Distance > CullDistance)
	{
		if (!bIsCulled)
		{
			CachedSubsystem->SetSoundPaused(VoiceHandle, true);
			bIsCulled = true;
		}
	}
	else
	{
		if (bIsCulled)
		{
			CachedSubsystem->SetSoundPaused(VoiceHandle, false);
			bIsCulled = false;
		}
	}
}

void USoLoudAudioComponent::Play()
{
	if (!CachedSubsystem || !IsValid(AudioAsset))
	{
		return;
	}

	if (VoiceHandle > 0)
	{
		CachedSubsystem->StopSound(VoiceHandle);
		VoiceHandle = 0;
	}

	VoiceHandle = CachedSubsystem->PlaySoundAtLocation(AudioAsset, GetComponentLocation());
}

void USoLoudAudioComponent::Stop()
{
	if (CachedSubsystem && VoiceHandle > 0)
	{
		CachedSubsystem->StopSound(VoiceHandle);
		VoiceHandle = 0;
	}
}

void USoLoudAudioComponent::StopFade(float FadeTime)
{
	if (CachedSubsystem && VoiceHandle > 0)
	{
		CachedSubsystem->StopSoundFade(VoiceHandle, FadeTime);
	}
}
