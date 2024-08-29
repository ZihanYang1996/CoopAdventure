// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

class UTransporter;
class UArrowComponent;

UCLASS()
class COOPADVENTURE_API AMovableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* Point1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* Point2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTransporter* Transporter;
};
