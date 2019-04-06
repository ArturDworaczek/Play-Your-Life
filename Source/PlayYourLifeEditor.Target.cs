// Copyright 2018 GGAMES, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class PlayYourLifeEditorTarget : TargetRules
{
	public PlayYourLifeEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "PlayYourLife" } );
	}
}
