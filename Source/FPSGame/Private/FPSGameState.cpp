// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MultiCastHandleMission_Implementation(APawn* Pawn, bool bMissionSuccess)
{
	// ʹ��FConstPawnIterator����ȡWorld�е�����Pawn
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		Pawn = It->Get();
		if (Pawn)
		{
			Pawn->DisableInput(nullptr);
		}
	}
}
