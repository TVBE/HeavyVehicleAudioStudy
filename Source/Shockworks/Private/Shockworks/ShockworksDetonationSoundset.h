// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundWave.h"
#include "ShockworksDetonationSoundset.generated.h"

/** Data asset containing the wavefiles and mix modifiers for a Shockworks Detonation. */
UCLASS()
class UShockworksDetonationSoundset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	/** Layer A WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Close, Meta = (DisplayName="Layer A"))
	TArray<USoundWave*> CloseLayerAWaveFiles;

	/** Defines the volume to play layer A at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Close, Meta = (DisplayName="Layer A Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float CloseLayerAVolume = 1.0;

	/** Defines the pitch shift to play layer A at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Close, Meta = (DisplayName="Layer A Pitch", ClampMin = "-12.0", ClampMax = "12.0", UIMin = "-12.0", UIMax = "12.0"))
	float CloseLayerAPitch = 0.0;

	/** Layer B WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Close, Meta = (DisplayName="Layer B"))
	TArray<USoundWave*> CloseLayerBWaveFiles;

	/** Defines the volume to play layer B at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Close, Meta = (DisplayName="Layer B Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float CloseLayerBVolume = 1.0;

	/** Defines the pitch shift to play layer B at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Close, Meta = (DisplayName="Layer B Pitch", ClampMin = "-12.0", ClampMax = "12.0", UIMin = "-12.0", UIMax = "12.0"))
	float CloseLayerBPitch = 0.0;
	
	/** Layer A WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Midrange, Meta = (DisplayName="Layer A"))
	TArray<USoundWave*> MidrangeLayerAWaveFiles;

	/** Defines the volume to play layer A at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category =  Midrange, Meta = (DisplayName="Layer A Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float MidrangeLayerAVolume = 1.0;

	/** Defines the pitch shift to play layer A at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Midrange, Meta = (DisplayName="Layer A Pitch", ClampMin = "-12.0", ClampMax = "12.0", UIMin = "-12.0", UIMax = "12.0"))
	float MidrangeLayerAPitch = 0.0;

	/** Layer B WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Midrange, Meta = (DisplayName="Layer B"))
	TArray<USoundWave*> MidrangeLayerBWaveFiles;

	/** Defines the volume to play layer B at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Midrange, Meta = (DisplayName="Layer B Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float MidrangeLayerBVolume = 1.0;

	/** Defines the pitch shift to play layer B at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Midrange, Meta = (DisplayName="Layer B Pitch", ClampMin = "-12.0", ClampMax = "12.0", UIMin = "-12.0", UIMax = "12.0"))
	float MidrangeLayerBPitch = 0.0;
	
	/** Layer A WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distant, Meta = (DisplayName="Layer A"))
	TArray<USoundWave*> DistantLayerAWaveFiles;

	/** Defines the volume to play layer A at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category =  Distant, Meta = (DisplayName="Layer A Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float DistantLayerAVolume = 1.0;

	/** Defines the pitch shift to play layer A at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distant, Meta = (DisplayName="Layer A Pitch", ClampMin = "-12.0", ClampMax = "12.0", UIMin = "-12.0", UIMax = "12.0"))
	float DistantLayerAPitch = 0.0;

	/** Layer B WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distant, Meta = (DisplayName="Layer B"))
	TArray<USoundWave*> DistantLayerBWaveFiles;

	/** Defines the volume to play layer B at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distant, Meta = (DisplayName="Layer B Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float DistantLayerBVolume = 1.0;

	/** Defines the pitch shift to play layer B at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distant, Meta = (DisplayName="Layer B Pitch", ClampMin = "-12.0", ClampMax = "12.0", UIMin = "-12.0", UIMax = "12.0"))
	float DistantLayerBPitch = 0.0;

	/** Checks whether this soundset contains valid data. */
	UFUNCTION()
	bool ValidateSoundset();
	
	/** Passes the Soundset data to a MetaSound instance. Only Metasound parameters of the same name as the soundset member variables will be updated. */
	UFUNCTION()
	void ApplySoundset(UAudioComponent* AudioComponent);
};
