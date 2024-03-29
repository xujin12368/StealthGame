// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
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

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

void AFPSAIGuard::SetAIState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	// 写在此处是因为SetAIState只有Server调用。
	// 为了让Server也调用客户端的复制函数，所以卸载Server调用的函数里面。以此达到同步的目的。
	OnRep_GuardState();
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

// 这个函数设定客户端复制规则。
// 其中DOREPLIFETIME这个宏是用来设定规则的。
// 此处使用的是默认规则。即复制给所有的客户端。
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}
