// Copyright 2023 Nino Saglia & Tim Verberne


#include "Deprecated/ShockworksColliderEventActor.h"

// Sets default values
AShockworksColliderEventActor::AShockworksColliderEventActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShockworksColliderEventActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShockworksColliderEventActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

