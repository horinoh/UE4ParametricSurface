// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ParametricSurfaceActor.generated.h"

UCLASS()
class UE4PARAMETRICSURFACE_API AParametricSurfaceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParametricSurfaceActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ParametricSurface)
	class UParametricSurfaceComponent* ParametricSurfaceComp;
};
