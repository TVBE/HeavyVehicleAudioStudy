// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "ShockworksEventActor.h"
#include "ShockworksDetonationEventActor.generated.h"

/** Event Actor that handles detonation events in the world. Derives from ShockworksEventActor. */
UCLASS(Blueprintable, BlueprintType)
class SHOCKWORKS_API AShockworksDetonationEventActor : public AShockworksEventActor
{
	GENERATED_BODY()

private:
	/** Audio Component for playing the detonation source audio. */
	UPROPERTY(VisibleAnywhere, Category = Components, Meta = (DisplayName = "Source Audio Component", AllowPrivateAcces = "true"))
	UAudioComponent* SourceAudioComponent;
	
	/** Component handling the audio of detonation fragments hitting the surrounding area and whizzing by the listener. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (DisplayName= "Fragmentation Component", AllowPrivateAccess = "true"))
	class UFragmentationComponent* FragmentationComponent;

	/** Component handling debris audio after the detonation. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (DisplayName= "Debris Component", AllowPrivateAccess = "true"))
	class UDebrisComponent* DebrisComponent;

	/** The MetaSound to use for the SourceAudioComponent */
	UPROPERTY()
	class UMetaSoundSource* SourceMetaSound;

public:
	// Sets default values for this actor's properties
	AShockworksDetonationEventActor();

public:
	bool StartEvent() override;

	/** Returns FragmentationComponent subobject. */
	FORCEINLINE UFragmentationComponent* GetFragmentationComponent() const { return FragmentationComponent; }

	/** Returns DebrisComponent subobject. */
	FORCEINLINE UDebrisComponent* GetDebrisComponent() const { return DebrisComponent; }
	
private:
	
	/** Sets the detonation event for this event actor and updates the MetaSound instances.*/
	bool AssignEvent(UShockworksEvent* Event) override;
};
