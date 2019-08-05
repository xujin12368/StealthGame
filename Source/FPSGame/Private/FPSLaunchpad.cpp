// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchpad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSLaunchpad::AFPSLaunchpad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LaunchpadBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launchpad Base"));
	RootComponent = LaunchpadBase;
	LaunchpadBase->SetWorldScale3D(FVector(.5f, .5f, .2f));

	Launchpad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launchpad"));
	Launchpad->SetupAttachment(RootComponent);
	Launchpad->SetWorldScale3D(FVector(2.5f, 2.5f, .1f));
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchpad::HandleOverlap);

}

void AFPSLaunchpad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitch;
	if (OtherActor && OtherActor->ActorHasTag(FName("Player")))
	{
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->LaunchCharacter(LaunchDirection.Vector()*LaunchStrength, false, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Character Cast Failed.No MyCharacter."));
		}
	}
	else
	{
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulse(LaunchDirection.Vector()*LaunchStrength, NAME_None, true);
			UE_LOG(LogTemp, Warning, TEXT("Add Force Successfully. %s"),*OtherComp->GetComponentVelocity().ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Add Force To Comp Failed."));
		}
	}

	if (LaunchParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchParticle, GetActorLocation());
	}
}

