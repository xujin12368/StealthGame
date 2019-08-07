// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MultiCastHandleMission_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator() ; It ; It++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
		if (PC)
		{
			PC->OnMissionComplete(InstigatorPawn, bMissionSuccess);
		}

		APawn* MyPawn = PC->GetPawn();
		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			MyPawn->DisableInput(PC);
		}
	}
}
