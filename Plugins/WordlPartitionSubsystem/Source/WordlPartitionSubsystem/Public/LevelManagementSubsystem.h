// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelManagementSubsystem.generated.h"

/**
 * 
 */

class ULevelStreamingDynamic;

USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level Info")
	TSoftObjectPtr<UWorld> LevelToLoad;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Level Info")
	FText LevelName;

};

UCLASS()
class WORDLPARTITIONSUBSYSTEM_API ULevelManagementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	ULevelManagementSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Level Management")
	TMap<FString, FLevelInfo> LevelsTemplatesMap;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Level Management")
	FRandomStream DungeonRandomSeed;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Level Management")
	int32 CurrentRunSeed = 0;
	
	UFUNCTION(BlueprintCallable, Category="Level Management")
	int32 GenerateAndSetRunSeed(int32 OverrideSeed = -1);

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	ULevelStreamingDynamic* LoadNextRoom(FTransform LevelSpawnTransform, bool& OutSuccess, const FString& OptionalLevelNameOverride, TSubclassOf<ULevelStreamingDynamic> OptionalLevelStreamingClass, bool bLoadAsTempPackage);

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void SetRandomRunSeed();

	UFUNCTION(BlueprintCallable, Category = "Level Management")
	void SetRunSeed(int32 NewSeed);
	
	
	
	
	
};
