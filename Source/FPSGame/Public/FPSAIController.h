// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIController.generated.h"

class UBlackboardComponent;
class UBlackboardData;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardData* BlackboardData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	FName TargetKey;

	UBlackboardComponent* BlackboardComp;

	TArray<AActor*> TargetActors;

	int32 TargetNum = 0;

public:
	virtual void Tick(float DeltaTime) override;
};
