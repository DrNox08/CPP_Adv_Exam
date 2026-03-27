// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Pawn.h"
#include "DataDrivenPawn.generated.h"

UCLASS()
class EXAM_CPPADVANCED_API ADataDrivenPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADataDrivenPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void InitializeFromAssetID();
	
	//Callbacks
	UFUNCTION()
	void OnPrimaryAssetLoadFinished();
	
	void OnMaterialLoaded();

	void OnAccessoryLoaded();
	
	TSharedPtr<FStreamableHandle> MaterialLoadHandle;
	TSharedPtr<FStreamableHandle> AccessoryLoadHandle;
	
	UPROPERTY()
	TObjectPtr<class USimpleLoaderSubsystem> LoaderSubsystem;
	
	UPROPERTY()
	TObjectPtr<class UPawnLoadoutData> LoadedPawnData;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> AccessoryMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data")
	FPrimaryAssetId PawnDataId;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	
};
