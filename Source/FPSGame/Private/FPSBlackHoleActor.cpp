// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHoleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody,ECollisionResponse::ECR_Overlap);

	KillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KillSphere"));
	KillSphere->SetupAttachment(MeshComp);
	KillSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHoleActor::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AFPSBlackHoleActor::BeginPlay()
{
	Super::BeginPlay();


}

void AFPSBlackHoleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SphereComp->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.Num() != 0)
	{
		for (auto& OverlappingActor : OverlappingActors)
		{
			auto OverlappingMeshComp = Cast<UStaticMeshComponent>(OverlappingActor->GetRootComponent());
			if (OverlappingMeshComp)
			{
				OverlappingMeshComp->AddRadialForce(GetActorLocation(), MyRadius, MyStrength, ERadialImpulseFalloff::RIF_Linear, true);
			}
		}
	}

}

