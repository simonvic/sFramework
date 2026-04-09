modded class MissionGameplay {

	override void OnMissionFinish() {
		super.OnMissionFinish();
		resetSCameraOverlays();
		resetSUserConfigConstraints();
	}

	protected void resetSCameraOverlays() {
		SCameraOverlaysManager.getInstance().deactivateAll();
	}

	protected void resetSUserConfigConstraints() {
		map<typename, ref SUserConfigBase> loadedModules = SUserConfig.getInstance().getLoadedModules();
		foreach (SUserConfigBase cfg : loadedModules) {
			cfg.removeConstraints();
		}
	}

}
