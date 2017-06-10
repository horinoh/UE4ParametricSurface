// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4ParametricSurfaceTarget : TargetRules
{
	public UE4ParametricSurfaceTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.Add("UE4ParametricSurface");
	}
}
