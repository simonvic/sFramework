modded class MissionGameplay{
	
	void MissionGameplay() {
		SPPEManager.onInit();		
	}
	

	void ~MissionGameplay() {
		resetSCameraOverlays();
		resetSPPE();
		resetSUserConfigConstraints();
	}
	
	protected void resetSCameraOverlays() {
		SCameraOverlaysManager.getInstance().deactivateAll();
	}
	
	protected void resetSPPE() {
		SPPEManager.deactivateAll();
		SPPEManager.applyDefault();
	}
	
	protected void resetSUserConfigConstraints() {
		TSUserConfigModules loadedModules = SUserConfig.getInstance().getLoadedModules();
		foreach (SUserConfigBase cfg : loadedModules) {
			cfg.removeConstraints();
		}
	}
	
	/*
	override void OnInit() {
		super.OnInit();
	}
	
	override void OnUpdate(float timeslice) {
		super.OnUpdate(timeslice);
	}
	

	override void OnKeyPress(int key) {
		super.OnKeyPress(key);
		PluginSDebug.Cast(GetPluginManager().GetPluginByType(PluginSDebug)).onKeyPress(key);
	}
	*/
}