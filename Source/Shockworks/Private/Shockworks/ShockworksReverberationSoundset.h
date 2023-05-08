// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundWave.h"
#include "ShockworksReverberationSoundset.generated.h"

/** Data asset containing the wavefiles for a detonation's reverberation. */
UCLASS()
class UShockworksReverberationSoundset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	/** The volume to play this reverberation set at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WaveFiles, Meta = (DisplayName="Volume", ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	float Volume {1.f}; 

	/** Quad A WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WaveFiles, Meta = (DisplayName="Layer A"))
	TArray<USoundWave*> LayerAWaveFiles;

	/** Quad B WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WaveFiles, Meta = (DisplayName="Layer B"))
	TArray<USoundWave*> LayerBWaveFiles;

	/** Quad C WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WaveFiles, Meta = (DisplayName="Layer C"))
	TArray<USoundWave*> LayerCWaveFiles;

	/** Quad D WaveFiles. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = WaveFiles, Meta = (DisplayName="Layer D"))
	TArray<USoundWave*> LayerDWaveFiles;
	
	/** Checks whether this soundset contains valid data. */
	UFUNCTION()
	bool ValidateSoundset();

	/** Passes the Soundset data to a MetaSound instance. Only Metasound parameters of the same name as the soundset member variables will be updated. */
	UFUNCTION()
	void ApplySoundset(UAudioComponent* AudioComponent);
};
