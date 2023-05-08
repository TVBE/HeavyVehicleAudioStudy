// Copyright 2023 Nino Saglia & Tim Verberne


#include "EventWorldData.h"
#include "Components/AudioComponent.h"

void FEventWorldData::SetMetaSoundParameters(UAudioComponent* AudioComponent)
{
	AudioComponent->SetFloatParameter("DistanceToListener", DistanceToListener);
	AudioComponent->SetFloatParameter("DelayToListener", DelayToListener);
	AudioComponent->SetFloatParameter("Obstruction", Obstruction);
	AudioComponent->SetFloatParameter("Enclosure", EventEnclosure);
}
