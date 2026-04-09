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
		DayZGame.Event_OnRPC.Insert(this.onRPC);
	}

	void onPreload() {
	}

	void onRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		switch (rpc_type) {
			//case sUDE_RPC.DEBUG: onRPCSyncUserConfigConstraint(ctx); break;
			case SRPCIDs.SYNC_USER_CONFIG_CONSTRAINTS: onConstraintsReceived(ctx); break;
		}
	}

	/**
	 * @brief Invoked when a set of constraints has been received from the server
	 * @param ctx
	 *
	 * When implementing, make sure to invoke super.onConstraintsReceived first, to ensure the
	 * correct order
	 * @code
	 *	override void onConstraintsReceived(ParamsReadContext ctx) {
	 *		super.onConstraintsReceived(ctx);
	 *		YourConstraints constraints;
	 *		if (!ctx.Read(constraints)) {
	 *			SLOG.c("Error!");
	 *			return;
	 *		}
	 *		// ...
	 *	}
	 * @endcode
	 */
	protected void onConstraintsReceived(ParamsReadContext ctx) {
		SLOG.d(""+this, "onConstraintsReceived");
	}

	/**
	 * @brief Load a module config file
	 * @param moduleType Typename of the module to load
	 * @param reload Choose to load even if it's been already loaded
	 * @return loaded module, null on fail
	 */
	SUserConfigBase load(typename moduleType, bool reload = false) {
		if (g_Game.IsDedicatedServer()) {
			SLOG.w("SUserConfig::load", "Trying to load user config from server!, Ignoring...");
			return null;
		}

		if (isModuleLoaded(moduleType) && !reload) {
			return modulesConfigs.Get(moduleType);
		}

		SLOG.i("SUserConfig::load", "Loading " + moduleType);

		//Check if correct typename
		SUserConfigBase moduleCfg = SUserConfigBase.Cast(moduleType.Spawn());
		if (!moduleCfg) {
			SLOG.e("SUserConfig::load", "Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....");
			return null;
		}

		// Load config
		if (!moduleCfg.load()) {
			string path = moduleCfg.getPath();
			SLOG.w(""+this, "Couldn't load user config [ " + path + " ]");
			SLOG.i(1, "Creating " + moduleCfg.Type() + " config file : " + path);
			moduleCfg.save();
			SLOG.i(2, "Done");
		}

		modulesConfigs.Set(moduleType, moduleCfg);
		SLOG.i(1, "Loaded user config: " + moduleCfg);
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
