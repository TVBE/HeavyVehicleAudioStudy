// Copyright 2023 Nino Saglia & Tim Verberne.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shockworks/EventWorldData.h"
#include "ShockworksEventActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEventChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventStart, const AShockworksEventActor*, EventActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventFinish, const AShockworksEventActor*, EventActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEventStop);


/** Abstract superclass that all Shockworks event actor types derive from. Contains base logic for triggering events.
 *	Handles object pool release when finished. */
UCLASS(Abstract, NotBlueprintable, ClassGroup=(Shockworks))
class SHOCKWORKS_API AShockworksEventActor : public AActor
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
private:
	/** When enabled, this EventActor will print text to the screen at its location displaying realtime debugging info. */
	UPROPERTY()
	bool Enable3DVisualisation {false};
#endif
	
public:
	/** Signature for the FOnEventChanged event delegate for other objects to subscribe to. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Event Changed"))
	FOnEventChange OnEventChangedDelegate;
	
	/** Signature for the FOnEventPlay event delegate for other objects to subscribe to. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Event Play"))
	FOnEventStart OnEventStartDelegate;

	/** Signature for the FOnEventStop event delegate for other objects to subscribe to. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Event Stop"))
	FOnEventStop OnEventStopDelegate;

	/** Signature for the FOnEventStop event delegate for other objects to subscribe to. */
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "On Event Finished"))
	FOnEventFinish OnEventFinishedDelegate;

public:
	/** Object that is currently holding this EventActor from the pool. If this is a nullptr the EventActor will auto release itself back to the pool after playing to prevent
	 *	orphaned instances from lingering around. */
	UPROPERTY(BlueprintGetter = GetHolder, Category = Actor, Meta = (DisplayName = "Holder"))
	UObject* Holder;

private:
	/** SceneRoot Component for this EventActor.*/
	UPROPERTY()
	USceneComponent* Root;

	/** The Shockworks Event assigned to this event actor. */
	UPROPERTY(BlueprintGetter = GetAssignedEvent, Category = Event, Meta = (DisplayName = "Event"))
	class UShockworksEvent* CurrentAssignedEvent;

	/** Struct containing information about the the actors location and relation to the listener. */
	UPROPERTY(BlueprintGetter = GetEventWorldData, Category = Event, Meta = (DisplayName = "Event World Data"))
	FEventWorldData EventWorldData;
	
	/** If set to true, this EventActor is currently playing an event. */
	UPROPERTY(BlueprintGetter = GetIsPlayingEvent, Category = Actor, Meta = (DisplayName = "Is Playing Event"))
	bool IsPlayingEvent {false};

	/** If set to true, this EventActor will automatically release itself back to the pool when finished. */
	UPROPERTY(BlueprintGetter = GetIsPendingRelease, Category = Actor, Meta = (DisplayName = "Is Pending Release"))
	bool IsPendingRelease {false};

	/** If set to true, this EventActor is currently in the process of stopping its event. */
	UPROPERTY()
	bool IsStoppingEvent {false};

	/** The system time when this EventActor last started an event. */
	UPROPERTY()
	float EventStartTime;

	/** Array of vectors to be used for the enclosure calculation. */
	static const TArray<FVector> EnclosureTraceVertices;

	/** Array of vector offsets to be used for the obstruction calculation. */
	static const TArray<FVector2D> ObstructionTraceOffsets;

public:	
	/** Sets default values for this actor's properties. */
	AShockworksEventActor();

#if WITH_EDITOR
private:
	/** Draws information about the event to the screen. */
	UFUNCTION()
	void DrawDebugTextToScreen();

#endif
	
public:
	/** called after construction. */
	virtual void OnConstruction(const FTransform& Transform) override;
	
	/** Called before destruction. */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/** Called every frame. */
	virtual void Tick(float DeltaTime) override;

	/** Starts the event assigned to this actor.
	 *	@return Whether the event was successfully started. */
	UFUNCTION(BlueprintCallable, Category = Event, Meta = (DisplayName = "Play"))
	virtual bool StartEvent();

	/** Starts the event assigned to this EventActor and releases the EventActor back to the pool when finished. */
	UFUNCTION(BlueprintCallable, Category = Event, Meta = (DisplayName = "Play And Release When Finished"))
	void StartEventAndReleaseWhenFinished();

	/** Stops the event that this actor is currently playing. Will be ignored if the event actor is not currently playing.
	 *	@param ForceStop Forces the event to stop immediately without applying a fade out.
	 */
	UFUNCTION(BlueprintCallable, Category = Event, BlueprintCosmetic, Meta = (DisplayName = "Stop Event"))
	void StopEvent(bool ForceStop);

	/** Releases the object back to the pool. This will be ignored if the EventActor is currently playing an event. */
	UFUNCTION(BlueprintCallable, Category = Actor, Meta = (DisplayName = "Release"))
	void Release();

	/** Assigns an event to this EventActor. This will be ignored if the EventActor is already playing an event.
	 *	@param Event The event to assign to the EventActor.
	 *	@return Whether a new event was successfully assigned.
	 */
	UFUNCTION(BlueprintCallable, Category = Event, Meta = (DisplayName = "Assign Event"))
	virtual bool AssignEvent(UShockworksEvent* Event);
	
	/** Returns the event currently assigned to this EventActor. */
	UFUNCTION(Blueprintgetter, Category = Event, Meta = (DisplayName = "Get Assigned Event"))
	FORCEINLINE UShockworksEvent* GetAssignedEvent() const {return CurrentAssignedEvent; }
	
	/** Returns an FEventWorldData struct containing contextual information about the actors location in the world.
	 *  Is normally updated every time a new event is posted on this actor. */
	UFUNCTION(BlueprintGetter, Category = Event, Meta = (DisplayName = "Get Event WorldData"))
	FORCEINLINE FEventWorldData GetEventWorldData() const {return EventWorldData; }

	/** Returns the object that is currently holding this EventActor from the pool. */
	UFUNCTION(BlueprintGetter, Category = Actor, Meta = (DisplayName = "Get Holder"))
	FORCEINLINE UObject* GetHolder() const {return Holder; }
	
	/** Returns whether the event actor is currently playing an event. */
	UFUNCTION(BlueprintGetter, Category = Actor, Meta = (DisplayName = "Is Playing Event"))
	FORCEINLINE bool GetIsPlayingEvent() const {return IsPlayingEvent; }

	/** Returns whether the event actor is pending release. If this is the case, the EventActor will release itself
	 *	back to the pool after finishing the event it is currently playing. */
	UFUNCTION(BlueprintGetter, Category = Actor, Meta = (DisplayName = "Is Pending Release"))
	FORCEINLINE bool GetIsPendingRelease() const {return IsPendingRelease; }

	/** Returns the time since this actor started playing an event. Returns -1 if the actor is not currently playing an event.
	 *	@return Time active in seconds.
	 */
	UFUNCTION(BlueprintPure, Category = Event, Meta = (DisplayName = "Time Active"))
	float GetTimeActive();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when the event is finished. */
	virtual void HandleEventFinished();

	/** Called when the event is stopped. */
	virtual void HandleEventStopped();

private:
	/** Returns the listener camera from the player. */
	APlayerCameraManager* GetListener();
	
	/** Returns the distance between the event actor and the listener.
	 *	@Location The location of the listener
	 *	@return The distance to the listener in unreal units.
	 */
	int GetDistanceToListener(const FVector Location);

	/** Returns the delay for the event to be audible to the listener.
	 *	@Distance The distance to the listener in unreal units.
	 *	@return The delay to the listener in seconds.
	 */
	float GetDelayToListener(const int Distance);

	/** Returns the enclosure amount for the event actor.
	 *	@Location The location to check for enclosure.
	 *	@return A float value ranging from 0 to 1, with 0 being non enclosed, and 1 being fully enclosed.
	 */
	float GetEnclosure(FVector Location);

	/** Generates the polyhedron for the enclosure calculation.
	 *	@VerticesAmount The amount of vertices the shape should have. 
	 *	@Radius The radius of the polyhedron.
	 *	@return An array of vectors arranged in a polyhedron.
	 */
	static TArray<FVector> GenerateEnclosureTraceVertices(const int VerticesAmount, float Radius);
	
	/** Returns the obstruction amount for the event actor in relation to the listener.
	 *	@return A float value ranging from 0 to 1, with 0 being unobstructed, and 1 being fully obstructed.
	 */
	float GetObstruction();

	/** Returns an array of vectors to be used for the obstruction calculation. */
	static TArray<FVector2D> GenerateObstructionOffsets();
	
	/** Generates a new FEventWorldData struct for the actors current location and relation to the listener. */
	void GenerateEventWorldData();

	/** Takes an array of FVectors, adds each vector to an Actor's location, and performs a line trace from the Actor to the vector's location.
	 *	@Location The location to perform the line traces from.
	 *	@TraceVertices The trace vectors to trace to.
	 *	@return the amount of hits returned out of the total length of the TraceVertices argument.
	 */
	FIntPoint PerformMultipleLinetraceFromLocation(const FVector Location, const TArray<FVector> TraceVertices);
};

