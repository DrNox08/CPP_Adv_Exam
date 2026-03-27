// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SoLoudAudioComponent.generated.h"

class USoLoudAudioAsset;
class USoLoudSubsystem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLOUDPLUGIN_API USoLoudAudioComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USoLoudAudioComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoLoud")
	USoLoudAudioAsset* AudioAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoLoud")
	bool bAutoPlay = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoLoud|Culling")
	bool bEnableDistanceCulling = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoLoud|Culling", meta=(ClampMin="0"))
	float CullDistance = 2000.f;

	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void Play();

	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void Stop();
	
	UFUNCTION(BlueprintCallable, Category="SoLoud")
	void StopFade(float FadeTime);

private:
	UPROPERTY(Transient)
	USoLoudSubsystem* CachedSubsystem = nullptr;

	UPROPERTY(Transient)
	int32 VoiceHandle = 0;
	
	bool bIsCulled = false;
};
