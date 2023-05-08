// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shockworks/ShockworksDetonationSoundset.h"
#include "Shockworks/ShockworksEvent.h"
#include "Shockworks/ScatterPattern.h"
#include "ShockworksDetonationEvent.generated.h"



/** Data asset for describing the behavior of a detonation. */
UCLASS(BlueprintType)
class SHOCKWORKS_API UShockworksDetonationEvent : public UShockworksEvent
{
	GENERATED_BODY()

public:
	
	/** The detonation soundset to use for this detonation event. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Soundset, Meta = (DisplayName = "Soundset"))
	UShockworksDetonationSoundset* Soundset;

	/** The events volume. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = General, Meta = (DisplayName = "Volume", ClampMin = "0.25", ClampMax = "2.0", UIMin = "0.25", UIMax = "2.0"))
	float Volume {1.f};

	/** The events surface impact if the event is facing a surface. Higher values simulate high velocity shells directly impacting a surface. This will introduce surface dependent impact audio. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = General, Meta = (DisplayName = "Surface Impact", ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int SurfaceImpact {25};
	
	/** When set to true, this detonation event will generate shrapnel effects around its source. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Shrapnel, Meta = (DisplayName = "Enable Shrapnel"))
	bool EnableShrapnel {false};

	/** The amount of shrapnel that this explosion generates. This setting will incur a slight performance cost at higher values. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Shrapnel, Meta = (DisplayName = "Shrapnel Amount", EditCondition = "EnableShrapnel", EditConditionHides, ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int ShrapnelAmount {25};

	/** The intensity of shrapnel impacting the surrounding area. Higher values will simulate larger fragments impacting at higher velocities. */ // Currently Unused!
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Shrapnel, Meta = (DisplayName = "Shrapnel Intensity", EditCondition = "EnableShrapnel", EditConditionHides, ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int ShrapnelIntensity {50};
	
	/** The radius for shrapnel to impact the surrounding terrain. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Shrapnel, Meta = (DisplayName = "Shrapnel Radius", EditCondition = "EnableShrapnel", EditConditionHides, ClampMin = "0", UIMin = "0"))
	int ShrapnelRadius {1500};

	/** The scatter pattern for shrapnel in relation to the detonation's direction. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Shrapnel, Meta = (DisplayName = "Scatter Pattern", EditCondition = "EnableShrapnel", EditConditionHides))
	EScatterPattern ShrapnelScatterPattern;

	/** When set to true, this detonation event will generate debris effects around its source.*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debris, Meta = (DisplayName = "Enable Debris"))
	bool EnableDebris {false};
	
	/** The amount of debris that this explosion generates. This setting will incur a slight performance cost at higher values. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debris, Meta = (DisplayName = "Debris Amount", EditCondition = "EnableDebris", EditConditionHides, ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int DebrisAmount {25};

	/** The intensity of debris impacting the surrounding area. Higher values will simulate larger fragments impacting at higher velocities. */ // Currently Unused!
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debris, Meta = (DisplayName = "Debris Intensity", EditCondition = "EnableDebris", EditConditionHides, ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int DebrisIntensity {50};

	/** The radius for debris to impact the surrounding terrain in units. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debris, Meta = (DisplayName = "Debris Radius", EditCondition = "EnableDebris", EditConditionHides, ClampMin = "0", UIMin = "0"))
	int DebrisRadius {1500};

	/** The scatter pattern for shrapnel in relation to the detonation's direction. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debris, Meta = (DisplayName = "Scatter Pattern", EditCondition = "EnableDebris", EditConditionHides))
	EScatterPattern DebrisScatterPattern;
	
	/** Scalar for the duration of debris to hit the surrounding terrain. Higher values simulate a more upward dispersion of debris. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Debris, Meta = (DisplayName = "Debris Duration Scalar", EditCondition = "EnableDebris", EditConditionHides, ClampMin = "0.5", UIMin = "0.5", ClampMax = "4.0", UIMax = "4.0"))
	float DebrisDurationScalar {1.f};

	// TEMP ------------------------------------------------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = TEMPORARY, Meta = (DisplayName = "Reverberation Soundset"))
	class UShockworksReverberationSoundset* ReverberationSoundset;
	
	// TEMP ------------------------------------------------------------------------------------------------------------
	
	/** Checks whether this event is valid and can be played. */
	bool Validate() override;
	
};
