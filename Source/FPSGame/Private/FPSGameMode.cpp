// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteGame(APawn * InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn == nullptr)
	{
		return;
	}

	InstigatorPawn->DisableInput(nullptr);

	TArray<AActor*> OutAcotors;
	UGameplayStatics::GetAllActorsOfClass(this, SpectatingView, OutAcotors);

	if (OutAcotors.Num() > 0)
	{
		AActor* SpectatingActor = OutAcotors[0];

		APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
		if (PC)
		{
			PC->SetViewTargetWithBlend(SpectatingActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No player controller"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spectating Actor is missing.Cannot switch tartget view."));
	}

	OnMissionComplete(InstigatorPawn, bMissionSuccess);

}
