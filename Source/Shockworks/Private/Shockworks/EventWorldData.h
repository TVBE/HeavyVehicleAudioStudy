// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "EventWorldData.generated.h"


/** Struct that contains data about a Shockworks event's world location and context in relation to the listener.
 *  This data is calculated and passed to observers via the BEGIN EVENT delegate whenever a event is triggered in an ShockworksDetonationEventActor instance. */
USTRUCT(BlueprintType)
struct FEventWorldData
{
	GENERATED_USTRUCT_BODY()
	
	/** When set to true, this event is assumed to be static. This allows observers to perform certain calculations only once instead of updating per frame. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = General)
	bool IsStatic {true};
	
	/** The distance between the event and the listener in units. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distance)
	int DistanceToListener {0};

	/** The time for the sound waves of the event to travel to the listener, multiplied by a global 'speed of sound' multiplier. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Distance, meta = (ClampMin = 0.0))
	float DelayToListener {0.f};

	/** The amount that the event is enclosed by geometry. This value allows us to scale interior reverb down when the event is triggered inside a partially open building. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Location, Meta = (ClampMin = 0.0, ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float EventEnclosure {0.f};

	/** The amount that the listener is enclosed by geometry. This value allows us to scale interior reverb down when the event is triggered inside a partially open building. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Location, Meta = (ClampMin = 0.0, ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float ListenerEnclosure {0.f};

	/** The amount of obstruction between the event and the listener. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Obstruction, meta = (ClampMin = 0.0, ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float Obstruction {0.f};

	/** Passes the structs data to a MetaSound instance on an AudioComponent. This will only work if the MetaSound implements parameters of the same name and type. */
	void SetMetaSoundParameters(UAudioComponent* AudioComponent);

	/** Constructor with default values. */
	FEventWorldData()
	{
	}
};