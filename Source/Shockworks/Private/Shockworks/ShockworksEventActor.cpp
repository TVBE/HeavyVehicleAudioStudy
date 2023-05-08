// Copyright 2023 Nino Saglia & Tim Verberne.

#include "Shockworks/ShockworksEventActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ShockworksEvent.h"
#include "Shockworks/Shockworks.h"
#include "Shockworks/ShockworksSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

// Initialize static member variables.
const TArray<FVector> AShockworksEventActor::EnclosureTraceVertices = GenerateEnclosureTraceVertices(20, 5000);
const TArray<FVector2D> AShockworksEventActor::ObstructionTraceOffsets = GenerateObstructionOffsets();

// Sets default values.
AShockworksEventActor::AShockworksEventActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	// Set the root for this actor that alle other components should attach to.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(Root);
	
}

// Called after the constructor but before BeginPlay.
void AShockworksEventActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	/** It is possible to spawn an EventActor outside the scope of the ShockworksSubsystem, as the EventActor will register itself to the subsystem on construction. 
	*	This means that the EventActor can be spawned anywhere in the game, but will still be properly registered and managed by the ShockworksSubsystem.
	*	However, keep in mind that directly spawning an EventActor in the world instead of requesting an EventActor through the subsytem is not by design and should be avoided if possible. */
	if(GetWorld())
	{
		if(UShockworksSubsystem* Subsystem {GetWorld()->GetSubsystem<UShockworksSubsystem>()})
		{
			Subsystem->RegisterEventActor(this);
		}
	}
	
}


// Called when the game starts or when spawned
void AShockworksEventActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called when the actor is marked for destruction by the engine
void AShockworksEventActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(GetWorld())
	{
		if(UShockworksSubsystem* Subsystem {GetWorld()->GetSubsystem<UShockworksSubsystem>()})
		{
			Subsystem->UnregisterEventActor(this);
		}
	}
	
	// Clear the invocation list of all delegates.
	OnEventChangedDelegate.RemoveAll(this);
	OnEventStartDelegate.RemoveAll(this);
	OnEventFinishedDelegate.RemoveAll(this);
	OnEventStopDelegate.RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AShockworksEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if(Enable3DVisualisation && IsPlayingEvent)
	{
		DrawDebugTextToScreen();
	}
#endif
}

#if WITH_EDITOR
void AShockworksEventActor::DrawDebugTextToScreen()
{
	if(IsPlayingEvent)
	{
		const UWorld* World {GetWorld()};
		if(!World)
		{
			return;
		}
		AActor* Actor {this};
		const FColor Color {FColor::White};
		const float TimeActive {GetTimeActive()};
		const FString String {GetAssignedEvent()->GetFName().ToString().Append(" [%f].", TimeActive)};
		
		DrawDebugString(World, FVector(), String, Actor, Color, -1.f, false, 1.f);
	}
}
#endif

/** If the EventActor is already playing an event when this function is called, this function call will be ignored. */
bool AShockworksEventActor::StartEvent()
{
	if (!CurrentAssignedEvent->Validate() || IsPlayingEvent)
	{
		return false;
	}
	GenerateEventWorldData();
	OnEventStartDelegate.Broadcast(this);
	IsPlayingEvent = true;
	IsPendingRelease = Holder == nullptr ? true : false; // If no object is currently holding this EventActor, we will auto release the EventActor back to the pool after playing.
	EventStartTime = FPlatformTime::Seconds();
	return true;
}

void AShockworksEventActor::StopEvent(bool ForceStop)
{
	OnEventStopDelegate.Broadcast();
	IsStoppingEvent = true;
	IsPendingRelease = Holder == nullptr ? true : false; // If no object is currently holding this EventActor, we will auto release the EventActor back to the pool after playing.
}

/** A new event cannot be assigned if this EventActor is already playing an event. */
bool AShockworksEventActor::AssignEvent(UShockworksEvent* Event)
{
	if(IsPlayingEvent)
	{
		const FString EventActorName {UKismetSystemLibrary::GetDisplayName(this)};
		UE_LOG(LogShockworksEventActor, Error, TEXT("Attempted to assign a new event %s, but this EventActor is currently playing an event."), *EventActorName);
		return false;
	}
	if (Event == nullptr)
	{
		const FString EventActorName {UKismetSystemLibrary::GetDisplayName(this)};
		UE_LOG(LogShockworksEventActor, Error, TEXT("Attempted to assign NULL event to %s."), *EventActorName);
		return false;
	}
	if (!Event->Validate())
	{
		return false;
	}
	CurrentAssignedEvent = Event;
	return true;
}

void AShockworksEventActor::Release()
{
	if(IsPlayingEvent)
	{
		return;
	}

	IsPendingRelease = false;
	CurrentAssignedEvent = nullptr;
	EventWorldData = FEventWorldData();
	EventStartTime = 0.0;
	
	UShockworksSubsystem* ActorPool {GetWorld()->GetSubsystem<UShockworksSubsystem>()};
	if(ActorPool != nullptr)
	{
		ActorPool->TransferToPool(this);
	}
}

/** If the EventActor is already playing an event when this function is called, this function call will be ignored.
 *	This means that the EventActor will not be marked for release after finishing with its current event. */
void AShockworksEventActor::StartEventAndReleaseWhenFinished()
{
	if(!IsPlayingEvent)
	{
		StartEvent();
		if(IsPlayingEvent)
		{
			IsPendingRelease = true;
		}
	}
}

float AShockworksEventActor::GetTimeActive()
{
	if(IsPlayingEvent)
	{
		return FPlatformTime::Seconds() - EventStartTime;
	}
	return -1.0;
}

void AShockworksEventActor::HandleEventFinished()
{
	if(IsPendingRelease || Holder == nullptr)
	{
		Release();
	}
}

void AShockworksEventActor::HandleEventStopped()
{
	IsStoppingEvent = false;
	if(IsPendingRelease || Holder == nullptr)
	{
		Release();
	}
}

APlayerCameraManager* AShockworksEventActor::GetListener()
{
	const UWorld* World {GetWorld()};
	if (IsValid(World))
	{
		const APlayerController* PlayerController = World->GetFirstPlayerController();
		if (IsValid(PlayerController))
		{
			return PlayerController->PlayerCameraManager;
		}
	}
	UE_LOG(LogShockworksEventActor, Warning, TEXT("Failed to get a reference to the player camera manager. "))
	return nullptr;
}


/** Called when StartEvent is called on this EventActor.*/
void AShockworksEventActor::GenerateEventWorldData()
{
	const APlayerCameraManager* Listener {GetListener()};
	FEventWorldData EventData = FEventWorldData();
	const int DistanceToListener {IsValid(Listener) ? GetDistanceToListener(Listener->GetCameraLocation()) : 0};
	EventData.DistanceToListener = DistanceToListener;
	EventData.DelayToListener = IsValid(Listener) ? GetDelayToListener(DistanceToListener) : 0.f;
	EventData.EventEnclosure = GetEnclosure(this->GetActorLocation());
	EventData.ListenerEnclosure = IsValid(Listener) ? GetEnclosure(Listener->GetCameraLocation()) : 0.f;
	EventData.Obstruction = IsValid(Listener) ? GetObstruction() : 0.f;

	EventWorldData = EventData;
}

/** Calculates the distance between the actor and the listener. */
int AShockworksEventActor::GetDistanceToListener(const FVector Location)
{
	return FMath::TruncToInt32(FVector::Dist(Location, GetActorLocation()));
}

/** Calculates the distance delay between the actor and listener. Will set the delay to zero under a certain distance threshold
 *  to prevent very short delays being perceived as bad audio synchronisation by the user. */
float AShockworksEventActor::GetDelayToListener(int Distance)
{
	float Delay {0.0};
	if (Distance >= 5000)
	{
		constexpr int SpeedOfSound {343}; // Real world speed of sound is 343 meters per second.
		Delay = (Distance * 0.01) / SpeedOfSound;
	}
	return Delay;
}

/** Calculates the enclosure of geometry around the event and the listener by performing multiple collision queries.
 *  We can assume that some of the line traces will always return a hit result when the event is played on a surface,
 *  therefore we enforce a threshold of trace results before we start counting them as enclosure. */
float AShockworksEventActor::GetEnclosure(FVector Location)
{
	FIntPoint HitResultVector {PerformMultipleLinetraceFromLocation(Location, EnclosureTraceVertices)};
	if(HitResultVector.Y == 0)
	{
		return 0.f;
	}
	const int Threshold {HitResultVector.X / 3};
	const int ClampedDifference {FMath::Max(0, (HitResultVector.X - Threshold))};
	return ClampedDifference / HitResultVector.Y;
	
}

TArray<FVector> AShockworksEventActor::GenerateEnclosureTraceVertices(const int VerticesAmount, float Radius)
{
	const float Phi  {(1.0f + FMath::Sqrt(5.0f)) / 2.0f};
	TArray<FVector> Array;
		for (int i {0}; i < VerticesAmount; i++)
		{
			float Theta {(2 * i * PI) / 12};
			float X {Radius * FMath::Cos(Theta) * FMath::Sqrt(1 - Phi * Phi / 5)};
			float Y {Radius * FMath::Sin(Theta) * FMath::Sqrt(1 - Phi * Phi / 5)};
			float Z {Radius * Phi / 5};
			Array.Add(FVector(X, Y, Z));
		}
	return Array;
}

/** Calculates the obstruction of geometry between the event and the listener by performing multiple line traces.
 *  We can assume that some of the line traces will always return a hit result when the listener is located on a surface,
 *  therefore we enforce a threshold of trace results before we start counting them as obstruction. */
float AShockworksEventActor::GetObstruction()
{
	float Obstruction {0.0};
	if (IsValid(GetWorld()->GetFirstPlayerController()->PlayerCameraManager))
	{
		int HitCount {0};
		const FRotator Direction {UKismetMathLibrary::FindLookAtRotation
			(this->GetActorLocation(), GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation())};
		
		const TArray<FVector2D> Offsets {ObstructionTraceOffsets};
		for(int i = 0; i < Offsets.Num(); i++)
		{
			constexpr int Multiplier {150};
			const FVector Start
			{Direction.RotateVector(FVector(0, Offsets[i].X * Multiplier, Offsets[i].Y * Multiplier)) + this->GetActorLocation()};
			const FVector End
			{Direction.RotateVector(FVector(0,Offsets[i].X * -Multiplier, Offsets[i].Y * Multiplier)) + GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation()};

			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
			{
				HitCount++;
			}
		}
		Obstruction = FMath::Clamp(HitCount - 3, 0, Offsets.Num()) * (1 / Offsets.Num());
	}
	return Obstruction;
}

/** Generates an array of vector offsets to be used by the obstruction traces. */
TArray<FVector2D> AShockworksEventActor::GenerateObstructionOffsets()
{
	TArray<FVector2D> Array;
	for(int i {0}; i < 3; i++)
	{
		for (int j {0}; j < 3; j++)
		{
			Array.Add(FVector2D(j-1, i-1));	
		}
	}
	return Array;
}

FIntPoint AShockworksEventActor::PerformMultipleLinetraceFromLocation(const FVector Location, const TArray<FVector> TraceVertices)
{
	if(!TraceVertices.IsEmpty())
	{
		return FIntPoint(0, 0);
	}
	int HitResultCount = 0;
	for (const FVector Vector : TraceVertices)
	{
		FHitResult OutHitResult;
		if (GetWorld()->LineTraceSingleByChannel(OutHitResult, Location, Vector + Location, ECC_Visibility))
		{
			HitResultCount++;
		}
	}
	return FIntPoint(HitResultCount, TraceVertices.Num());
}


#if WITH_EDITOR
/** void AShockworksEventActor::Set3DVisualizeEnabled(const bool Value)
{
	Enable3DVisualisation = Value;
	SetActorTickEnabled(Value);
}
*/
#endif















