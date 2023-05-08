// Copyright 2023 Nino Saglia & Tim Verberne


#include "Shockworks/ShockworksDetonationEventActor.h"
#include "Shockworks/Shockworks.h"
#include "MetasoundSource.h"
#include "Sound/SoundWave.h"
#include "FragmentationComponent.h"
#include "DebrisComponent.h"
#include "Shockworks/ShockworksDetonationEvent.h"
#include "ShockworksReverberationSoundset.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets the default values.
AShockworksDetonationEventActor::AShockworksDetonationEventActor()
{
	/** Since we do not need our DetonationEvent to update every frane, we can disable Tick for this class. */
	PrimaryActorTick.bCanEverTick = false;

	/** Create components. */
	SourceAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SourceAudioComponent"));
	SourceAudioComponent->SetupAttachment(this->RootComponent);
	static ConstructorHelpers::FObjectFinder<UMetaSoundSource> SourceMetaSoundObject(TEXT("/Game/Shockworks/Audio/MetaSounds/Sources/MSS_ShockworksDetonationSource"));
	if(SourceMetaSoundObject.Succeeded())
	{
		SourceAudioComponent->SetAutoActivate(false);
		SourceAudioComponent->SetSound(SourceMetaSoundObject.Object);
		SourceAudioComponent->SetBoolParameter("EnableBinauralSimulation", true);
	}
	
	FragmentationComponent = CreateDefaultSubobject<UFragmentationComponent>(TEXT("FragmentationComponent"));
	
	DebrisComponent = CreateDefaultSubobject<UDebrisComponent>(TEXT("DebrisComponent"));
}

bool AShockworksDetonationEventActor::StartEvent()
{
	if (AShockworksEventActor::StartEvent())
	{
		if(SourceAudioComponent != nullptr && IsValid(SourceAudioComponent->GetSound()))
		{
				Cast<UShockworksDetonationEvent>(GetAssignedEvent())->ReverberationSoundset->ApplySoundset(SourceAudioComponent);
				GetEventWorldData().SetMetaSoundParameters(SourceAudioComponent);
				SourceAudioComponent->Play();
		}
		return true;
	}
	return false;
}

bool AShockworksDetonationEventActor::AssignEvent(UShockworksEvent* Event)
{
	if(Event->GetClass() != UShockworksDetonationEvent::StaticClass())
	{
		const FString EventType {UKismetSystemLibrary::GetDisplayName(Event)};
		UE_LOG(LogShockworksEventActor, Error, TEXT("Attempted to assign incorrect event type %s to DetonationEventActor."), *EventType);
		return false;
	}
	if(AShockworksEventActor::AssignEvent(Event))
	{
		Cast<UShockworksDetonationEvent>(Event)->Soundset->ApplySoundset(SourceAudioComponent);
		return true;
	}
	return false;
}







