// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoLoudAudioAsset.h"
#include "SoLoudSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoLoudBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SOLOUDPLUGIN_API USoLoudBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	/** Blueprint helper that returns the active SoLoud audio subsystem for the given world context. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static USoLoudSubsystem* GetSoLoudSubsystem(const UObject* WorldContextObject);
	
	/** Blueprint helper that plays a 3D sound at the given world location using the SoLoud audio subsystem.
	*  Returns a voice handle that can be used to control the sound later.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static int32 PlaySoundAtLocation(const UObject* WorldContextObject, USoLoudAudioAsset* AssetToPlay, FVector Location);
	
	/** Blueprint helper that plays the specified audio asset without spatialization.
	*  Mainly intended for quick testing or debugging.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static void PlayTestSound(const UObject* WorldContextObject, USoLoudAudioAsset* AssetToPlay);
	
	/** Blueprint helper that updates the SoLoud listener from the specified player's camera and pawn.
	*  Updates listener position, orientation, and velocity for 3D audio and Doppler effects.
	*/
	UFUNCTION(BlueprintCallable, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static void UpdateListenerFromPlayer(const UObject* WorldContextObject, int32 PlayerIndex = 0);
	
	/** Blueprint helper that immediately sets the volume of a playing sound using its voice handle. */
	UFUNCTION(BlueprintCallable, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static void SetSoundVolume(const UObject* WorldContextObject, int32 Handle, float Volume);
	
	/** Blueprint helper that gradually changes the volume of a playing sound over time using its voice handle. */
	UFUNCTION(BlueprintCallable, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static void FadeSoundVolume(const UObject* WorldContextObject, int32 Handle, float TargetVolume, float FadeTime);
	
	/** Blueprint helper that fades a sound out over time and automatically stops it when the fade is complete. */
	UFUNCTION(BlueprintCallable, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static void StopSoundFade(const UObject* WorldContextObject, int32 Handle, float FadeTime);
	
	/** Blueprint helper that returns whether the given voice handle is still valid in the SoLoud audio subsystem. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="SoLoud", meta=(WorldContext="WorldContextObject"))
	static bool IsVoiceValid(const UObject* WorldContextObject, int32 Handle);
	
};
