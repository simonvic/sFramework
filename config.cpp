#define S_UDE
#define S_FRAMEWORK

class CfgPatches {
	class sFramework {
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {
			"DZ_Scripts"
		};
	};
};

class CfgMods {
	class sFramework {
		dir = "MyMODS/sFramework";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "sFramework";
		credits = "simonvic";
		author = "simonvic";
		authorID = "0";
		version = "0.1";
		extra = 0;
		type = "mod";
		dependencies[] = {
			"Game",
			"World",
			"Mission"
		};
		class defs {
			class engineScriptModule{
				value = "";
				files[] = { 
					"MyMODS/sFramework/scripts/defines"
				};
			};
			class gameLibScriptModule{
				value = "";
				files[] = {
					"MyMODS/sFramework/scripts/defines"
				};
			};
			class gameScriptModule {
				value = "";
				files[] = {
					"MyMODS/sFramework/scripts/defines",
					"MyMODS/sFramework/scripts/3_Game"
				};
			};
			class worldScriptModule {
				value = "";
				files[] = {
					"MyMODS/sFramework/scripts/defines",
					"MyMODS/sFramework/scripts/4_World"
				};
			};
			class missionScriptModule {
				value = "";
				files[] = {
					"MyMODS/sFramework/scripts/defines",
					"MyMODS/sFramework/scripts/5_Mission"
				};
			};
		};
	};
};

class CfgVehicles{
	class SCameraOverlay {
		image="";                                 // Resource image path, can be whatever an ImageWidget accepts texture
		alpha=1.0;                                // [0.0 - 1.0] Alpha value (transparency)
		mask="";                                  // Resource image path, can be whatever an ImageWidget accepts as mask
		maskProgress=1.0;                         // [0.0 - 1.0] Mask progress
		maskTransitionWidth=1.0;                  // Mask transition width (used as progress + transitionWidth)
		position[] = {0.0, 0.0};                  // [0.0 - 1.0] X and Y position in screenspace
		size[] = {1.0, 1.0};                      // [0.0 - 1.0] X and Y size in screenspace
		rotation[] = {0.0, 0.0, 0.0};             // Yaw, Pitch and Roll defined in degrees
		priority = 0;                             // Higher priority means closer to the camera (also known as z-depth)
		targetCameras[] = {"DayZPlayerCamera"};   // Camera typename on which the overlay will be visible
		hidesWithIngameHUD = 0;                   // [0 = false, 1 = true] Determines if it must hides when the player hides the ingame HUD
	};

	class SCameraOverlay_Eyewear : SCameraOverlay {
		priority = 2;
		targetCameras[] = {"DayZPlayerCamera1stPerson", "DayZPlayerCameraIronsights"};
	};

	class SCameraOverlay_Headgear : SCameraOverlay {
		priority = 1;
		targetCameras[] = {"DayZPlayerCamera1stPerson", "DayZPlayerCameraIronsights"};
	};

};

class Cfg_sUDE{
	class BaseBuilding{
		enabled = 0;
	};
	class Caves{
		enabled = 0;
	};
	class Character{
		enabled = 0;
	};
	class Driving{
		enabled = 0;
	};
	class Inventory{
		enabled = 0;
	};
	class Looting{
		enabled = 0;
	};
	class Medical{
		enabled = 0;
	};
	class Gunplay{
		enabled = 0;
	};
	class SoftSkills{
		enabled = 0;
	};
	class Survival{
		enabled = 0;
	};
	class Visual{
		enabled = 0;	
	};
};