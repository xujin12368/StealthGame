// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MultiCastHandleMission_Implementation(APawn* Pawn, bool bMissionSuccess)
{
	// 使用FConstPawnIterator来获取World中的所有Pawn
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		Pawn = It->Get();
		if (Pawn)
		{
			Pawn->DisableInput(nullptr);
		}
	}
}
