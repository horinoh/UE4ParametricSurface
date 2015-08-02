// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/PrimitiveComponent.h"
#include "ParametricSurfaceComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Rendering, EditinlineNew, Meta = (BlueprintSpawnableComponent))
class UE4PARAMETRICSURFACE_API UParametricSurfaceComponent : public UPrimitiveComponent
{
	GENERATED_BODY()
	
public:
	UParametricSurfaceComponent(const FObjectInitializer& ObjectInitializer);

	//!< USceneComponent
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	//!< UPrimitiveComponent
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual class UMaterialInterface* GetMaterial(int32 ElementIndex) const override;
	virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials) const override;
	virtual int32 GetNumMaterials() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	UMaterialInterface* MaterialInterface;
};
