// Copyright 2023 Nino Saglia & Tim Verberne


#include "Shockworks/ShockworksSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Shockworks/Shockworks.h"
#include "Shockworks/ShockworksDetonationEvent.h"
#include "Shockworks/ShockworksDetonationEventActor.h"
#include "Shockworks/ShockworksEventActor.h"

#if !UE_BUILD_SHIPPING
	const TAutoConsoleVariable<bool> CVarEnableDebugging(
		TEXT("shockworks.EnableDebugging"),
		true,
		TEXT("Enables or disables general debugging options for Shockworks.")
		);

	const TAutoConsoleVariable<bool> CVar3DVisualizeEvents(
		TEXT("shockworks.3DVisualizeEvents"),
		false,
		TEXT("When enabled, active events display print debugging text at their location showing basic information regarding their state.")
		);
#endif

void UShockworksSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

#if !UE_BUILD_SHIPPING
	EnableSubsystemTick = true; // Enable tick for the subsystem if this is a non-shipping build.
#endif
}

AShockworksEventActor* UShockworksSubsystem::PlayShockworksEventAtLocation(UShockworksEvent* Event, FVector Location, FRotator Rotation, UObject* Initiator)
{
	if(!Event)
	{
		UE_LOG(LogShockworksSubsystem, Warning, TEXT("PlayShockworksEventAtLocation was called without specifying an event."))
		return nullptr;
	}
	if(!Event->Validate())
	{
		return nullptr;
	}
	
	AShockworksEventActor* Actor {nullptr};
	if(Event->IsA(UShockworksDetonationEvent::StaticClass()))
	{
		Actor = GetEventActorFromPool(AShockworksDetonationEventActor::StaticClass(), Event, Initiator);
	}
	
	if(Actor)
	{
		if (Actor->GetAssignedEvent() != Event)
		{
			Actor->AssignEvent(Event);
		}
		Actor->SetActorLocationAndRotation(Location, Rotation);
		Actor->StartEventAndReleaseWhenFinished();
	}
	return Actor;
}

AShockworksEventActor* UShockworksSubsystem::GetEventActorFromPool(const TSubclassOf<AShockworksEventActor> Type, const UShockworksEvent* Event, UObject* Initiator)
{
	if(!Type)
	{
		UE_LOG(LogShockworksSubsystem, Warning, TEXT("GetEventActorFromPool was called without specifying which type of EventActor to return. "));
		return nullptr;
	}
	UClass* Class {Type.Get()};
	if(!EventActorPool.IsEmpty())
	{
		AShockworksEventActor* FoundActor {nullptr};
		for (AShockworksEventActor* Actor : EventActorPool)
		{
			if (Actor->GetClass() == Class)
			{
				if(!Event)
				{
					FoundActor = Actor;
					break;
				}
				if(Event && Actor->GetAssignedEvent() == Event)
				{
					FoundActor = Actor;
					break;
				}
				FoundActor = Actor;
			}
		}
		if(FoundActor)
		{
			const FString ActorType {FoundActor->GetActorNameOrLabel()};
			UE_LOG(LogShockworksSubsystem, Verbose,TEXT("%s was pulled from the pool."), *ActorType)
			FoundActor->Holder = Initiator;
			return FoundActor;
		}
	}
	AShockworksEventActor* Actor {Cast<AShockworksEventActor>(GetWorld()->SpawnActor(Type))};
	const FString ActorType {UKismetSystemLibrary::GetClassDisplayName(Actor->GetClass())};
	UE_LOG(LogShockworksSubsystem, Verbose,TEXT("A new instance of %s was created."), *ActorType)
	return Actor;
}

AShockworksEventActor* UShockworksSubsystem::GetEventActorFromPool(const TSubclassOf<AShockworksEventActor> Type, UObject* Initiator)
{
	return GetEventActorFromPool(Type, nullptr,Initiator);
}

void UShockworksSubsystem::StopAllEvents(const bool ForceStop)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShockworksEventActor::StaticClass(), FoundActors);
	int StoppedActorCount {0};
	if (FoundActors.Num() > 0)
	{
		for (AActor* Actor : FoundActors)
		{
			AShockworksEventActor* EventActor {Cast<AShockworksEventActor>(Actor)};
			if(EventActor != nullptr && EventActor->GetIsPlayingEvent())
			{
				EventActor->StopEvent(ForceStop);
				StoppedActorCount++;
			}
		}
	}
	UE_LOG(LogShockworksSubsystem, Log, TEXT("StopAllEvents called on ShockworksSubsystem. Stopped %d EventActors."), StoppedActorCount);
}

void UShockworksSubsystem::TransferToPool(AShockworksEventActor* EventActor)
{
	EventActorPool.Add(EventActor);
	const FString ActorType {UKismetSystemLibrary::GetDisplayName(EventActor)};
	UE_LOG(LogShockworksSubsystem, Verbose,TEXT("%s was released back to the pool."), *ActorType)
}

int UShockworksSubsystem::GetEventActorInstanceCount() const
{
	return EventActorInstances.Num();
}

int UShockworksSubsystem::GetEventActorPoolCount() const
{
	return EventActorPool.Num();
}

void UShockworksSubsystem::RegisterEventActor(AShockworksEventActor* EventActor)
{
	if(EventActor)
	{
		EventActorInstances.AddUnique(EventActor);
		// Subscribe to events.
		EventActor->OnEventStartDelegate.AddDynamic(this, &UShockworksSubsystem::HandleEventStart);
		EventActor->OnEventFinishedDelegate.AddDynamic(this, &UShockworksSubsystem::HandleEventFinished);

		const FString ActorName {EventActor->GetFName().ToString()};
		UE_LOG(LogShockworksSubsystem, Verbose, TEXT("Registered %s"), *ActorName);
	}
}

void UShockworksSubsystem::UnregisterEventActor(AShockworksEventActor* EventActor)
{
	if(EventActor)
	{
		EventActorInstances.Remove(EventActor);
		EventActorPool.Remove(EventActor);
		
		const FString ActorName {EventActor->GetFName().ToString()};
		UE_LOG(LogShockworksSubsystem, Verbose, TEXT("Unregistered %s"), *ActorName);
	}
}

void UShockworksSubsystem::HandleEventStart(const AShockworksEventActor* EventActor)
{
	return; // Implement this
}

void UShockworksSubsystem::HandleEventFinished(const AShockworksEventActor* EventActor)
{
	return; // Implement this
}

bool UShockworksSubsystem::GetStartClearance(const AShockworksEventActor* EventActor)
{
	if(EventActor)
	{
		return true;
	}
	return false;
}

#if !UE_BUILD_SHIPPING
void UShockworksSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
#endif











	

