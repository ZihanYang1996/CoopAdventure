// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COOPADVENTURE_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTransporter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FVector StartPoint;
	FVector EndPoint;

	bool bArePointsSet = false;

	UPROPERTY(EditAnywhere)
	float MoveTime = 1.0f;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;

	UPROPERTY(VisibleAnywhere)
	int ActivatedTriggerCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAreAllTriggerActorsActivated;

	UFUNCTION(BlueprintCallable)
	void SetPoints(const FVector& Start, const FVector& End);

	UPROPERTY(EditAnywhere)
	bool bIsOwnerTriggerActor = false;

private:
	UFUNCTION()
	void OnTriggerActivated();

	UFUNCTION()
	void OnTriggerDeactivated();
};
