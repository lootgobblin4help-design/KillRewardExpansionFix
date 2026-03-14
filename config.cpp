class CfgPatches
{
	class KillRewardExpansionFix
	{
		units[]          = {};
		weapons[]        = {};
		requiredVersion  = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"JM_CF_Scripts",
			"KillReward",
			"DayZExpansion_Market_Scripts"
		};
	};
};

class CfgMods
{
	class KillRewardExpansionFix
	{
		dir         = "KillRewardExpansionFix";
		name        = "KillReward Expansion Market Fix";
		credits     = "";
		author      = "LOOTGOBBLIN";
		version     = "1.0";
		extra       = 0;
		type        = "mod";

		dependencies[] =
		{
			"Game",
			"World",
			"Mission"
		};

		class defs
		{
			class worldScriptModule
			{
				value   = "";
				files[] = { "KillRewardExpansionFix/Scripts/4_World" };
			};
		};
	};
};
