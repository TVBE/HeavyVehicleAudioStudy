// Copyright 2023 Nino Saglia & Tim Verberne

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShockworksColliderEventActor.generated.h"

UCLASS()
class SHOCKWORKS_API AShockworksColliderEventActor : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AShockworksColliderEventActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	/** Audio Component for playing the detonation source audio. */
	UPROPERTY(VisibleAnywhere, Category = Components, Meta = (DisplayName = "ShockworksCollider Audio Component", AllowPrivateAcces = "true"))
	UAudioComponent* SourceAudioComponent;

	/** The MetaSound to use for the SourceAudioComponent */
	UPROPERTY()
	class UMetaSoundSource* SourceMetaSound;

};
