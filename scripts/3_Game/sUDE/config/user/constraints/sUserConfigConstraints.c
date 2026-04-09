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
		if (g_Game.IsClient()) {
			SLOG.w("SUserConfigConstraints::load", "Trying to load user config constraints from client!, Ignoring...");
			return null;
		}

		if (isModuleLoaded(moduleType) && !reload) {
			return modulesCfgConstraints.Get(moduleType);
		}

		SLOG.i("SUserConfigConstraints::load", "Loading " + moduleType);

		// Check if correct typename
		SUserConfigConstraintsBase moduleCfgConstraints = SUserConfigConstraintsBase.Cast(moduleType.Spawn());
		if (!moduleCfgConstraints) {
			SLOG.e("SUserConfigConstraints::load", "Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....");
			return null;
		}

		// Load config
		if (!moduleCfgConstraints.load()) {
			string path = moduleCfgConstraints.getPath();
			SLOG.w(""+this, "Couldn't load user config constraints [ " + path + " ]");
			SLOG.i(1, "Creating " + moduleCfgConstraints.Type() + " constraints file : " + path);
			moduleCfgConstraints.save();
			SLOG.i(2, "Done");
		}
		modulesCfgConstraints.Set(moduleType, moduleCfgConstraints);
		SLOG.i(1, "Loaded user config constraints: " + moduleCfgConstraints);
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
