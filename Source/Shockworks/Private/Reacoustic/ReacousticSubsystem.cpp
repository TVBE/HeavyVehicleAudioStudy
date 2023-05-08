// Copyright 2023 Nino Saglia & Tim Verberne


#include "ReacousticSubsystem.h"
#include "ReacousticComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Shockworks/Shockworks.h"

void UReacousticSubsystem::RegisterComponent(UReacousticComponent* Component)
{
	ReacousticComponents.AddUnique(Component);
}

void UReacousticSubsystem::UnregisterComponent(UReacousticComponent* Component)
{
	ReacousticComponents.Remove(Component);
}

bool UReacousticSubsystem::IsReacousticCompatible(AActor* Actor)
{
	if (Actor->GetRootComponent() && Actor->GetRootComponent()->IsSimulatingPhysics())
	{
		TArray<UStaticMeshComponent*> Components;
		Actor->GetComponents<UStaticMeshComponent>(Components);
		if(!Components.IsEmpty())
		{
			for(UStaticMeshComponent* Component : Components)
			{
				if(Component->BodyInstance.bNotifyRigidBodyCollision)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void UReacousticSubsystem::AddReacousticComponentToActor(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogReacousticComponent, Warning, TEXT("AddReacousticComponentToActor was called without passing an actor pointer."))
		return;
	}
	if (Actor->GetComponentByClass(UReacousticComponent::StaticClass()))
	{
		const FString ActorName {Actor->GetActorNameOrLabel()};
		UE_LOG(LogReacousticSubsystem, Verbose, TEXT("Skipped adding a Reacoustic component to %s because this actor already has a Reacoustic component attached to it."), *ActorName);
	}
	Actor->AddComponentByClass(UReacousticComponent::StaticClass(), false, FTransform(), true);
}


TArray<AActor*> UReacousticSubsystem::GetCompatibleActorsOfClass(UClass* ClassType)
{
	TArray<AActor*> Array;
	const UWorld* World {GetWorld()};
	if (!World || !ClassType)
	{
		return Array;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ClassType, FoundActors);
	const int TotalCount {FoundActors.Num()};
	int CompatibleCount {0};
	for (AActor* Actor : FoundActors)
	{
		if(IsReacousticCompatible(Actor))
		{
			Array.Add(Actor);
			CompatibleCount++;
		}
	}
	const FString Name {ClassType->GetName()};
	UE_LOG(LogReacousticSubsystem, Verbose, TEXT("Found %d actors of type %s out of which %d can be used by the Reacoustic system."), TotalCount, *Name, CompatibleCount);
	return Array;
}


