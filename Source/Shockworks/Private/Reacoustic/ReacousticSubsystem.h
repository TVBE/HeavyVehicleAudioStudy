// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ReacousticSubsystem.generated.h"

/** 
 */
UCLASS()
class UReacousticSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	/** Array of pointers to all currently active ReacousticComponents. */
	TArray<class UReacousticComponent*> ReacousticComponents;

	/** Pool of AudioComponents to be used for Reacoustic hit events. */
	TArray<class UAudioComponent*> AudioComponentPool;
	
public:
	/** Registers an component to the Reacoustic subsystem. This function is called by a component OnConstruct
	 *	@Component The component to register.
	 */
	void RegisterComponent(class UReacousticComponent* Component);

	/** Unregisters an component to the Reacoustic subsystem. This function is called by a component at EndPlay
	 *	@Component The component to register.
	 */
	void UnregisterComponent(UReacousticComponent* Component);
	
	/** Tries to add a Reacoustic component to an actor. A Reacoustic component will be added to the physics actor if conditions are met.
	 *	@Actor The actor to add the component to.
	 */
	UFUNCTION(BlueprintCallable, Category = Reacoustic, Meta = (DisplayName = "Attach Reacoustic Component To Actor"))
	void AddReacousticComponentToActor(AActor* Actor);

	/** Checks whether an actor meets the conditions to be used by Reacoustic.
	 *	For this, an actor must have IsSimulatingPhysics and a StaticMeshComponent with bNotifyRigidBodyCollision set to true.
	 *	@Actor The actor to check the condition for.
	 *	@Return Whether the actor is compatible with Reacoustic or not.
	 */
	UFUNCTION(BlueprintPure, Category = Reacoustic, Meta = (DisplayName = "Is Reacoustic Compatible"))
	bool IsReacousticCompatible(AActor* Actor);

private:
	/** Returns all actors in the level of a given class that have Physics and GenerateHitEvents enabled. This is a slow operation, and should not be called often during runtime.
	 *	@ClassType Which class to look for in the world.
	 *	@return An array of actor pointers that potentially meet the conditions to be registered by Reacoustic.
	 */
	UFUNCTION()
	TArray<AActor*> GetCompatibleActorsOfClass(UClass* ClassType);
};


	
