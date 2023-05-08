// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ShockworksEvent.generated.h"

/** Base class for creating Shockworks Events.
 */
UCLASS(Abstract, NotBlueprintable)
class UShockworksEvent : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Optional description for the event. */
	UPROPERTY(EditInstanceOnly, Category = Description)
	FText Description;

	/** Optional tags for the event. */
	UPROPERTY(EditInstanceOnly, Category = Description)
	TArray<FText> Tags;

	/** Checks whether this event is valid and can be played. */
	virtual bool Validate();
	
};
