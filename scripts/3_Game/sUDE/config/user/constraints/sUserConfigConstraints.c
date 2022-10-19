typedef map<typename, ref SUserConfigConstraintsBase> TSUserConfigConstraintsModules;

//@todo do proper IsClient/IsServer checks

/*
*	@Singleton
*/
class SUserConfigConstraints {
	
	private static ref SUserConfigConstraints INSTANCE;
	static SUserConfigConstraints getInstance() {
		if (!INSTANCE) INSTANCE = new SUserConfigConstraints();
		return INSTANCE;
	}

	private void SUserConfigConstraints() {
		modulesCfgConstraints = new TSUserConfigConstraintsModules;
		m_syncRPC = new SyncSUserConfigConstraintsRPC();
	}
	

	protected ref TSUserConfigConstraintsModules modulesCfgConstraints;
	protected ref SyncSUserConfigConstraintsRPC m_syncRPC;
	

	void onPreload() {
	}

	/**
	*	@brief Load a module config constraint file
	*	 @param moduleType typename - Typename of the module to load
	*	 @param reload bool - Choose to load even if it's been already loaded
	*/
	void load(typename moduleType, bool reload = false) {
		
		if (GetGame().IsClient()) {
			SLog.w("Trying to load user config constraints from client!, Ignoring...","SUserConfigConstraints::load");
			return;
		}
		
		if (isModuleLoaded(moduleType) && !reload) return;
		
		//Check if correct typename
		SUserConfigConstraintsBase moduleCfgConstraints = SUserConfigConstraintsBase.Cast(moduleType.Spawn());
		if (!moduleCfgConstraints) {
			SLog.e("Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....","SUserConfigConstraints::load");
			return;
		}

		SLog.i("Loading " + moduleType,"SUserConfigConstraints::load");
		validateModuleCfgFile(moduleCfgConstraints);
		
		// Load config
		moduleCfgConstraints.load();
		modulesCfgConstraints.Set(moduleType, moduleCfgConstraints);
		SLog.i("Done -> " + moduleCfgConstraints,"",1);
	}
	
	/**
	*	@brief Validate a module config constraint file. Copy the default if not present; create default file if also not present
	*	 @param moduleCfgConstraints SUserConfigConstraintsBase - Module to validate
	*/
	protected void validateModuleCfgFile(SUserConfigConstraintsBase moduleCfgConstraints) {		
		
		if (moduleCfgConstraints.isValid()) return;

		string path = moduleCfgConstraints.getPath();
		SLog.w("Couldn't load neither user config constraint [ " + path + " ]", "SUserConfigConstraints");
		SLog.i("Creating " + moduleCfgConstraints.Type() + " config constraint file : " + path,"SUserConfigConstraints",1);
		moduleCfgConstraints.save();
		SLog.i("Done","",2);
	}
	
	protected bool isModuleLoaded(typename moduleCfgConstraints) {
		return modulesCfgConstraints.Contains(moduleCfgConstraints);
	}
	
	/**
	*	@brief Save modules configuration
	*/
	void save() {
		foreach (SUserConfigConstraintsBase moduleCfgConstraints : modulesCfgConstraints) {
			moduleCfgConstraints.save();
		}
	}
	
	
	bool isValid() {
		foreach (SUserConfigConstraintsBase moduleCfgConstraints : modulesCfgConstraints) {
			if (!moduleCfgConstraints.isValid()) return false;
		}
		return true;
	}
	
	void printLoadedModules() {
		foreach (SUserConfigConstraintsBase moduleCfgConstraints : modulesCfgConstraints) {
			SLog.d(moduleCfgConstraints);
		}
	}
	
	TSUserConfigConstraintsModules getLoadedModules() {
		return modulesCfgConstraints;
	}
	
	
	void syncWithEverybody() {
		syncWith(null);
	}
	
	void syncWith(PlayerIdentity playerIdentity) {
		m_syncRPC.setup();
		m_syncRPC.sendTo(playerIdentity);
	}
	
}