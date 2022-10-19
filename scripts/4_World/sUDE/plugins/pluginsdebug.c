/*
class SDebugCheats {
	static bool weapon_noDamage = true;
	static bool weapon_infAmmo = true;
	static bool weapon_infMags = true;
	static bool weapon_noJam = true;
	static bool player_noDamage = false;
}

modded class Weapon_Base {
	
	override void EEFired(int muzzleType, int mode, string ammoType) {
		super.EEFired(muzzleType, mode, ammoType);
		if (SDebugCheats.weapon_noDamage) {
			if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
				SetHealth(GetMaxHealth());
				ItemSuppressor suppressor = GetAttachedSuppressor();
				if (suppressor) {
					suppressor.SetHealth(suppressor.GetMaxHealth());
				}
			}
		}
		
		
		if (SDebugCheats.weapon_infAmmo) {
			Magazine magazine = GetMagazine(GetCurrentMuzzle());
			if (magazine == null) {
				FillChamber();
				SpawnAmmo();
			} else {
				if (SDebugCheats.weapon_infMags || magazine.GetAmmoCount() == 0) {
					if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
						magazine.ServerSetAmmoMax();
					}
					if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
						magazine.LocalSetAmmoMax();
					}
				}
			}
		}
		
	}
	
	override bool IsJammed() {
		if (SDebugCheats.weapon_noJam) return false;
		return super.IsJammed();
	}
	
}

modded class PluginManager {
	override void Init() {
		super.Init();
		//                              client, server
		RegisterPlugin( "PluginSDebug", true,   true);
	}
}


class PluginSDebug : PluginBase {
	
	protected float m_time;
	
	static PlayerBase simonvic;
	static ItemBase itemInHands;
	static Weapon_Base weapon;
	static int simonvicServerWeight;
	
	static string steamID64;
	
	static ref array<SurvivorBase> theBoris = {};
	static ref array<Object> spawnedItems = {};
	
	
	static ref SDebugUI dui;
		
	static ref SCOAnimationDebug scoAnimated;
	static ref SCOTimedDebug scoTimed;
	
	void PluginSDebug() {
		if (GetGame().IsClient()) {
			dui = SDebugUI.of(ClassName());
			scoAnimated = new SCOAnimationDebug();
			scoTimed = new SCOTimedDebug();
		}
		DayZGame.Event_OnRPC.Insert(this.onRPC);
	}
	
	void ~PluginSDebug() {
		deleteAll();
	}
	
	override void OnUpdate(float delta_time) {
		m_time += delta_time;
		if (GetGame().IsClient() || !GetGame().IsMultiplayer()) {
			simonvic = PlayerBase.Cast(GetGame().GetPlayer());
			if (simonvic) {
				itemInHands = simonvic.GetItemInHands();
				Class.CastTo(weapon, itemInHands);
			}
			onUpdateClient(delta_time);
		}
		
		if (GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			array<Man> players = {};
			GetGame().GetPlayers(players);
			simonvic = PlayerBase.Cast(players[0]);
			if (simonvic) {
				itemInHands = simonvic.GetItemInHands();
				Class.CastTo(weapon, itemInHands);
			}
			onUpdateServer(delta_time);
		}
		
		onUpdateBoth(delta_time);
	}
	
	void onUpdateBoth(float delta_time) {
		//SPPEManager.m_resultPPE.debugUI();
		//SPPEManager.debugOverride("Graphics/Materials/postprocess/glow", "TargetBrightness", 0.5);
	}
	
	void onUpdateServer(float delta_time) {
		if (!simonvic) return;
		if (simonvicServerWeight != simonvic.GetWeight()) {
			simonvicServerWeight = simonvic.GetWeight();
			GetGame().RPCSingleParam(null, RPCID_DEBUG_SYNC, new Param2<string, string>("simonvicServerWeight", ""+simonvicServerWeight), true);
		}
	}
	
	static bool weapon_noDamage = SDebugCheats.weapon_noDamage;
	static bool weapon_infAmmo = SDebugCheats.weapon_infAmmo;
	static bool weapon_infMags = SDebugCheats.weapon_infMags;
	static bool weapon_noJam = SDebugCheats.weapon_noJam;
	static bool player_noDamage = SDebugCheats.player_noDamage;

	
	void onUpdateClient(float delta_time) {
		if (!simonvic) return;
		dui.begin();
		dui.size("175px 1").window("Debug");
		string keybindings;
		keybindings += "[NUMPAD *] to enable cursor<br/>";
		keybindings += "[NUMPAD /] to disable cursor<br/>";
		keybindings += "[NUMPAD +] to enable UI<br/>";
		keybindings += "[NUMPAD -] to disable UI";
		dui.size("1 64px").textrich(keybindings);
		SoftSkillsManager ssm = simonvic.GetSoftSkillsManager();
		float strength;
		dui.size("1 24px").slider("Strength (soft skill)", strength, 0.1, -1, 1);
		if (ssm.GetSpecialtyLevel() != strength) {
			debugRPC("specialty", ""+strength);
		}
		ssm.SetSpecialtyLevel(strength);
		simonvic.GetStatSpecialty().Set(strength);
		
		
		dui.size("1 64px").table({
			{"Weight(grams)"}
			{"client", ""+simonvic.GetWeight()}
			{"server", ""+simonvicServerWeight}
			{"desync", ""+(simonvic.GetWeight() - simonvicServerWeight)}
		});
		dui.text("Debug toggles");
		dui.check("weapon_noDamage", weapon_noDamage);
		syncDebugCheat("weapon_noDamage", weapon_noDamage, SDebugCheats.weapon_noDamage);
		
		dui.check("weapon_infAmmo", weapon_infAmmo);
		syncDebugCheat("weapon_infAmmo", weapon_infAmmo, SDebugCheats.weapon_infAmmo);
		
		dui.check("weapon_infMags", weapon_infMags);
		syncDebugCheat("weapon_infMags", weapon_infMags, SDebugCheats.weapon_infMags);
		
		dui.check("weapon_noJam", weapon_noJam);
		syncDebugCheat("weapon_noJam", weapon_noJam, SDebugCheats.weapon_noJam);
		
		dui.check("player_noDamage", player_noDamage);
		syncDebugCheat("player_noDamage", player_noDamage, SDebugCheats.player_noDamage);
		
		dui.newline();
		
		dui.button("player_heal", this, "heal");
		dui.button("spawn_weapons", this, "debugRPC", new Param1<string>("spawn_weapons"));
		dui.button("spawn_boris", this, "debugRPC", new Param1<string>("spawn_boris"));
		dui.button("delete_all", this, "debugRPC", new Param1<string>("delete_all"));
		
		
		if (weapon) {
			dui.pos("200px 0").size("300px").window(weapon.GetDebugName());
			array<ref array<string>> recoilTable = {{"No data available."}, {"Shoot once to show recoil stats"}};
			RecoilBase recoil = simonvic.GetAimingModel().getRecoil();
			if (recoil) {
				recoilTable = recoil.toDebugTable();
			}
			dui.table(recoilTable);
			dui.spacer();			
			auto m = weapon.GetPropertyModifierObject();
			dui.size("300px 64px").table({
				{"Attachments modifiers"}
				{"mouse",        string.Format("%1%% %2%%", m.recoilControlMouseX*100,        m.recoilControlMouseY*100)}
				{"stability",    string.Format("%1%% %2%%", m.recoilControlHandsX*100,        m.recoilControlHandsY*100)}
				{"misalignment", string.Format("%1%% %2%%", m.recoilControlMisalignmentX*100, m.recoilControlMisalignmentY*100)}
				{"kick",         string.Format("%1%%",      m.recoilControlKick*100)}
			});
			
			dui.spacer();
			dui.table({
				{"Stats"}
				{"weight", ""+weapon.GetWeight()}
				{"length", ""+weapon.getWeaponLength()}
			});
		}
		
		
		
	}
	
	private void syncDebugCheat(string name, bool localCheat, out bool cheat) {
		if (localCheat != cheat) {
			debugRPC(name, ""+localCheat);
		}
		cheat = localCheat;
	}
	
	static int RPCID_DEBUG_SYNC = SRPCIDs.SYNC_USER_CONFIG_CONSTRAINTS + 1;
	
	private void debugRPC(string name, string value = "") {
		SLog.d(name + " = " + value, "debugRPC");
		GetGame().RPCSingleParam(null, RPCID_DEBUG_SYNC, new Param2<string, string>(name, value), true);
	}

	void onRPC(PlayerIdentity sender, Object target, int rpcId, ParamsReadContext ctx) {
		if (rpcId != RPCID_DEBUG_SYNC) return;
		Param2<string, string> p;
		if (ctx.Read(p)) {
			onDebugRPC(p.param1, p.param2, sender, target);
		} else {
			SLog.c("Can't read debug sync param!");
		}
		
		
	}
	
	void onDebugRPC(string name, string value, PlayerIdentity sender, Object target) {
		if (GetGame().IsServer()) {
			switch (name) {
				case "specialty":
				simonvic.GetSoftSkillsManager().SetSpecialtyLevel(value.ToFloat());
				simonvic.GetStatSpecialty().Set(value.ToFloat());
				break;
				case "weapon_noDamage": SDebugCheats.weapon_noDamage = value == "true"; break;
				case "weapon_infAmmo":  SDebugCheats.weapon_infAmmo = value == "true"; break;
				case "weapon_infMags":  SDebugCheats.weapon_infMags = value == "true"; break;
				case "weapon_noJam":    SDebugCheats.weapon_noJam = value == "true"; break;
				case "player_noDamage": godmode(value == "true"); break;
				case "player_heal":     heal(); break;
				case "player_teleport": teleport(value.ToVector()); break;
				case "spawn_infected":  spawnInfected(value.ToVector()); break;
				case "spawn_apple":     spawnApple(value.ToVector()); break;
				case "spawn_weapons":   spawnWeaponsSet(getPlayerByIdentity(sender).GetPosition() + "0 1 0"); break;
				case "spawn_boris":     spawnDefaultBorisDummies(); break;
				case "delete_all":      deleteAll(); break;
				case "exchangeSteamID": GetGame().RPCSingleParam(null, RPCID_DEBUG_SYNC, new Param2<string, string>("exchangeSteamID", sender.GetPlainId()), true, sender); break;				
				case "debugSpawn":      debugSpawn(value);
				default:                Print("unknown cheat");
			}
		}
		
		if (GetGame().IsClient()) {
			switch (name) {
				case "exchangeSteamID": steamID64 = value; break;
				case "simonvicServerWeight": simonvicServerWeight = value.ToInt(); break;
			}
		}
	}
	
	private static PlayerBase getPlayerByIdentity(PlayerIdentity identity) {
		array<Man> players = {};
		GetGame().GetPlayers(players);
		foreach (Man player : players) {
			if (player.GetIdentity() == identity) {
				return PlayerBase.Cast(player);
			}
		}
		return null;
	}
	
	private void spawnInfected(vector target) {
		if (GetGame().IsClient()) {
			debugRPC("spawn_infected", string.Format("%1 %2 %3", target[0], target[1], target[2]));
			return;
		}
		GetGame().CreateObject("ZmbM_NBC_Yellow", target, false, true);
	}
	
	private void spawnApple(vector target) {
		if (GetGame().IsClient()) {
			debugRPC("spawn_apple", string.Format("%1 %2 %3", target[0], target[1], target[2]));
			return;
		}
		Object apple = GetGame().CreateObject("Apple", target);
		dBodyDestroy(apple);
		autoptr PhysicsGeomDef geoms[] = {PhysicsGeomDef("", dGeomCreateSphere( 0.1 ), "material/default", 0xffffffff)};
		dBodyCreateDynamicEx(apple, "0 0 0", 1.0, geoms);
	}
	
	private void godmode(bool enable = true) {
		SDebugCheats.player_noDamage = enable;
		player_noDamage = enable;
		if (GetGame().IsClient()) {
			debugRPC("player_noDamage", ""+enable);
			return;
		}
		simonvic.SetAllowDamage(!enable);
	}
	
	private void heal() {
		if (GetGame().IsClient()) {
			debugRPC("player_heal");
			return;
		}
		simonvic.SetHealth(simonvic.GetMaxHealth("", ""));
		simonvic.SetHealth( "","Blood", simonvic.GetMaxHealth("","Blood"));
		simonvic.SetHealth("","Shock", simonvic.GetMaxHealth("","Shock"));
		simonvic.GetStatHeatComfort().Set(simonvic.GetStatHeatComfort().GetMax());
		simonvic.GetStatTremor().Set(simonvic.GetStatTremor().GetMin());
		simonvic.GetStatWet().Set(simonvic.GetStatWet().GetMin());
		simonvic.GetStatEnergy().Set(simonvic.GetStatEnergy().GetMax());
		simonvic.GetStatWater().Set(simonvic.GetStatWater().GetMax());
		simonvic.GetStatDiet().Set(simonvic.GetStatDiet().GetMax());
		simonvic.AddHealth("LeftLeg","Health",(simonvic.GetMaxHealth("LeftLeg", "Health")  - simonvic.GetHealth("LeftLeg", "Health")));
		simonvic.AddHealth("RightLeg","Health",(simonvic.GetMaxHealth("RightLeg", "Health") - simonvic.GetHealth("RightLeg", "Health")));
		simonvic.AddHealth("RightFoot","Health",(simonvic.GetMaxHealth("RightFoot", "Health") - simonvic.GetHealth("RightFoot", "Health")));
		simonvic.AddHealth("LeftFoot","Health",(simonvic.GetMaxHealth("LeftFoot", "Health") - simonvic.GetHealth("LeftFoot", "Health")));
		simonvic.GetBleedingManagerServer().RemoveAllSources();
		simonvic.RemoveAllAgents();
	}
	
	private void teleport(vector target) {
		if (GetGame().IsClient()) {
			debugRPC("player_teleport", string.Format("%1 %2 %3", target[0], target[1], target[2]));
			return;
		}
		simonvic.SetPosition(target);
	}
	
	void onKeyPress(int keycode) {
		switch (keycode) {
			case KeyCode.KC_MULTIPLY:
			GetGame().GetInput().ChangeGameFocus(1);
			GetGame().GetUIManager().ShowUICursor(true);			
			break;
			
			case KeyCode.KC_DIVIDE:
			GetGame().GetInput().ChangeGameFocus(-1);
			GetGame().GetUIManager().ShowUICursor(false);
			break;
			
			case KeyCode.KC_ADD:
			foreach (auto d : SDebugUI.instances) d.show();
			dui.show();
			break;
			
			case KeyCode.KC_SUBTRACT:
			foreach (auto i : SDebugUI.instances) i.hide();
			dui.hide();
			break;
			
			case KeyCode.KC_NUMPAD1: heal(); break;
			//case KeyCode.KC_NUMPAD2: godmode(!player_noDamage); break;
			case KeyCode.KC_NUMPAD5: teleport(getLookingPosition(10000)); break;
			case KeyCode.KC_NUMPAD7: spawnInfected(getLookingPosition(500)); break;
			case KeyCode.KC_M: spawnApple(getLookingPosition(1)); break;
		}		
		//Print(typename.EnumToString(KeyCode, keycode));
	}
	
	static vector getLookingPosition(float distance) {
		vector camPos = GetGame().GetCurrentCameraPosition();
		SRaycast ray = new SRaycast(camPos, camPos + GetGame().GetCurrentCameraDirection() * distance);
		vector contactPos = ray.ignore(simonvic).launch().getContactPosition();
		delete ray;
		return contactPos;
	}
	

	static void setupShootingDebugArea() {
		
		if (!simonvic) {
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			simonvic = PlayerBase.Cast(players[0]);
		}
		vector startPosition = simonvic.GetPosition();
		
		spawnedItems.Insert(SSpawnable.build("StaticObj_Misc_Sandbox").spawn(startPosition + Vector(0,-0.5,0)).collect());
		PluginSDebug.spawnWeaponsSet(startPosition + "0 1.35 -5");
		PluginSDebug.spawnDefaultBorisDummies();
	}
	
	static ref array<ref array<string>> weaponsByCat = {
		{"M4A1","M16A2","AK101","AK74","AKM","AKS74U","FAL","VSS","ASVAL","Aug","AugShort","FAMAS","SawedOffFAMAS"}
		{"B95","CZ527","Winchester70","Mosin9130","SawedoffMosin9130","Repeater","Ruger1022","SVD","SKS","Izh18","SawedoffIzh18"}
		{"Mp133Shotgun", "Saiga", "Izh43Shotgun", "SawedoffIzh43Shotgun"}
		{"MP5K","CZ61","UMP45","PP19"}
		{"Magnum","MakarovIJ70","MKII","Colt1911","CZ75","FNX45","Deagle","LongHorn"}
	};
	
	static void spawnWeaponsSet(vector position) {
		if (position == "0 0 0") position = simonvic.GetPosition();
		vector pos = position;
		foreach (auto weaponsList : weaponsByCat) {
			foreach (auto w : weaponsList) {
				Object o = GetGame().CreateObject(w, pos);
				EntityAI e = EntityAI.Cast(o);
				if (!e) {
					Print("Can't spawn " + w);
				} else {
					Print("spawning " + w);
					e.SetOrientation("180 0 90");
					e.OnDebugSpawn();
					spawnedItems.Insert(e);
					pos = pos + "1 0 0";
				}
			}
			pos = Vector(position[0] - 3, position[1], pos[2] - 2);
		}
	}
	
	static void debugSpawn(string type) {
		if (GetGame().IsClient()) {
			PluginSDebug.Cast(GetPluginManager().GetPluginByType(PluginSDebug)).debugRPC("debugSpawn", type);
			return;
		}
		vector pos = simonvic.GetPosition();
		EntityAI.Cast(GetGame().CreateObject(type, pos)).OnDebugSpawn();
	}
	
	static void spawnBorisDummies(vector startPosition, array<float> distances, vector direction) {
		foreach (float dis : distances) {
			Object boris = GetGame().CreateObject("SurvivorM_Boris", startPosition + direction * dis, false, true, true);
			if (boris != null) {
				boris.SetAllowDamage(false);
				theBoris.Insert(SurvivorBase.Cast(boris));
			}
		}
	}
	
	static void spawnDefaultBorisDummies() {
		PluginSDebug.spawnBorisDummies(simonvic.GetPosition(), {
			25,
			50,
			100,
			220,
			400,
			600,
			800,
			1000
		}, simonvic.GetDirection());
	}
	

	static void setTheBorisInvincibility(bool invincible) {
		foreach (SurvivorBase boris : theBoris) {
			boris.SetAllowDamage(!invincible);
		}
	}
	

	

	
	static void deleteTheBoris() {
		foreach (auto o : theBoris) {
			GetGame().ObjectDelete(o);
			GetGame().ObjectDeleteOnClient(o);
		}
	}
	
	static void deleteSpawnedItems() {
		foreach (auto o : spawnedItems) {
			GetGame().ObjectDelete(o);
			GetGame().ObjectDeleteOnClient(o);
		}
	}
	
	static void deleteAll() {
		deleteTheBoris();
		deleteSpawnedItems();
	}
	

	static void updateMovementSettings() {
		SHumanCommandMoveSettings hcm = simonvic.GetDayZPlayerType().CommandMoveSettingsW();

		//! run sprint (SHIFT HOLD) filter 
		hcm.m_fRunSpringTimeout = 0.1;							//!< filter span value		[s]
		hcm.m_fRunSpringMaxChange = 10.0;							//!< filter speed value		[val/s]

		//! WSAD direction filter 
		hcm.m_fDirFilterTimeout = 0.1;						//!< direction filter timeout [s]
		hcm.m_fDirFilterSpeed = Math.DEG2RAD * 360; 		//!< direction filter max rot speed [rad/s]
		hcm.m_fMaxSprintAngle = Math.DEG2RAD * 45.01;		//!< max sprint angle [rad]

		hcm.m_fTurnAngle = 45;						//!< this is when char starts tu turn
		hcm.m_fTurnEndUpdateNTime = 0.7;						//!< this is end norm time (turn amount is not updated after this time)
		hcm.m_fTurnNextNTime = 0.9;						//!< this is norm time when new turn from old turn is evaluated again (turn->turn)


		hcm.m_fHeadingChangeLimiterIdle = 500000; 			//!<
		hcm.m_fHeadingChangeLimiterWalk = 2000;				//!<
		hcm.m_fHeadingChangeLimiterRun = 1500;				//!<		
		hcm.m_fLeaningSpeed = 3.0;
		simonvic.StartCommand_Move();
	}
	
	static void logAllRecoilsCSV(string path = "$saves://sGunplay_recoils.csv") {
		SFileHelper.touch(path);
		SFileHelper.echo(RecoilBase.toCSVStringAll(), path);
	}
	
	
	static void sandbox() {

	}
}

*/