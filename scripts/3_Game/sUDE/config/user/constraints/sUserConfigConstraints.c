/**
 * @brief A "bank" of user configuration constraints
 */
class SUserConfigConstraints {

	private static ref SUserConfigConstraints INSTANCE;
	static SUserConfigConstraints getInstance() {
		if (!INSTANCE) INSTANCE = new SUserConfigConstraints();
		return INSTANCE;
	}

	protected ref map<typename, ref SUserConfigConstraintsBase> modulesCfgConstraints;
	protected ref SyncSUserConfigConstraintsRPC m_syncRPC;

	private void SUserConfigConstraints() {
		modulesCfgConstraints = new map<typename, ref SUserConfigConstraintsBase>();
		m_syncRPC = new SyncSUserConfigConstraintsRPC();
	}

	void onPreload() {
	}

	/**
	 * @brief Load a user config constraint file
	 * @param moduleType typename of the module to load
	 * @param reload bool - Choose to load even if it's been already loaded
	 * @return loaded module, null on fail
	 */
	SUserConfigConstraintsBase load(typename moduleType, bool reload = false) {
		SLog.i("Loading " + moduleType, "SUserConfigConstraints::load");

		if (GetGame().IsClient()) {
			SLog.w("Trying to load user config constraints from client!, Ignoring...","SUserConfigConstraints::load");
			return null;
		}

		if (isModuleLoaded(moduleType) && !reload) {
			return null;
		}

		// Check if correct typename
		SUserConfigConstraintsBase moduleCfgConstraints = SUserConfigConstraintsBase.Cast(moduleType.Spawn());
		if (!moduleCfgConstraints) {
			SLog.e("Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....","SUserConfigConstraints::load");
			return null;
		}

		// Load config
		if (!moduleCfgConstraints.load()) {
			string path = moduleCfgConstraints.getPath();
			SLog.w("Couldn't load user config constraints [ " + path + " ]", "SUserConfigConstraints");
			SLog.i("Creating " + moduleCfgConstraints.Type() + " constraints file : " + path, "", 1);
			moduleCfgConstraints.save();
			SLog.i("Done", "", 2);
		}
		modulesCfgConstraints.Set(moduleType, moduleCfgConstraints);
		SLog.i("Loaded user config constraints: " + moduleCfgConstraints);
		return moduleCfgConstraints;
	}

	protected bool isModuleLoaded(typename moduleCfgConstraints) {
		return modulesCfgConstraints.Contains(moduleCfgConstraints);
	}

	map<typename, ref SUserConfigConstraintsBase> getLoadedModules() {
		return modulesCfgConstraints;
	}

	/**
	 * @brief Send the constraints to all clients
	 */
	void syncWithEverybody() {
		syncWith(null);
	}

	/**
	 * @brief Send the constraints to a client
	 * @param playerIdentity of the client; if null, send to all players
	 */
	void syncWith(PlayerIdentity playerIdentity) {
		m_syncRPC.setup();
		m_syncRPC.sendTo(playerIdentity);
	}

}
