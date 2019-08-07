// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// 使用网络多播，让Server把信息发送给所有的Client
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastHandleMission(APawn* InstigatorPawn, bool bMissionSuccess);
};
