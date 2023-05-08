// Copyright 2023 Nino Saglia & Tim Verberne


#include "ReacousticComponent.h"
#include "ReacousticSubsystem.h"
#include "Shockworks/Shockworks.h"

// Sets default values for this component's properties
UReacousticComponent::UReacousticComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
}
// Called after the component has been fully created and registered, but before it has been initialized and is ready for use.
void UReacousticComponent::OnComponentCreated()
{
	TArray<UActorComponent*> Components;
	if(!GIsEditor) // We cannot get a pointer to the owner's static mesh component while in the editor viewport. We only do this when the game is running or simulating.
	{
		if(GetOwner())
		{
			GetOwner()->GetComponents(Components);
			if(!Components.IsEmpty())
			{
				for (UActorComponent* Component : Components)
				{
					UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component); 
					if (StaticMeshComponent->IsSimulatingPhysics() && StaticMeshComponent->BodyInstance.bNotifyRigidBodyCollision)
					{
						MeshComponent = StaticMeshComponent;
						break;
					}
				}
			}
			if(!MeshComponent)
			{
				const FString ComponentName {this->GetName()};
				UE_LOG(LogReacousticComponent, Warning, TEXT("%s was unable to find a static mesh component with physics simulation enabled in its owner. "), *ComponentName);
			}
		}
	}
	Super::OnComponentCreated();
}

// Called when the game starts
void UReacousticComponent::BeginPlay()
{
	Super::BeginPlay();
	if(const UWorld* World {GetWorld()})
	{
		if(UReacousticSubsystem* Subsystem {World->GetSubsystem<UReacousticSubsystem>()})
		{
			Subsystem->RegisterComponent(this);
		}
	}
	else
	{
		const FString ComponentName {this->GetName()};
		UE_LOG(LogReacousticComponent, Warning, TEXT("%s was unable to register itself to the Reacoustic subsystem."), *ComponentName);
	}

	// Subscribe to the OnComponentHit delegate of the target StaticMeshComponent.
	MeshComponent->OnComponentHit.AddDynamic(this, &UReacousticComponent::HandleOnComponentHit);
}

// Called when the game ends or the object is marked for destruction.
void UReacousticComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(const UWorld* World {GetWorld()})
	{
		if(UReacousticSubsystem* Subsystem {World->GetSubsystem<UReacousticSubsystem>()})
		{
			Subsystem->UnregisterComponent(this);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UReacousticComponent::HandleOnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogReacousticComponent, Warning, TEXT("Hit callback"));
}


// Called every frame
void UReacousticComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

