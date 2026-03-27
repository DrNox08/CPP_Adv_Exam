// Fill out your copyright notice in the Description page of Project Settings.

#include "SoLoudSubsystem.h"

#include "SoLoudSettings.h"

void USoLoudSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SoloudEngine = MakeUnique<SoLoud::Soloud>();

	const USoLoudSettings* Settings = GetDefault<USoLoudSettings>();

	// SoLoud FLAGS
	uint32 Flags = SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::LEFT_HANDED_3D;

	SoloudEngine->init(Flags);

	if (Settings)
	{
		SoloudEngine->setGlobalVolume(Settings->GlobalVolume);

		UE_LOG(LogTemp, Log, TEXT("SoLoud initialized. MaxChannels=%d GlobalVolume=%.2f DistanceModel=%d"),
		       Settings->DefaultMaxChannels,
		       Settings->GlobalVolume,
		       static_cast<int32>(Settings->DistanceModel));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SoLoud initialized, but settings were not found."));
	}
}

void USoLoudSubsystem::Deinitialize()
{
	if (SoloudEngine.IsValid())
	{
		SoloudEngine->stopAll();
		SoloudEngine->deinit();
	}

	for (auto& Pair : LoadedAudioResources)
	{
		if (Pair.Value.WavInstance)
		{
			delete Pair.Value.WavInstance;
			Pair.Value.WavInstance = nullptr;
		}
	}

	LoadedAudioResources.Empty();

	Super::Deinitialize();
}

bool USoLoudSubsystem::LoadAudioAsset(USoLoudAudioAsset* AssetToLoad)
{
	if (!SoloudEngine.IsValid())
	{
		return false;
	}

	if (!IsValid(AssetToLoad) || AssetToLoad->RawAudioData.IsEmpty())
	{
		return false;
	}

	if (LoadedAudioResources.Contains(AssetToLoad))
	{
		return true;
	}

	FSoLoudWavResource& NewResource = LoadedAudioResources.Add(AssetToLoad);
	NewResource.WavInstance = new SoLoud::Wav();

	const SoLoud::result LoadResult = NewResource.WavInstance->loadMem(
		AssetToLoad->RawAudioData.GetData(),
		AssetToLoad->RawAudioData.Num(),
		false,
		false
	);

	if (LoadResult != SoLoud::SO_NO_ERROR)
	{
		delete NewResource.WavInstance;
		NewResource.WavInstance = nullptr;
		LoadedAudioResources.Remove(AssetToLoad);

		UE_LOG(LogTemp, Warning, TEXT("Failed to load audio asset into SoLoud."));
		return false;
	}

	return true;
}

void USoLoudSubsystem::UnloadAudioAsset(USoLoudAudioAsset* AssetToUnload)
{
	if (FSoLoudWavResource* Resource = LoadedAudioResources.Find(AssetToUnload))
	{
		if (Resource->WavInstance)
		{
			delete Resource->WavInstance;
			Resource->WavInstance = nullptr;
		}

		LoadedAudioResources.Remove(AssetToUnload);
	}
}

bool USoLoudSubsystem::IsAudioAssetLoaded(USoLoudAudioAsset* Asset) const
{
	return LoadedAudioResources.Contains(Asset);
}

void USoLoudSubsystem::PlayTestSound(USoLoudAudioAsset* AssetToPlay)
{
	if (!SoloudEngine.IsValid())
	{
		return;
	}

	if (!LoadAudioAsset(AssetToPlay))
	{
		return;
	}

	if (FSoLoudWavResource* Resource = LoadedAudioResources.Find(AssetToPlay))
	{
		SoloudEngine->play(*Resource->WavInstance);
	}
}

int32 USoLoudSubsystem::PlaySoundAtLocation(USoLoudAudioAsset* AssetToPlay, FVector Location)
{
	if (!SoloudEngine.IsValid())
	{
		return 0;
	}

	if (!LoadAudioAsset(AssetToPlay))
	{
		return 0;
	}

	FSoLoudWavResource* Resource = LoadedAudioResources.Find(AssetToPlay);
	if (!Resource || !Resource->WavInstance)
	{
		return 0;
	}

	const USoLoudSettings* Settings = GetDefault<USoLoudSettings>();
	const SoLoud::AudioSource::ATTENUATION_MODELS AttenuationMode =
		Settings ? ConvertDistanceModel(Settings->DistanceModel) : SoLoud::AudioSource::INVERSE_DISTANCE;

	Resource->WavInstance->set3dMinMaxDistance(1.0f, 50.0f);
	Resource->WavInstance->set3dAttenuation(AttenuationMode, 1.0f);

	const unsigned int Handle = SoloudEngine->play3d(
		*Resource->WavInstance,
		Location.X / 100.0f,
		Location.Y / 100.0f,
		Location.Z / 100.0f
	);

	SoloudEngine->update3dAudio();

	return static_cast<int32>(Handle);
}

void USoLoudSubsystem::UpdateSoLoudListener(FVector Location, FVector Forward, FVector Up)
{
	if (!SoloudEngine.IsValid())
	{
		return;
	}

	SoloudEngine->set3dListenerParameters(
		Location.X / 100.0f, Location.Y / 100.0f, Location.Z / 100.0f,
		Forward.X, Forward.Y, Forward.Z,
		Up.X, Up.Y, Up.Z
	);

	SoloudEngine->update3dAudio();
}

SoLoud::AudioSource::ATTENUATION_MODELS USoLoudSubsystem::ConvertDistanceModel(ESoLoudDistanceModel InModel) const
{
	switch (InModel)
	{
	case ESoLoudDistanceModel::Linear:
		return SoLoud::AudioSource::LINEAR_DISTANCE;

	case ESoLoudDistanceModel::Exponential:
		return SoLoud::AudioSource::EXPONENTIAL_DISTANCE;

	case ESoLoudDistanceModel::Inverse:
	default:
		return SoLoud::AudioSource::INVERSE_DISTANCE;
	}
}

void USoLoudSubsystem::SetSoundPosition(int32 Handle, FVector Location)
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return;
	}

	SoloudEngine->set3dSourcePosition(
		static_cast<unsigned int>(Handle),
		Location.X / 100.0f,
		Location.Y / 100.0f,
		Location.Z / 100.0f
	);

	SoloudEngine->update3dAudio();
}

void USoLoudSubsystem::StopSound(int32 Handle)
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return;
	}

	SoloudEngine->stop(static_cast<unsigned int>(Handle));
}

void USoLoudSubsystem::SetSoundVelocity(int32 Handle, FVector Velocity)
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return;
	}
	SoloudEngine->set3dSourceVelocity(
		static_cast<unsigned int>(Handle),
		Velocity.X / 100.0f,
		Velocity.Y / 100.0f,
		Velocity.Z / 100.0f);
}

void USoLoudSubsystem::SetListenerVelocity(FVector Velocity)
{
	if (!SoloudEngine.IsValid())
	{
		return;
	}

	SoloudEngine->set3dListenerVelocity(
		Velocity.X / 100.0f,
		Velocity.Y / 100.0f,
		Velocity.Z / 100.0f);
}

void USoLoudSubsystem::SetSoundVolume(int32 Handle, float Volume)
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return;
	}

	SoloudEngine->setVolume(static_cast<unsigned int>(Handle), FMath::Max(0.0f, Volume));
}

void USoLoudSubsystem::FadeSoundVolume(int32 Handle, float TargetVolume, float FadeTime)
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return;
	}

	SoloudEngine->fadeVolume(
		static_cast<unsigned int>(Handle),
		FMath::Max(0.0f, TargetVolume),
		FMath::Max(0.0f, FadeTime)
	);
}

void USoLoudSubsystem::StopSoundFade(int32 Handle, float FadeTime)
{
	if (!SoloudEngine.IsValid() || Handle <= 0) { return; }

	const unsigned int VoiceHandle = static_cast<unsigned int>(Handle);
	const float ClampedFadeTime = FMath::Max(0.0f, FadeTime);

	SoloudEngine->fadeVolume(VoiceHandle, 0.0f, ClampedFadeTime);
	SoloudEngine->scheduleStop(VoiceHandle, ClampedFadeTime);
}

bool USoLoudSubsystem::IsVoiceValid(int32 Handle) const
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return false;
	}

	return SoloudEngine->isValidVoiceHandle(static_cast<unsigned int>(Handle));
}
