// Copyright 2023 Nino Saglia & Tim Verberne

#include "ShockworksReverberationSoundset.h"
#include "Shockworks/Shockworks.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AudioComponent.h"


bool UShockworksReverberationSoundset::ValidateSoundset()
{
	if(LayerAWaveFiles.IsEmpty() || LayerBWaveFiles.IsEmpty() || LayerCWaveFiles.IsEmpty() || LayerDWaveFiles.IsEmpty())
	{
		const FString Asset {UKismetSystemLibrary::GetDisplayName(this)};
		UE_LOG(LogShockworks, Error, TEXT("Reverberation Soundset %s is missing sound files."), *Asset);
		return false;
	}
	return true;
}

void UShockworksReverberationSoundset::ApplySoundset(UAudioComponent* AudioComponent)
{
	if(ValidateSoundset())
	{
		// Set Wavefile Array values.
		AudioComponent->SetObjectArrayParameter("ExteriorReverberationLayerAWaveFiles", reinterpret_cast<TArray<UObject*>&>(LayerAWaveFiles));
		AudioComponent->SetObjectArrayParameter("ExteriorReverberationLayerBWaveFiles", reinterpret_cast<TArray<UObject*>&>(LayerBWaveFiles));
		AudioComponent->SetObjectArrayParameter("ExteriorReverberationLayerCWaveFiles", reinterpret_cast<TArray<UObject*>&>(LayerCWaveFiles));
		AudioComponent->SetObjectArrayParameter("ExteriorReverberationLayerDWaveFiles", reinterpret_cast<TArray<UObject*>&>(LayerDWaveFiles));

		// Set volume value.
		AudioComponent->SetFloatParameter("ExteriorReverberationVolume", Volume);
	}
}


