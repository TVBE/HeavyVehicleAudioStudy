// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "AssetSelection.h"
#include "Components/ActorComponent.h"
#include "Reacoustic/PhysicsHitAudioDataAsset.h"
#include "ShockworksColliderComponent.generated.h"



UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UShockworksColliderComponent : public UActorComponent
{
	
	GENERATED_BODY()
	
/**
	note to self: hiermee kan je private variabelen aceccen in bp :)
	private:
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
**/
	
protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	bool IsInitialized;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TObjectPtr<AActor> Owner;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	double Mass;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TMap<TEnumAsByte<EPhysicalSurface> ,int32> SurfaceMaterialAudioDataRef;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Default")
	double MaxSpeedBaseScalar;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double ImpactValue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double SurfaceDampening;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<UPhysicsHitAudioDataAsset> ColliderAudioDataAsset;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	FPhysicsHitAudioData ObjectColliderAudioData;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<UAudioComponent> AvailableHitSoundComponent;

public:	
	// Sets default values for this component's properties
	UShockworksColliderComponent();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
