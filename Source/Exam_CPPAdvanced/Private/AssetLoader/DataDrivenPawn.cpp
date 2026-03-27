// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader/DataDrivenPawn.h"

#include "SimpleLoaderSubsystem.h"
#include "AssetLoader/PawnLoadoutData.h"

// Sets default values
ADataDrivenPawn::ADataDrivenPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	SetRootComponent(CharacterMesh);

	AccessoryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AccessoryMesh"));
	AccessoryMesh->SetupAttachment(CharacterMesh);
}

// Called when the game starts or when spawned
void ADataDrivenPawn::BeginPlay()
{
	Super::BeginPlay();
#pragma region Init
	UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance non valida."));
		return;
	}

	LoaderSubsystem = GameInstance->GetSubsystem<USimpleLoaderSubsystem>();
	if (!IsValid(LoaderSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("SimpleLoaderSubsystem non trovato."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("SimpleLoaderSubsystem trovato correttamente."));

#pragma endregion

	InitializeFromAssetID();
}

void ADataDrivenPawn::InitializeFromAssetID()
{
	if (!PawnDataId.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PawnDataId non valido."));
		return;
	}

	LoaderSubsystem->OnLoadingFinished.AddDynamic(this, &ADataDrivenPawn::OnPrimaryAssetLoadFinished);

	TArray<FPrimaryAssetId> AssetsToLoad;
	AssetsToLoad.Add(PawnDataId);

	TArray<FName> Bundles;
	Bundles.Add(TEXT("CharacterVisuals"));

	LoaderSubsystem->LoadPrimaryAssetsWithBundles(AssetsToLoad, Bundles);

	UE_LOG(LogTemp, Log, TEXT("Richiesto caricamento async del Primary Asset: %s"), *PawnDataId.ToString());
}

void ADataDrivenPawn::OnPrimaryAssetLoadFinished()
{
#pragma region Setup
	if (!IsValid(LoaderSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoaderSubsystem non valido nella callback."));
		return;
	}

	UPrimaryDataAsset* LoadedAsset = LoaderSubsystem->
		GetLoadedPrimaryAsset(PawnDataId, UPawnLoadoutData::StaticClass());
	if (!IsValid(LoadedAsset))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetLoadedPrimaryAsset ha restituito nullptr."));
		return;
	}

	UPawnLoadoutData* PawnLoadoutData = Cast<UPawnLoadoutData>(LoadedAsset);
	if (!IsValid(PawnLoadoutData))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast a UPawnLoadoutData fallito."));
		return;
	}

	LoadedPawnData = PawnLoadoutData;

	UE_LOG(LogTemp, Log, TEXT("PawnLoadoutData recuperato correttamente: %s"), *PawnLoadoutData->GetName());

#pragma endregion

#pragma region SkeletalMesh
	if (PawnLoadoutData->SkeletalMesh.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("Lo SkeletalMesh nel Data Asset non è assegnato."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Soft reference SkeletalMesh trovata: %s"),
	       *PawnLoadoutData->SkeletalMesh.ToSoftObjectPath().ToString());

	USkeletalMesh* LoadedSkeletalMesh = PawnLoadoutData->SkeletalMesh.Get();
	if (!IsValid(LoadedSkeletalMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("Lo SkeletalMesh non risulta risolto in memoria."));
		return;
	}

	if (!IsValid(CharacterMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMesh non valido."));
		return;
	}

	CharacterMesh->SetSkeletalMesh(LoadedSkeletalMesh);
	UE_LOG(LogTemp, Log, TEXT("SkeletalMesh applicato correttamente al Pawn: %s"), *LoadedSkeletalMesh->GetName());

#pragma endregion

#pragma region Material

	if (LoadedPawnData->Material.IsNull())
	{
		UE_LOG(LogTemp, Log, TEXT("Nessun Material assegnato nel Data Asset."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Material soft reference trovata: %s"),
		       *LoadedPawnData->Material.ToSoftObjectPath().ToString());
	}

	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

	MaterialLoadHandle = StreamableManager.RequestAsyncLoad(
		LoadedPawnData->Material.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ADataDrivenPawn::OnMaterialLoaded)
	);
#pragma endregion
	
#pragma region AccessoryMesh
	if (LoadedPawnData->AccessoryMesh.IsNull())
	{
		UE_LOG(LogTemp, Log, TEXT("Nessuna AccessoryMesh assegnata nel Data Asset."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("AccessoryMesh soft reference trovata: %s"),
		       *LoadedPawnData->AccessoryMesh.ToSoftObjectPath().ToString());

		//FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();

		AccessoryLoadHandle = StreamableManager.RequestAsyncLoad(
			LoadedPawnData->AccessoryMesh.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ADataDrivenPawn::OnAccessoryLoaded)
		);
	}
#pragma endregion
}

void ADataDrivenPawn::OnMaterialLoaded()
{
	if (!IsValid(LoadedPawnData))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadedPawnData non valido in OnMaterialLoaded."));
		return;
	}

	if (!IsValid(CharacterMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMesh non valido in OnMaterialLoaded."));
		return;
	}

	UMaterialInterface* LoadedMaterial = LoadedPawnData->Material.Get();
	if (!IsValid(LoadedMaterial))
	{
		UE_LOG(LogTemp, Warning, TEXT("Il materiale non risulta risolto in memoria."));
		return;
	}

	CharacterMesh->SetMaterial(1, LoadedMaterial);
	//CharacterMesh->SetMaterial(1, LoadedMaterial);

	UE_LOG(LogTemp, Log, TEXT("Material applicato correttamente: %s"), *LoadedMaterial->GetName());
}

void ADataDrivenPawn::OnAccessoryLoaded()
{
	if (!IsValid(LoadedPawnData))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadedPawnData non valido in OnAccessoryLoaded."));
		return;
	}

	if (!IsValid(CharacterMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterMesh non valido in OnAccessoryLoaded."));
		return;
	}

	if (!IsValid(AccessoryMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("AccessoryMesh component non valido."));
		return;
	}

	UStaticMesh* LoadedAccessoryMesh = LoadedPawnData->AccessoryMesh.Get();
	if (!IsValid(LoadedAccessoryMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("La AccessoryMesh non risulta risolta in memoria."));
		return;
	}

	AccessoryMesh->SetStaticMesh(LoadedAccessoryMesh);

	if (LoadedPawnData->AccessorySocketName != NAME_None)
	{
		AccessoryMesh->AttachToComponent(
			CharacterMesh,
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			LoadedPawnData->AccessorySocketName
		);

		UE_LOG(LogTemp, Log, TEXT("AccessoryMesh agganciata al socket: %s"),
		       *LoadedPawnData->AccessorySocketName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("AccessorySocketName non assegnato. Accessorio caricato ma non agganciato a un socket specifico."));
	}
}

// Called every frame
void ADataDrivenPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ADataDrivenPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
