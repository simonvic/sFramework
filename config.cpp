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

		defines[] = {
			"sUDE",
			"S_UDE",
			"S_FRAMEWORK"
		};

		dependencies[] = {
			"Game",
			"World",
			"Mission"
		};
		class defs {
			class gameScriptModule {
				value = "";
				files[] = {
					"MyMODS/sFramework/scripts/3_Game"
				};
			};
			class worldScriptModule {
				value = "";
				files[] = {
					"MyMODS/sFramework/scripts/4_World"
				};
			};
			class missionScriptModule {
				value = "";
				files[] = {
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
	class Logging {
		class Levels {
			class Level {
				label = "";
				shortLabel = "";
			};
			class Trace : Level {
				label = "trace";
				shortLabel = "tt";
			};
			class Debug : Level {
				label = "debug";
				shortLabel = "dd";
			};
			class Info : Level {
				label = "info";
				shortLabel = "ii";
			};
			class Warning : Level {
				label = "warn";
				shortLabel = "ww";
			};
			class Error : Level {
				label = "error";
				shortLabel = "ee";
			};
			class Critical : Level {
				label = "critical";
				shortLabel = "cc";
			};
			class Fatal : Level {
				label = "fatal";
				shortLabel = "ff";
			};
		};
		class Formatters {
			class Formatter {
				type = "";
				entries[] = {};
			};
			class Default : Formatter {
				entries[] = { "${message}" };
			};
			class Full : Formatter {
				entries[] = { "${date:#Y-#m-#dT#H:#M:#S} [${level.shortLabel}] ${module} ${logger.name} | ${message}" };
			};
			class Compact : Formatter {
				entries[] = { "${date:#H:#M:#S} [${level.shortLabel}] ${logger.name} | ${message}" };
			};
			class Minimal : Formatter {
				entries[] = { "[${level.shortLabel}] ${message}" };
			};
			class JSON : Formatter {
				type = "JSON";
				entries[] = {
					"{",
					"	'date': '${date:#Y-#m-#dT#H:#M:#S}',",
					"	'level': {",
					"		'severity': ${level.severity},",
					"		'label': '${level.label}',",
					"		'shortLabel': '${level.shortLabel}'",
					"	},",
					"	'module': '${module}',",
					"	'logger': '${logger.name}',",
					"	'caller': '${caller}',",
					"	'message': '${message}'",
					"}"
				};
			};
			class CSV : Formatter {
				type = "CSV";
				entries[] = {
					"${date:#H:#M:#S}",
					"${level.severity}",
					"${level.label}",
					"${level.shortLabel}",
					"${module}",
					"${logger.name}",
					"${caller}",
					"${message}"
				};
				forceDoubleQuotes = 1;
				separator = ",";
			};
			class TSV : CSV {
				separator = "\t";
			};
		};
		class Writers {
			class Writer {
				type = "";
			};
			class Console : Writer {
				type = "Console";
			};
			class File : Writer {
				type = "File";
				logsFolder = "$profile:logs";
				logFilename = "${date:#Y-#m-#d}.log";
			};
			class Remote : Writer {
				type = "Remote";
				header[] = {"text/plain"};
				endpoint = "http://localhost:8080/sLogger";
			};

			class FileCSV : File {
				logFilename = "${date:#Y-#m-#d}.csv";
			};
			class FileJSON : File {
				logFilename = "${date:#Y-#m-#d}.json";
			};
			class RemoteREST : Remote {
				header[] = {"application/json"};
			};
			class SUDELogFile : File {
				logsFolder = "$profile:sUDE\\logs";
				logFilename = "${date:#Y-#m-#d}.log";
			};
		};
		class Appenders {
			class Appender {
				header[] = {};
				footer[] = {};
				formatter = "";
				writers[] = {};
			};
			class Default : Appender {
				formatter = "Default";
				writers[] = { "Console", "File" };
			};
			class Console : Appender {
				formatter = "Compact";
				writers[] = { "Console" };
			};
			class File : Appender {
				formatter = "Full";
				writers[] = { "File" };
			};
			class FileJSON : File {
				formatter = "JSON";
				writers[] = { "FileJSON" };
			};
			class FileCSV : File {
				formatter = "CSV";
				writers[] = { "FileCSV" };
			};
			class Remote : Appender {
				formatter = "Full";
				writers[] = { "Remote" };
			};
			class RemoteREST : Remote {
				formatter = "JSON";
				writers[] = { "RemoteREST" };
			};
			class SUDEAppender : Appender {
				header[] = {
					"============================================================================================",
					"            ${mod.name} | ${mod.version} | ${date:#Y-#m-#d #H:#M:#S}",
					"--------------------------------------------------------------------------------------------",
					"            DayZ version: ${dayz.version}",
					"            Profile name: ${dayz.profileName}",
					"            Machine name: ${dayz.machineName}",
					"             Average FPS: ${dayz.averageFPS}",
					"                    Time: ${dayz.time}",
					"               Tick time: ${dayz.tickTime}",
					"              World name: ${dayz.worldName}",
					"                    Mods: ${dayz.mods}",
					"--------------------------------------------------------------------------------------------",
					"FF = FATAL | CC = CRITICAL | EE = ERROR | WW = WARNING | II = INFO | DD = DEBUG | TT = TRACE",
					"____________________________________________________________________________________________",
					"",
					""
				};
				footer[] = {
					"--------------------------------------${date:#H:#m:#S}--------------------------------------",
					"",
					""
				};
				formatter = "Full";
				writers[] = {
					"SUDELogFile"
				};
			};
		};
		class Loggers {
			class Logger {
				severityThreshold = 0;
				levels[] = {};
				appenders[] = {};
			};
			class Legacy : Logger {
				levels[] = {
					"Debug",
					"Info",
					"Warning",
					"Error",
					"Critical"
				};
				appenders[] = { "SUDEAppender" };
			};
			class Default : Logger {				
				levels[] = {
					"Trace",
					"Debug",
					"Info",
					"Warning",
					"Error",
					"Critical",
					"Fatal"
				};
				appenders[] = { "Default" };
			};
			class WorkbenchDebugLogger : Default {
				appenders[] = {
					"Console",
					"File",
					"FileJSON",
					"FileCSV",
					"RemoteREST",
					"SUDEAppender"
				};
			};
		};
	};
};