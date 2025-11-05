class SUserConfig {
	
	private static ref SUserConfig INSTANCE;
	static SUserConfig getInstance() {
		if (!INSTANCE) INSTANCE = new SUserConfig();
		return INSTANCE;
	}

	protected ref map<typename, ref SUserConfigBase> modulesConfigs;

	private void SUserConfig() {
		modulesConfigs = new map<typename, ref SUserConfigBase>();
	}
	
	/**
	*	@brief Load a module config file
	*	 @param moduleType typename - Typename of the module to load
	*	 @param reload bool - Choose to load even if it's been already loaded
	*/
	void load(typename moduleType, bool reload = false) {
		SLog.i("Loading " + moduleType, "SUserConfig::load");

		if (GetGame().IsDedicatedServer()) {
			SLog.w("Trying to load user config from server!, Ignoring...","SUserConfig::load");
			return;
		}
		
		if (isModuleLoaded(moduleType) && !reload) return;
		
		//Check if correct typename
		SUserConfigBase moduleCfg = SUserConfigBase.Cast(moduleType.Spawn());
		if (!moduleCfg) {
			SLog.e("Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....","SUserConfig::load");
			return;
		}

		// Load config
		if (!moduleCfg.load()) {
			string path = moduleCfg.getPath();
			SLog.w("Couldn't load user config [ " + path + " ]", "SUserConfig");
			SLog.i("Creating " + moduleCfg.Type() + " config file : " + path, "", 1);
			moduleCfg.save();
			SLog.i("Done", "", 2);
		}

		modulesConfigs.Set(moduleType, moduleCfg);
		SLog.i("Loaded user config: " + moduleCfg, ""+this);
	}
	
	protected bool isModuleLoaded(typename module) {
		return modulesConfigs.Contains(module);
	}
	
	// TODO: should return immutable view
	map<typename, ref SUserConfigBase> getLoadedModules() {
		return modulesConfigs;
	}
	
}
