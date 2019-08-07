// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// 因为PlayerController是在Server和Client都会实例化的
	// 所以把这个声明在PC中，再给GameState调用，即可实现GameOver的Widget
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerController")
	void OnMissionComplete(APawn* InstigatorPawn, bool bMissionSuccess);
};
