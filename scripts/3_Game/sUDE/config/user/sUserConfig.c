/**
 * @brief A "bank" of user configuration modules
 */
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
	 * @brief Load a module config file
	 * @param moduleType Typename of the module to load
	 * @param reload Choose to load even if it's been already loaded
	 * @return loaded module, null on fail
	 */
	SUserConfigBase load(typename moduleType, bool reload = false) {
		if (GetGame().IsDedicatedServer()) {
			SLog.w("Trying to load user config from server!, Ignoring...","SUserConfig::load");
			return null;
		}

		if (isModuleLoaded(moduleType) && !reload) {
			return modulesConfigs.Get(moduleType);
		}

		SLog.i("Loading " + moduleType, "SUserConfig::load");

		//Check if correct typename
		SUserConfigBase moduleCfg = SUserConfigBase.Cast(moduleType.Spawn());
		if (!moduleCfg) {
			SLog.e("Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....","SUserConfig::load");
			return null;
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
		return moduleCfg;
	}

	/**
	 * @brief Check if a module has been loaded
	 * @param module typename
	 * @return true if loaded, false otherwise
	 */
	protected bool isModuleLoaded(typename module) {
		return modulesConfigs.Contains(module);
	}

	// TODO: should return immutable view
	map<typename, ref SUserConfigBase> getLoadedModules() {
		return modulesConfigs;
	}

}
