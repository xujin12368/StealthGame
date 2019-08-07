// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "FPSAIGuard.h"

void AFPSAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardData == nullptr || BehaviorTree == nullptr)
	{
		return;
	}

	UseBlackboard(BlackboardData, BlackboardComp);

	bool bRunSuccess = RunBehaviorTree(BehaviorTree);

	UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Run %d"), bRunSuccess);

	AFPSAIGuard* MyGuard = Cast<AFPSAIGuard>(GetPawn());
	if (MyGuard)
	{
		TargetActors = MyGuard->GetTargetActors();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIGuard Cast Failed."));
	}
}

void AFPSAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActors.Num() == 0)
	{
		return;
	}

	TargetNum %= TargetActors.Num();

	GetBlackboardComponent()->SetValueAsObject(TargetKey, TargetActors[TargetNum++]);

}
