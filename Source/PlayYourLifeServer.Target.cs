// Copyright 2018 GGAMES, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PlayYourLifeServerTarget : TargetRules
{
	public PlayYourLifeServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.Add("PlayYourLife");
	}
}
