// Fill out your copyright notice in the Description page of Project Settings.

#include "Shockworks/ShockworksDetonationEvent.h"
#include "Shockworks/Shockworks.h"
#include "Kismet/KismetSystemLibrary.h"



bool UShockworksDetonationEvent::Validate()
{
	if(!Soundset)
	{
		const FString Asset {UKismetSystemLibrary::GetDisplayName(this)};
		UE_LOG(LogShockworks, Warning, TEXT("Event %s has no soundset and cannot be played."), *Asset);
		return false;
	}
	if(!Soundset->ValidateSoundset())
	{
		return false;
	}
	if(Volume == 0.0)
	{
		const FString Asset {UKismetSystemLibrary::GetDisplayName(this)};
		UE_LOG(LogShockworks, Warning, TEXT("Event %s contains invalid data and cannot be played."), *Asset);
		return false;
	}
	return true;
}


