// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4ParametricSurface.h"
#include "ParametricSurfaceComponent.h"

#include "DynamicMeshBuilder.h"

class FParametricSurfaceSceneProxy : public FPrimitiveSceneProxy
{
public:
	FParametricSurfaceSceneProxy(const UParametricSurfaceComponent* InComponent)
		: FPrimitiveSceneProxy(InComponent)
		, MaterialInterface(InComponent->MaterialInterface)
	{
	}

	//!< FPrimitiveSceneProxy
	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, class FMeshElementCollector& Collector) const override
	{
		FDynamicMeshBuilder MeshBuilder;

		const auto Num = 20;
		const auto DeltaUV = 1.0f / (Num - 1);
		TArray<FDynamicMeshVertex> Vertices;
		Vertices.Reserve(Num * Num);
		for (auto i = 0; i < Num; ++i)
		{
			for (auto j = 0; j < Num; ++j)
			{
				const auto UV = FVector2D(DeltaUV * j, DeltaUV * i);

				FDynamicMeshVertex Vertex;
				Vertex.Position = GetPosition(UV);
				Vertex.TextureCoordinate = GetUV(UV);
				const auto Edge01 = GetPosition(UV + FVector2D(0.01f, 0.0f)) - Vertex.Position;
				const auto Edge02 = GetPosition(UV - FVector2D(0.0f, 0.01f)) - Vertex.Position;
				Vertex.TangentX = Edge01.GetSafeNormal();
				Vertex.TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
				Vertex.Color = FColor::Green;

				Vertices.Add(Vertex);
			}
		}
		MeshBuilder.AddVertices(Vertices);

		TArray<int32> Indices;
		Indices.Reserve((Num - 1) * (Num - 1) * 6);
		for (auto i = 0; i < Num - 1; ++i)
		{
			for (auto j = 0; j < Num - 1; ++j)
			{
				const auto Index = j + i * Num;
				Indices.Add(Index);
				Indices.Add(Index + Num);
				Indices.Add(Index + 1);

				Indices.Add(Index + 1);
				Indices.Add(Index + Num);
				Indices.Add(Index + Num + 1);
			}
		}
		MeshBuilder.AddTriangles(Indices);

		auto MaterialRenderProxy = UMaterial::GetDefaultMaterial(MD_Surface)->GetRenderProxy(IsSelected());
		if (nullptr != MaterialInterface)
		{
			MaterialRenderProxy = MaterialInterface->GetRenderProxy(false);
		}
		if (Views[0]->Family->EngineShowFlags.Wireframe)
		{
			MaterialRenderProxy = GEngine->WireframeMaterial->GetRenderProxy(IsSelected());
		}
		MeshBuilder.GetMesh(GetLocalToWorld(), MaterialRenderProxy, 0, false, false, 0, Collector);
	}
	virtual uint32 GetMemoryFootprint(void) const
	{
		return sizeof(*this) + FPrimitiveSceneProxy::GetAllocatedSize();
	}
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bDynamicRelevance = true;
		Result.bShadowRelevance = IsShadowCast(View);
		return Result;
	}

	virtual FVector2D GetUV(const FVector2D& InUV) const
	{
		return GetUV_Torus(InUV);
		//return GetUV_Pillow(InUV);
		//return GetUV_Snail(InUV);
	}
	virtual FVector GetPosition(const FVector2D& InUV) const
	{
		return GetPosition_Torus(InUV) * 100.0f;
		//return GetPosition_Pillow(InUV) * 100.0f;
		//return GetPosition_Snail(InUV) * 100.0f;
	}

	//!< ParametricSurface http://www.3d-meier.de/tut3/Seite0.html
	FVector2D GetUV_Torus(const FVector2D& InUV) const { return InUV * 2.0f * PI; }
	FVector GetPosition_Torus(const FVector2D& InUV) const
	{
		const auto A = 1.0f;
		const auto B = 0.5f;
		const auto UV = GetUV(InUV);
		return FVector((A + B * FMath::Cos(UV.Y)) * FMath::Cos(UV.X), (A + B * FMath::Cos(UV.Y)) * FMath::Sin(UV.X), B * FMath::Sin(UV.Y));
	}
	FVector2D GetUV_Pillow(const FVector2D& InUV) const { return FVector2D(InUV.X* PI, (InUV.Y * 2.0f - 1.0f) * PI); }
	FVector GetPosition_Pillow(const FVector2D& InUV) const
	{
		const auto A = 0.5f;
		const auto UV = GetUV(InUV);
		return FVector(FMath::Cos(UV.X), FMath::Cos(UV.Y), A * FMath::Sin(UV.X) * FMath::Sin(UV.Y));
	}
	FVector2D GetUV_Snail(const FVector2D& InUV) const { return FVector2D(InUV.X* PI, (InUV.Y * 2.0f - 1.0f) * PI); }
	FVector GetPosition_Snail(const FVector2D& InUV) const
	{
		const auto UV = GetUV(InUV);
		return FVector(UV.X * FMath::Cos(UV.Y) * FMath::Sin(UV.X), UV.X * FMath::Cos(UV.X) * FMath::Cos(UV.Y), -UV.X * FMath::Sin(UV.Y));
	}

protected:
	UMaterialInterface* MaterialInterface;
};

UParametricSurfaceComponent::UParametricSurfaceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MaterialInterface(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MI(TEXT("MaterialInstanceConstant'/Game/Materials/M_Simple_Inst.M_Simple_Inst'"));
	if (MI.Succeeded())
	{
		MaterialInterface = MI.Object;
	}
}

FBoxSphereBounds UParametricSurfaceComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	const auto Radius = 100.0f;
	const auto PosMax = FVector(Radius, Radius, Radius);
	const auto PosMin = -PosMax;
	const auto Center = (PosMin + PosMax) * 0.5f;
	const auto Extent = PosMax - PosMin;
#if 0
	const auto BSB = FBoxSphereBounds(Center, Extent, Extent.Size()).TransformBy(LocalToWorld);
	const auto Box = BSB.GetBox();
	DrawDebugBox(GetWorld(), Box.GetCenter(), Box.GetExtent(), FColor::Green, true);
	return BSB;
#else
	return FBoxSphereBounds(Center, Extent, Extent.Size()).TransformBy(LocalToWorld);
#endif
}

FPrimitiveSceneProxy* UParametricSurfaceComponent::CreateSceneProxy()
{
	return new FParametricSurfaceSceneProxy(this);
}

class UMaterialInterface* UParametricSurfaceComponent::GetMaterial(int32 ElementIndex) const
{
	return (0 == ElementIndex) ? MaterialInterface : nullptr;
}

void UParametricSurfaceComponent::GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials, bool bGetDebugMaterials) const
{
	if (nullptr != MaterialInterface)
	{
		OutMaterials.Add(MaterialInterface);
	}
}

int32 UParametricSurfaceComponent::GetNumMaterials() const
{
	return (nullptr == MaterialInterface) ? 0 : 1;
}