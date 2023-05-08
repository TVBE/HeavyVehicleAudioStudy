#include "ShockworksDetonationSoundset.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UShockworksDetonationSoundset::ValidateSoundset()
{
	if(CloseLayerAWaveFiles.IsEmpty() || CloseLayerBWaveFiles.IsEmpty() || MidrangeLayerAWaveFiles.IsEmpty()
		|| MidrangeLayerBWaveFiles.IsEmpty() || DistantLayerAWaveFiles.IsEmpty() || DistantLayerBWaveFiles.IsEmpty())
	{
		const FString Asset {UKismetSystemLibrary::GetDisplayName(this)};
		UE_LOG(LogTemp, Error, TEXT("Detonation Soundset %s is missing sound files."), *Asset);
		return false;
	}
	return true;
}


void UShockworksDetonationSoundset::ApplySoundset(UAudioComponent* AudioComponent)
{
	if(ValidateSoundset())
		{
		// Set Wavefile Array values.
		AudioComponent->SetObjectArrayParameter("CloseLayerAWaveFiles", reinterpret_cast<TArray<UObject*>&>(CloseLayerAWaveFiles));
		AudioComponent->SetObjectArrayParameter("CloseLayerBWaveFiles", reinterpret_cast<TArray<UObject*>&>(CloseLayerBWaveFiles));
		AudioComponent->SetObjectArrayParameter("MidrangeLayerAWaveFiles", reinterpret_cast<TArray<UObject*>&>(MidrangeLayerAWaveFiles));
		AudioComponent->SetObjectArrayParameter("MidrangeLayerBWaveFiles", reinterpret_cast<TArray<UObject*>&>(MidrangeLayerBWaveFiles));
		AudioComponent->SetObjectArrayParameter("DistantLayerAWaveFiles", reinterpret_cast<TArray<UObject*>&>(DistantLayerAWaveFiles));
		AudioComponent->SetObjectArrayParameter("DistantLayerBWaveFiles", reinterpret_cast<TArray<UObject*>&>(DistantLayerBWaveFiles));
	
		// Set volume values
		AudioComponent->SetFloatParameter("CloseLayerAVolume", CloseLayerAVolume);
		AudioComponent->SetFloatParameter("CloseLayerBVolume", CloseLayerBVolume);
		AudioComponent->SetFloatParameter("MidrangeLayerAVolume", MidrangeLayerAVolume);
		AudioComponent->SetFloatParameter("MidrangeLayerBVolume", MidrangeLayerBVolume);
		AudioComponent->SetFloatParameter("DistantLayerAVolume", DistantLayerAVolume);
		AudioComponent->SetFloatParameter("DistantLayerBVolume", DistantLayerBVolume);
	
		// Set pitch values.
		AudioComponent->SetFloatParameter("CloseLayerAPitch", CloseLayerAPitch);
		AudioComponent->SetFloatParameter("CloseLayerBPitch", CloseLayerBPitch);
		AudioComponent->SetFloatParameter("MidrangeLayerAPitch", MidrangeLayerAPitch);
		AudioComponent->SetFloatParameter("MidrangeLayerAPitch", MidrangeLayerBPitch);
		AudioComponent->SetFloatParameter("DistantLayerAPitch", DistantLayerAPitch);
		AudioComponent->SetFloatParameter("DistantLayerBPitch", DistantLayerBPitch);
		}
	
}
