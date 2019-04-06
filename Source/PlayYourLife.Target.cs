// Copyright 2018 GGAMES, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PlayYourLifeTarget : TargetRules
{
	public PlayYourLifeTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "PlayYourLife" } );
	}
}
