// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinArea.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWinConditionMet);
UCLASS()
class COOPADVENTURE_API AWinArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWinArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* WinAreaBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWinConditionMet = false;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCWinConditionMet();

	UPROPERTY(BlueprintAssignable)
	FOnWinConditionMet OnWinConditionMet;
};
