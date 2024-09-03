// Fill out your copyright notice in the Description page of Project Settings.


#include "WinArea.h"

#include "CoopAdventureCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AWinArea::AWinArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WinAreaBox"));
	SetRootComponent(WinAreaBox);
}

// Called when the game starts or when spawned
void AWinArea::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (!bIsWinConditionMet)
		{
			TArray<AActor*> OverlappingActors;
			WinAreaBox->GetOverlappingActors(OverlappingActors, ACoopAdventureCharacter::StaticClass());

			bIsWinConditionMet = (OverlappingActors.Num() == 2);
			if (bIsWinConditionMet)
			{
				UE_LOG(LogTemp, Warning, TEXT("Win condition met"));
			}
		}
	}
}
