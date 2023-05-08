// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"

/** Enumeration for scatter patterns of debris and shrapnel fragments. */
UENUM(BlueprintType)
enum class EScatterPattern : uint8
{
	/** The Uniform scatter pattern causes shrapnel and debris fragments to scatter in all directions. This is useful for simulating static detonations. */
	Uniform				UMETA(DisplayName = "Uniform"),
	/** The Upward scatter pattern causes shrapnel and debris fragments to scatter backwards in relation to the event's forward vector. This is useful for simulating high velocity shells. */
	Upward				UMETA(DisplayName = "Upward"),
	/** The Sideways scatter pattern causes shrapnel and debris fragments to scatter sidewards in relation to the event's forward vector. This is useful for simulating air-burst munitions. */
	Sideways			UMETA(DisplayName = "Sideways"),
	/** The forwards scatter pattern causes shrapnel and debris fragments to scatter in the direction of the event's forward vector. This is useful for simulating flechette type munitions.
	 *	This scatter pattern will only work for event's that do not directly face a surface. Realistically speaking, this scatter pattern is unsuitable for debris simulation. */
	Forward				UMETA(DisplayName = "Forward"),
};
