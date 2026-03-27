// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/UniquePtr.h"
#include "soloud.h"
#include "soloud_wav.h"
#include "SoLoudAudioAsset.h"
#include "SoLoudSettings.h"
#include "SoLoudSubsystem.generated.h"

USTRUCT()
struct FSoLoudWavResource
{
	GENERATED_BODY()

	SoLoud::Wav* WavInstance = nullptr;

	FSoLoudWavResource() : WavInstance(nullptr)
	{
	}
};

UCLASS()
class SOLOUDPLUGIN_API USoLoudSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Loads a SoLoud audio asset into memory so it can be played by the audio engine.
	*  Returns true if the asset was successfully loaded or already present in memory.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	bool LoadAudioAsset(USoLoudAudioAsset* AssetToLoad);
	
	/** Unloads a previously loaded audio asset from the SoLoud engine and frees its memory. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void UnloadAudioAsset(USoLoudAudioAsset* AssetToUnload);
	
	/** Returns whether the specified audio asset is currently loaded in the SoLoud engine. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	bool IsAudioAssetLoaded(USoLoudAudioAsset* Asset) const;
	
	/** Plays the specified audio asset without spatialization.
	*  Mainly intended for quick testing or debugging.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void PlayTestSound(USoLoudAudioAsset* AssetToPlay);
	
	/** Plays a 3D sound at the given world location.
	*  Returns a voice handle that can be used to control the sound later (volume, pause, stop, etc.).
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	int32 PlaySoundAtLocation(USoLoudAudioAsset* AssetToPlay, FVector Location);
	
	/** Updates the listener transform used by the SoLoud 3D audio system.
	*  Should typically be called every frame using the player camera transform.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void UpdateSoLoudListener(FVector Location, FVector Forward, FVector Up);
	
	/** Updates the world position of a playing 3D sound. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void SetSoundPosition(int32 Handle, FVector Location);
	
	/** Stops a playing sound immediately. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void StopSound(int32 Handle);
	
	/** Pauses or resumes a playing sound. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void SetSoundPaused(int32 Handle, bool bPaused);
	
	/** Sets the velocity of a 3D sound source.
	*  Used by SoLoud to compute Doppler effects.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void SetSoundVelocity(int32 Handle, FVector Velocity);
	
	/** Sets the velocity of the listener.
	*  Used to calculate Doppler effects relative to moving sound sources.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void SetListenerVelocity(FVector Velocity);
	
	/** Immediately sets the volume of a playing sound. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void SetSoundVolume(int32 Handle, float Volume);
	
	/** Gradually changes the volume of a playing sound over time. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void FadeSoundVolume(int32 Handle, float TargetVolume, float FadeTime);
	
	/** Fades the sound out over time and automatically stops it once the fade is complete. */
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void StopSoundFade(int32 Handle, float FadeTime);
	
	/** Returns whether the given voice handle still represents a valid active sound. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SoLoud")
	bool IsVoiceValid(int32 Handle) const;

private:
	SoLoud::AudioSource::ATTENUATION_MODELS ConvertDistanceModel(ESoLoudDistanceModel InModel) const;

private:
	UPROPERTY(Transient)
	TMap<USoLoudAudioAsset*, FSoLoudWavResource> LoadedAudioResources;

	TUniquePtr<SoLoud::Soloud> SoloudEngine;
};
