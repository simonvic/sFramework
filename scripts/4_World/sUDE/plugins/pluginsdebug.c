/*
modded class Weapon_Base{
	override void EEFired(int muzzleType, int mode, string ammoType){
		super.EEFired(muzzleType, mode, ammoType);

		Magazine magazine = GetMagazine(GetCurrentMuzzle());

		if (GetGame().IsServer() || !GetGame().IsMultiplayer())
			SetHealth(GetMaxHealth()); // prevent weapon from deteriorating

		if (magazine){
			if (GetGame().IsServer() || !GetGame().IsMultiplayer())
				magazine.ServerSetAmmoMax(); // unlimited ammo
			
			if (GetGame().IsClient() || !GetGame().IsMultiplayer())
				magazine.LocalSetAmmoMax(); // update client side UI
		}
		
		ItemSuppressor suppressor = GetAttachedSuppressor();
		if (suppressor && (GetGame().IsServer() || !GetGame().IsMultiplayer())) {
			suppressor.SetHealth(suppressor.GetMaxHealth());
		}
	}

	override bool IsJammed(){
		return false; // prevent jamming
	}
}

class PluginSDebug : PluginBase {
	
	protected float m_time;
	
	static bool crosshair_enabled = false;
	static bool bodyClipAllContact_enabled = false;
	static bool bodyClipContactPos_enabled = false;
		
	static PlayerBase simonvic;
	static Weapon_Base m_weapon;
	ref SRaycast m_crosshairRaycast;
	ref SRaycast m_bodyClipRaycast;
	
	static ref array<SurvivorBase> theBoris = new array<SurvivorBase>;
	
	Shape line = Debug.DrawLine("0 0 0", "0 0 0", 0xFF0000);
	
	void PluginSDebug(){
		m_crosshairRaycast = new SRaycast(vector.Zero, vector.Zero, 0.05, ObjIntersectView, CollisionFlags.NEARESTCONTACT);
		m_bodyClipRaycast = new SRaycast(vector.Zero, vector.Zero, 0.05, 0, CollisionFlags.FIRSTCONTACT);
	}
	
	void ~PluginSDebug(){
	}
	
	override void OnUpdate(float delta_time){
		
		if(GetGame().IsClient() || !GetGame().IsMultiplayer()) {
			simonvic = PlayerBase.Cast(GetGame().GetPlayer());
			onUpdateClient(delta_time);
		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			simonvic = PlayerBase.Cast(players[0]);
			onUpdateServer(delta_time);
		}
		
		onUpdateBoth(delta_time);
	}
	
	void onUpdateBoth(float delta_time){
	}
	
	void onUpdateClient(float delta_time){
		if(!simonvic) simonvic = PlayerBase.Cast(GetGame().GetPlayer());
		if(simonvic && Weapon_Base.Cast(simonvic.GetItemInHands())) m_weapon = Weapon_Base.Cast(simonvic.GetItemInHands());
		
		m_time += delta_time;
		if(crosshair_enabled) updateCrosshair();
		
		if(bodyClipAllContact_enabled || bodyClipContactPos_enabled) updateBodyClip();
		
		if(false) updateMouse();
	}


	void onUpdateServer(float delta_time){
	}
	
	
	static void setupShootingDebugArea(){
		if(!simonvic){
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			simonvic = PlayerBase.Cast(players[0]);
		}
		vector startPosition = simonvic.GetPosition();
		vector margin = "0 0 0.5";
		vector verticalMargin = "0 0.2 0";
		
		PluginSDebug.spawnWeaponsSet(startPosition, margin);
		PluginSDebug.spawnAmmoSet(startPosition + "-1 0 0", margin, verticalMargin);
		PluginSDebug.spawnDefaultBorisDummies();
	}
	
	
	static void spawnBorisDummies(vector startPosition, array<float> distances, vector direction){
		foreach(float dis : distances){
			EntityAI boris = null;
			SSpawnable.build("SurvivorM_Boris").spawn(startPosition + direction * dis).collect(boris);
			if(boris != null){
				boris.SetCanBeDestroyed(false);
				theBoris.Insert(SurvivorBase.Cast(boris));
			}
		}
	}
	
	static void spawnProneBoris(){
		SurvivorBase boris = SurvivorBase.Cast(GetGame().CreateObject("SurvivorM_Boris",simonvic.GetPosition() + "1 2 1"));
		boris.SetOrientation("0 0 0");
		HumanCommandMove hcm = boris.StartCommand_Move();
		Print(hcm);
		if(hcm){
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(hcm.ForceStance, 1000, false, DayZPlayerConstants.STANCEIDX_PRONE);
			hcm.ForceStance(DayZPlayerConstants.STANCEIDX_PRONE);
		}
	}
	
	static void spawnDefaultBorisDummies(){
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
	
	static void deleteTheBoris(){
		foreach(SurvivorBase boris : theBoris){
			GetGame().ObjectDelete(boris);
			GetGame().ObjectDeleteOnClient(boris);
		}
	}
	
	static void setTheBorisInvincibility(bool invincible){
		foreach(SurvivorBase boris : theBoris){
			boris.SetCanBeDestroyed(!invincible);
		}
	}
	
	
	
	static void spawnWeaponsSet(vector startPosition, vector margin){
		
		SSpawnableBundle s = new SSpawnableBundle();
		
		////////////////////////////////////////////////////////////
		// ASSAULT RIFLES
		s.build("M4A1").withAttachments({
			"M4_Suppressor",
			"M4_OEBttstck",
			"M4_RISHndgrd"
		}).withSpawnableAttachments(
			(new SSpawnable("ReflexOptic")).withAttachment("Battery9V"));
		
		s.build("M16A2");
		
		s.build("AK101").withAttachments({
			"AK_Suppressor",
			"AK_PlasticBttstck",
			"AK_RailHndgrd"
		}).withSpawnableAttachments(
			(new SSpawnable("PSO11Optic")).withAttachment("Battery9V"),
			(new SSpawnable("UniversalLight")).withAttachment("Battery9V"));
		
		s.build("AK74").withAttachments({
			"AK_Suppressor",
			"AK_PlasticBttstck",
			"AK_RailHndgrd"
		}).withSpawnableAttachments(
			(new SSpawnable("KobraOptic")).withAttachment("Battery9V"),
			(new SSpawnable("UniversalLight")).withAttachment("Battery9V"));
		
		s.build("AKM").withAttachments({
			"AK_Suppressor",
			"AK_WoodBttstck",
			"AK_WoodHndgrd"
		}).withSpawnableAttachments(
			(new SSpawnable("KobraOptic")).withAttachment("Battery9V"));
		
		s.build("AKS74U").withAttachments({
			"AK_Suppressor",
			"AKS74U_Bttstck",
			"GhillieAtt_tan"
		});
		
		s.build("FAL").withAttachments({
			"Fal_FoldingBttstck"
		}).withSpawnableAttachments(
			(new SSpawnable("M68Optic")).withAttachment("Battery9V"));
		
		s.build("VSS").withAttachments({
			"KashtanOptic"
		});
		
		s.build("ASVAL").withSpawnableAttachments(
			(new SSpawnable("UniversalLight")).withAttachment("Battery9V"),
			(new SSpawnable("ACOGOptic")).withAttachment("Battery9V"));
		
		s.build("Aug").withAttachments({
			"M4_Suppressor",
			"GhillieAtt_tan"
		}).withSpawnableAttachments(
			(new SSpawnable("UniversalLight")).withAttachment("Battery9V"),
			(new SSpawnable("ReflexOptic")).withAttachment("Battery9V"));
		
		
		s.build("AugShort").withAttachments({
			"M4_Suppressor"
		});
		
		s.build("FAMAS").withAttachments({
			"M4_Suppressor"
		});
		
		s.build("SawedOffFAMAS").withAttachments({
			"M4_Suppressor"
		});
		
		////////////////////////////////////////////////////////////
		// SMG
		s.build("MP5K").withAttachments({
			"MP5_Compensator",
			"MP5k_StockBttstck",
			"MP5_RailHndgrd"
		}).withSpawnableAttachments(
			(new SSpawnable("ReflexOptic")).withAttachment("Battery9V"),
			(new SSpawnable("UniversalLight")).withAttachment("Battery9V"));
		
		s.build("CZ61");
		s.build("UMP45");
		
		
		//s.build("PP19").withAttachments({
		//	"PP19_Bttstck"
		//});
		
		
		
		
		////////////////////////////////////////////////////////////
		// RIFLES
		s.build("B95").withAttachments({
			"HuntingOptic"
		});
		
		s.build("CZ527").withAttachments({
			"HuntingOptic"
		});
		
		s.build("CZ550").withAttachments({
			"HuntingOptic"
		});
		
		s.build("Winchester70").withAttachments({
			"HuntingOptic"
		});
		
		s.build("Mosin9130").withAttachments({
			"PUScopeOptic",
			"Mosin_Compensator"
		});
		
		s.build("SawedoffMosin9130");
		s.build("Repeater");
		s.build("Ruger1022");
		s.build("SVD");
		s.build("SKS");
		s.build("Izh18");
		s.build("SawedoffIzh18");
		
		
		////////////////////////////////////////////////////////////
		// SHOTGUNS
		s.build("Saiga").withAttachments({
			"Saiga_Bttstck"
		});
		
		s.build("Mp133Shotgun").withSpawnableAttachments(
			(new SSpawnable("FNP45_MRDSOptic")).withAttachment("Battery9V"));
		
		s.build("Izh43Shotgun");
		s.build("SawedoffIzh43Shotgun");
		
		
		
		////////////////////////////////////////////////////////////
		// HANDGUNS
		s.build("Magnum");
		s.build("SawedoffMagnum");
		s.build("MakarovIJ70");
		//s.build("P1");
		//s.build("Derringer");
		s.build("MKII");
		s.build("Colt1911").withAttachments({
			"PistolSuppressor"
		}).withSpawnableAttachments(
			(new SSpawnable("TLRLight")).withAttachment("Battery9V"));
		
		s.build("CZ75").withAttachments({
			"PistolSuppressor"
		}).withSpawnableAttachments(
			(new SSpawnable("TLRLight")).withAttachment("Battery9V"),
			(new SSpawnable("FNP45_MRDSOptic")).withAttachment("Battery9V"));
		
		s.build("FNX45").withAttachments({
			"PistolSuppressor"
		}).withSpawnableAttachments(
			(new SSpawnable("TLRLight")).withAttachment("Battery9V"),
			(new SSpawnable("FNP45_MRDSOptic")).withAttachment("Battery9V"));
		
		s.build("Glock19").withAttachments({
			"PistolSuppressor"
		}).withSpawnableAttachments(
			(new SSpawnable("TLRLight")).withAttachment("Battery9V"),
			(new SSpawnable("FNP45_MRDSOptic")).withAttachment("Battery9V"));
		
		s.build("Deagle").withAttachments({
			"PistolSuppressor",
			"PistolOptic"
		});
		
		s.build("LongHorn").withAttachments({
			"PistolOptic"
		});
		
		vector position = startPosition;
		array<ref SSpawnable> spawnables = s.getBundle();
		foreach(SSpawnable spawnable : spawnables){
			spawnable.spawn(position).collect().SetOrientation("0 0 0");
			position = position + margin;
		}
	}
	
	static void spawnAmmoSet(vector startPosition, vector margin, vector verticalMargin){
		vector position = startPosition;
		
		SSpawnable.build("Mag_STANAG_30Rnd").spawn(position);
		SSpawnable.build("Mag_STANAGCoupled_30Rnd").spawn(position + verticalMargin );

		position = position + margin;
		
		SSpawnable.build("Mag_AK101_30Rnd").spawn(position);		

		position = position + margin;
		
		SSpawnable.build("Mag_AK74_30Rnd").spawn(position);

		position = position + margin;
		
		SSpawnable.build("Mag_AKM_30Rnd").spawn(position);
		SSpawnable.build("Mag_AKM_Drum75Rnd").spawn(position);

		position = position + margin;
		
		SSpawnable.build("Mag_FAL_20Rnd").spawn(position);

		position = position + margin;
		
		SSpawnable.build("Mag_VSS_10Rnd").spawn(position);

		position = position + margin;
		
		SSpawnable.build("Mag_VAL_20Rnd").spawn(position);

		position = position + margin;
		
		SSpawnable.build("Mag_MP5_15Rnd").spawn(position);
		SSpawnable.build("Mag_MP5_30Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		Magazine.Cast(SSpawnable.build("Ammo_308Win").spawn(position).collect()).ServerSetAmmoCount(20);

		position = position + margin;
		
		SSpawnable.build("Mag_CZ527_5rnd").spawn(position);

		position = position + margin;
		
		Magazine.Cast(SSpawnable.build("Ammo_308WinTracer").spawn(position).collect()).ServerSetAmmoCount(20);

		position = position + margin;
		
		Magazine.Cast(SSpawnable.build("Ammo_762x54Tracer").spawn(position).collect()).ServerSetAmmoCount(20);

		position = position + margin;
		
		Magazine.Cast(SSpawnable.build("Ammo_357").spawn(position).collect()).ServerSetAmmoCount(20);

		position = position + margin;
		
		SSpawnable.build("Mag_SVD_10Rnd").spawn(position);

		position = position + margin;
		
		Magazine.Cast(SSpawnable.build("Ammo_762x39Tracer").spawn(position).collect()).ServerSetAmmoCount(20);

		position = position + margin;
		
		Magazine.Cast(SSpawnable.build("Ammo_12gaPellets").spawn(position).collect()).ServerSetAmmoCount(20);
		Magazine.Cast(SSpawnable.build("Ammo_12gaSlug").spawn(position + verticalMargin).collect()).ServerSetAmmoCount(20);
		Magazine.Cast(SSpawnable.build("Ammo_12gaRubberSlug").spawn(position).collect()).ServerSetAmmoCount(20);

		position = position + margin;
		
		SSpawnable.build("Mag_Saiga_5Rnd").spawn(position + verticalMargin);
		SSpawnable.build("Mag_Saiga_8Rnd").spawn(position + verticalMargin);
		SSpawnable.build("Mag_Saiga_Drum20Rnd").spawn(position + verticalMargin * 2);

		position = position + margin;
		
		SSpawnable.build("Mag_IJ70_8Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		SSpawnable.build("Mag_MKII_10Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		SSpawnable.build("Mag_1911_7Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		SSpawnable.build("Mag_CZ75_15Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		SSpawnable.build("Mag_FNX45_15Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		SSpawnable.build("Mag_Glock_15Rnd").spawn(position + verticalMargin);

		position = position + margin;
		
		SSpawnable.build("Mag_Deagle_9rnd").spawn(position + verticalMargin);

		position = position + margin;
	
	}
	
	static void updateMovementSettings(){
		PlayerBase player;
		if(GetGame().IsClient() || !GetGame().IsMultiplayer()) {
			player = PlayerBase.Cast(GetGame().GetPlayer());



		}
		
		if(GetGame().IsServer() || !GetGame().IsMultiplayer()) {
			array<Man> players = new array<Man>;
			GetGame().GetPlayers(players);
			player = PlayerBase.Cast(players[0]);
		}
		
		SHumanCommandMoveSettings hcm = player.GetDayZPlayerType().CommandMoveSettingsW();

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
		player.StartCommand_Move();
	}
	
	
	void updateMouse(){
			
		vector start = GetGame().GetCurrentCameraPosition();
		vector end = start + "1 0 0";
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Debug.RemoveShape, 50, false, Debug.DrawLine(start, end, 0xFF0000));
	}
	
	void updateCrosshair(){
		Weapon_Base weapon = m_weapon;
		vector usti_hlavne_position = weapon.ModelToWorld(weapon.GetSelectionPositionLS( "usti hlavne" ));//usti hlavne
		vector konec_hlavne_position = weapon.ModelToWorld(weapon.GetSelectionPositionLS( "konec hlavne" ));//konec hlavne	
		vector direction = vector.Direction(konec_hlavne_position, usti_hlavne_position );
		
		//SDebug.spawnDebugDot(m_crosshairRaycast.getContactPos(), 0.005, 0.5);
		//SDebug.spawnDebugDot(m_crosshairRaycast.getBegPos(), 0.005, 0.5);
		//SDebug.spawnDebugDot(m_crosshairRaycast.getEndPos(), 0.005, 0.5);
		//SDebug.spawnDebugDot(usti_hlavne_position, 0.005, 0.5);
		//SDebug.spawnDebugDot(konec_hlavne_position, 0.005, 0.5);
		vector from = usti_hlavne_position;
		vector to = konec_hlavne_position + (direction * 100);
		
		m_crosshairRaycast.init(from,to).launch();
		
		
		Debug.DestroyAllShapes();
		Debug.DrawLine(from, m_crosshairRaycast.getContactPosition(), SColor.rgb(0xF00000).getARGB());
		SDebug.spawnDebugDot(m_crosshairRaycast.getContactPosition(), 0.05, 1);
		
	}
	
	void updateBodyClip(){		
		
		vector point = simonvic.GetPosition() + vector.Forward * 2;
		
		vector from = simonvic.GetPosition();
		vector axis = vector.Up;
		float cosAngle = Math.Cos(m_time*Math.PI);
		float sinAngle = Math.Sin(m_time*Math.PI);
		
		
		vector offsetPos = from - point;
		vector result = vector.RotateAroundZero(offsetPos, axis, cosAngle, sinAngle) + from;
		
		
		m_bodyClipRaycast.from(from + "0 1.5 0").to(result + "0 1.5 0").ignore(simonvic).launch();
		
		
		if(bodyClipContactPos_enabled && m_bodyClipRaycast.hasHit())
			SDebug.spawnDebugDot(m_bodyClipRaycast.getContactPosition(), 0.05, 2);
		
		if(bodyClipAllContact_enabled){
			SDebug.spawnDebugDot(m_bodyClipRaycast.getContactPosition(), 0.02, 2);
			SDebug.spawnDebugDot(from, 0.02, 1);
			SDebug.spawnDebugDot(point, 0.02, 1);
		}
		
	}
	
	static void sandbox() {

	}

}
*/