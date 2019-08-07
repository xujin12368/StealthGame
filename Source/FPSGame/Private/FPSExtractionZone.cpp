// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	ExtractionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Extraction Comp"));
	ExtractionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExtractionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExtractionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ExtractionComp->SetBoxExtent(FVector(200.f));
	ExtractionComp->SetHiddenInGame(false);

	ExtractionComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Comp"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->DecalSize = FVector(200.f);

}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AFPSCharacter* MyChracter = Cast<AFPSCharacter>(OtherActor);
		if (MyChracter)
		{
			if (MyChracter->bIsCarryingObj)
			{
				AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
				if (GM) // 此处检测是因为GameMode在Client上是nullptr，如果不检测会引起崩溃
				{
					GM->CompleteGame(MyChracter, true);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("You need to carry on objective."));
				UGameplayStatics::PlaySound2D(this, MissionSound);
			}
		}
	}
}

