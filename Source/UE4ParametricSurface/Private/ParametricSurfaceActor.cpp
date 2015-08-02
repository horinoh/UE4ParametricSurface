// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4ParametricSurface.h"
#include "ParametricSurfaceActor.h"

#include "ParametricSurfaceComponent.h"

// Sets default values
AParametricSurfaceActor::AParametricSurfaceActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParametricSurfaceComp = ObjectInitializer.CreateDefaultSubobject<UParametricSurfaceComponent>(this, TEXT("ParametricSurface"));
}

// Called when the game starts or when spawned
void AParametricSurfaceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParametricSurfaceActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

