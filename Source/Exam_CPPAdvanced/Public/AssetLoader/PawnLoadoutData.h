// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnLoadoutData.generated.h"

/**
 * 
 */
UCLASS()
class EXAM_CPPADVANCED_API UPawnLoadoutData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual", meta=(AssetBundles="CharacterVisuals"))
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Visual")
	TSoftObjectPtr<UMaterialInterface> Material;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Accessory")
	TSoftObjectPtr<UStaticMesh> AccessoryMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Accessory")
	FName AccessorySocketName = NAME_None;
};
