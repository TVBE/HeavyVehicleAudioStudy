// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "ShockworksEventActor.h"
#include "Subsystems/WorldSubsystem.h"
#include "UnrealEd.h"
#include "ShockworksSubsystem.generated.h"
/** Shockworks subsystem that is instantiated at the construction of an UWorld. */
UCLASS()
class SHOCKWORKS_API UShockworksSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#if !UE_BUILD_SHIPPING
	/** When enabled, this subsystem is allowed to tick. */
	bool EnableSubsystemTick {false};
#endif
	
private:
	/** Actor pool for ShockworksEventActors. */
	UPROPERTY()
	TArray<class AShockworksEventActor*> EventActorPool;

	/** All ShockworksEventActors currently in the world or in the pool.*/
	UPROPERTY()
	TArray<class AShockworksEventActor*> EventActorInstances;

public:
	void OnWorldBeginPlay(UWorld& InWorld) override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsTickable() const override { return EnableSubsystemTick; }

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UShockworksSubsystem, STATGROUP_Tickables);
	}
	
	/**
	 *	Notifies the subsystem that an EventActor has been instanced in the world.
	 *	@param EventActor The EventActor that notifies the subsystem.
	 */
	UFUNCTION()
	void RegisterEventActor(AShockworksEventActor* EventActor);

	/**
	 *	Notifies the subsystem that an EventActor has been destroyed in the world.
	 *	@param EventActor The EventActor that notifies the subsystem.
	 */
	UFUNCTION()
	void UnregisterEventActor(AShockworksEventActor* EventActor);

	/**
	 *	Requests clearance to play an event from the ShockworksSubsystem.
	 * @param EventActor The EventActor that requests clearance to play an event.
	 * @return Whether the EventActor can play its event.
	 */
	UFUNCTION()
	bool GetStartClearance(const AShockworksEventActor* EventActor);

	/** Handles the OnEventPlayDelegate of an EventActor. */
	UFUNCTION()
	void HandleEventStart(const AShockworksEventActor* EventActor);

	/** Handles the OnEventFinishedDelegate of an EventActor. */
	UFUNCTION()
	void HandleEventFinished(const AShockworksEventActor* EventActor);
	
	/** Transfers an EventActor back to the pool. */
	UFUNCTION()
	void TransferToPool(AShockworksEventActor* EventActor);
	
	/**
	 *	Play a Shockworks Event at a location.
	 *	@param Event The Shockworks Event to play.
	 *	@param Location The location to play the event at.
	 *	@param Rotation The rotation to play the event at. All events treat their forward X axis as the point of impact by default, so rotate them accordingly.
	 *	@param Initiator The object that will hold the EventActor from the pool. It is recommended to leave this empty unless you explicitly want to reuse the EventActor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = Default, Meta = (DisplayName = "Play Shockworks Event At Location"))
	AShockworksEventActor* PlayShockworksEventAtLocation(class UShockworksEvent* Event, FVector Location, FRotator Rotation, UObject* Initiator);
	
	/**
	 *	Returns an EventActor of the given type from the pool.
	 *  @param Type The type of EventActor to spawn.
	 *  @param Event The event that the EventActor should have. This will search through the pool and return the first instance of an EventActor with this event if there is any.
	 *  @param Initiator The object that will hold the EventActor from the pool.
	 *  Not passing an object pointer to the EventActor when obtaining one from the pool will always cause the EventActor to auto release itself after playing.
	 */
	UFUNCTION(BlueprintCallable, Category = Default, Meta = (DisplayName = "Get Event Actor"))
	AShockworksEventActor* GetEventActorFromPool(const TSubclassOf<AShockworksEventActor> Type, const UShockworksEvent* Event, UObject* Initiator); 
	AShockworksEventActor* GetEventActorFromPool(const TSubclassOf<AShockworksEventActor> Type, UObject* Initiator); 
	
	
	/** Stop all EventActors currently playing an event.
	 *	@param ForceStop forces all events to stop immediately instead of fading out.
	 */
	UFUNCTION(BlueprintCallable, Category = Default, Meta = (DisplayName = "Stop All Events"))
	void StopAllEvents(const bool ForceStop);
	
	/** Returns the amount of EventActor instances in the world. This includes EventActors in the pool. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Get Event Actor Instance Count"))
	int GetEventActorInstanceCount() const;

	/** Returns the amount of EventActor instances in the pool. */
	UFUNCTION(BlueprintPure, Category = Default, Meta = (DisplayName = "Get Event Actor Pool Count"))
	int GetEventActorPoolCount() const;
	
};
