// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManagementSubsystem.h"

#include "DungeonSettings.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/KismetMathLibrary.h"

ULevelManagementSubsystem::ULevelManagementSubsystem()
{
}

void ULevelManagementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UDungeonSettings* Settings = GetDefault<UDungeonSettings>();
	if (Settings)
	{
		LevelsTemplatesMap = Settings->LevelsTemplatesMap;
	}
}

void ULevelManagementSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

ULevelStreamingDynamic* ULevelManagementSubsystem::LoadNextRoom(FTransform LevelSpawnTransform, bool& OutSuccess,
                                                                const FString& OptionalLevelNameOverride,
                                                                TSubclassOf<ULevelStreamingDynamic>
                                                                OptionalLevelStreamingClass,
                                                                bool bLoadAsTempPackage)
{
	OutSuccess = false;

	if (LevelsTemplatesMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelsTemplatesMap is empty. Please check your DungeonSettings."));
		return nullptr;
	}
	
	TArray<FString> Keys;
	LevelsTemplatesMap.GetKeys(Keys);

	if (Keys.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelsTemplatesMap has no keys."));
		return nullptr;
	}
	
	Keys.Sort();
	
	const int32 RandomIndex = DungeonRandomSeed.RandRange(0, Keys.Num() - 1);
	const FString& Key = Keys[RandomIndex];

	const FLevelInfo LevelToLoad = LevelsTemplatesMap.FindRef(Key);

	return ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
		this,
		LevelToLoad.LevelToLoad,
		LevelSpawnTransform,
		OutSuccess,
		OptionalLevelNameOverride,
		OptionalLevelStreamingClass,
		bLoadAsTempPackage);
}

void ULevelManagementSubsystem::SetRandomRunSeed()
{
	GenerateAndSetRunSeed(-1);
}

void ULevelManagementSubsystem::SetRunSeed(int32 NewSeed)
{
	GenerateAndSetRunSeed(NewSeed);
}

int32 ULevelManagementSubsystem::GenerateAndSetRunSeed(int32 OverrideSeed)
{
	const UDungeonSettings* Settings = GetDefault<UDungeonSettings>();
	
	if (OverrideSeed != -1)
	{
		CurrentRunSeed = OverrideSeed;
	}
	else if (Settings && Settings->UseFixedSeed)
	{
		CurrentRunSeed = Settings->FixedSeedToUse;
	}
	else
	{
		CurrentRunSeed = static_cast<int32>(FDateTime::Now().GetTicks());
	}
	
	DungeonRandomSeed.Initialize(CurrentRunSeed);

	UE_LOG(LogTemp, Log, TEXT("Run Seed Initialized: %d"), CurrentRunSeed);

	return CurrentRunSeed;
}
