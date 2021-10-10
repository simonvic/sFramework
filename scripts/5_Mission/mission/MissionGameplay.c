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
}