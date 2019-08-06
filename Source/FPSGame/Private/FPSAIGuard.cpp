// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Comp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSight);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearing);

	GuardState = EAIState::Idle;

}

TArray<AActor*> AFPSAIGuard::GetTargetActors() const
{
	return TargetActors;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginRotation = GetActorRotation();
}

void AFPSAIGuard::SetAIState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	OnStateChanged(GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::OnSight(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.f, 2, FColor::Red, false, 2.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	GM->CompleteGame(Pawn, false);

	SetAIState(EAIState::Alert);
}

void AFPSAIGuard::OnHearing(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alert)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.f, 2, FColor::Blue, false, 2.f);

	FVector NewDirection = Location - GetActorLocation();
	FRotator NewRotation = NewDirection.Rotation();
	NewRotation.Pitch = 0;
	NewRotation.Roll = 0;

	SetActorRotation(NewRotation);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AFPSAIGuard::HandleTimer, 3.f, false, 3.f);

	SetAIState(EAIState::Suspection);
}

void AFPSAIGuard::HandleTimer()
{
	if (GuardState == EAIState::Alert)
	{
		return;
	}

	SetActorRotation(OriginRotation);

	SetAIState(EAIState::Idle);
}
