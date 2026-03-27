// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SoLoudSettings.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESoLoudDistanceModel : uint8
{
	Linear      UMETA(DisplayName = "Linear"),
	Inverse     UMETA(DisplayName = "Inverse"),
	Exponential UMETA(DisplayName = "Exponential")
};

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="SoLoud Settings"))
class SOLOUDPLUGIN_API USoLoudSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	USoLoudSettings();

	// Numero massimo di canali audio simultanei
	UPROPERTY(EditAnywhere, Config, Category="General", meta=(ClampMin="1", UIMin="1"))
	int32 DefaultMaxChannels;

	// Volume globale del sistema audio
	UPROPERTY(EditAnywhere, Config, Category="General", meta=(ClampMin="0.0", UIMin="0.0"))
	float GlobalVolume;

	// Modello di attenuazione 3D di default
	UPROPERTY(EditAnywhere, Config, Category="3D Audio")
	ESoLoudDistanceModel DistanceModel;

	virtual FName GetCategoryName() const override;
	
};
