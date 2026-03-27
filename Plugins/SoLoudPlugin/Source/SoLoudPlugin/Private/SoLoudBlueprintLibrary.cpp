// Fill out your copyright notice in the Description page of Project Settings.


#include "SoLoudBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"

USoLoudSubsystem* USoLoudBlueprintLibrary::GetSoLoudSubsystem(const UObject* WorldContextObject)
{
	if (!IsValid(WorldContextObject))
	{
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<USoLoudSubsystem>();
}

int32 USoLoudBlueprintLibrary::PlaySoundAtLocation(const UObject* WorldContextObject, USoLoudAudioAsset* AssetToPlay,
                                                   FVector Location)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem)
	{
		return 0;
	}

	return SoLoudSubsystem->PlaySoundAtLocation(AssetToPlay, Location);
}

void USoLoudBlueprintLibrary::PlayTestSound(const UObject* WorldContextObject, USoLoudAudioAsset* AssetToPlay)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem)
	{
		return;
	}

	SoLoudSubsystem->PlayTestSound(AssetToPlay);
}

void USoLoudSubsystem::SetSoundPaused(int32 Handle, bool bPaused)
{
	if (!SoloudEngine.IsValid() || Handle <= 0)
	{
		return;
	}

	SoloudEngine->setPause(static_cast<unsigned int>(Handle), bPaused);
}

void USoLoudBlueprintLibrary::UpdateListenerFromPlayer(const UObject* WorldContextObject, int32 PlayerIndex)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem || !IsValid(WorldContextObject))
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, PlayerIndex);
	if (!PC)
	{
		return;
	}

	APlayerCameraManager* CameraManager = PC->PlayerCameraManager;
	if (!CameraManager)
	{
		return;
	}

	// posizione + rotazione camera
	const FVector Location = CameraManager->GetCameraLocation();
	const FRotator Rotation = CameraManager->GetCameraRotation();

	const FVector Forward = Rotation.Vector();
	const FVector Up = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Z);

	SoLoudSubsystem->UpdateSoLoudListener(Location, Forward, Up);

	// velocity dal pawn
	APawn* Pawn = PC->GetPawn();
	if (Pawn)
	{
		SoLoudSubsystem->SetListenerVelocity(Pawn->GetVelocity());
	}
}

void USoLoudBlueprintLibrary::SetSoundVolume(const UObject* WorldContextObject, int32 Handle, float Volume)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem)
	{
		return;
	}

	SoLoudSubsystem->SetSoundVolume(Handle, Volume);
}

void USoLoudBlueprintLibrary::FadeSoundVolume(const UObject* WorldContextObject, int32 Handle, float TargetVolume,
                                              float FadeTime)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem)
	{
		return;
	}

	SoLoudSubsystem->FadeSoundVolume(Handle, TargetVolume, FadeTime);
}

void USoLoudBlueprintLibrary::StopSoundFade(const UObject* WorldContextObject, int32 Handle, float FadeTime)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem)
	{
		return;
	}
	SoLoudSubsystem->StopSoundFade(Handle, FadeTime);
}

bool USoLoudBlueprintLibrary::IsVoiceValid(const UObject* WorldContextObject, int32 Handle)
{
	USoLoudSubsystem* SoLoudSubsystem = GetSoLoudSubsystem(WorldContextObject);
	if (!SoLoudSubsystem)
	{
		return false;
	}

	return SoLoudSubsystem->IsVoiceValid(Handle);
}
