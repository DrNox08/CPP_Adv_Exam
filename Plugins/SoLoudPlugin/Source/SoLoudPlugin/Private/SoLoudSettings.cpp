// Fill out your copyright notice in the Description page of Project Settings.


#include "SoLoudSettings.h"

USoLoudSettings::USoLoudSettings()
{
	DefaultMaxChannels = 32;
	GlobalVolume = 1.0f;
	DistanceModel = ESoLoudDistanceModel::Inverse;
}

FName USoLoudSettings::GetCategoryName() const
{
	return TEXT("Plugins");
}