// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchpad.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class FPSGAME_API AFPSLaunchpad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchpad();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* LaunchpadBase;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* Launchpad;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	UParticleSystem* LaunchParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LaunchStrength;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LaunchPitch;

public:	

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
