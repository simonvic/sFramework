typedef map<typename, ref SUserConfigBase> TSUserConfigModules;

/*
*	@Singleton
*/
class SUserConfig{
	
	private static ref SUserConfig INSTANCE = new SUserConfig();
	private void SUserConfig(){}
	static SUserConfig getInstance(){
		return INSTANCE;
	}
	
	protected ref TSUserConfigModules modulesConfigs = new TSUserConfigModules;
		
	/**
	*	@brief Load a module config file
	*	 @param moduleType typename - Typename of the module to load
	*	 @param reload bool - Choose to load even if it's been already loaded
	*/
	void load(typename moduleType, bool reload = false){
		
		if (GetGame().IsServer() && GetGame().IsMultiplayer()) {
			SLog.w("Trying to load user config from server!, Ignoring...","SUserConfig::load");
			return;
		}
		
		if(isModuleLoaded(moduleType) && !reload) return;
		
		//Check if correct typename
		SUserConfigBase moduleCfg = SUserConfigBase.Cast(moduleType.Spawn());
		if(!moduleCfg){
			SLog.e("Error while loading < " + moduleType + " > Maybe not a module type?. Ignoring....","SUserConfig::load");
			return;
		}

		SLog.i("Loading " + moduleType,"SUserConfig::load");
		validateModuleCfgFile(moduleCfg);
		
		// Load config
		moduleCfg.load();
		modulesConfigs.Set(moduleType, moduleCfg);
		SLog.i("Done -> " + moduleCfg,"",1);
	}
	
	/**
	*	@brief Validate a module config file. Copy the default if not present; create default file if also not present
	*	 @param moduleCfg SUserConfigBase - Module to validate
	*/
	protected void validateModuleCfgFile(SUserConfigBase moduleCfg){		
		if(!moduleCfg.isValid()){
			string path = moduleCfg.getPath();
			string defaultPath = moduleCfg.getDefaultPath();
			if(moduleCfg.isDefaultValid()){
				CopyFile(defaultPath, path);
			}else{
				SLog.w("Couldn't load neither user config [ " + path + " ] nor default config [ " + defaultPath + " ]", "SUserConfig");
				SLog.i("Creating " + moduleCfg.Type() + " default config file : " + defaultPath,"SUserConfig",1);
				moduleCfg.createDefault();
				SLog.i("Done","",2);
				SLog.i("Creating " + moduleCfg.Type() + " config file : " + path,"SUserConfig",1);
				moduleCfg.save();
				SLog.i("Done","",2);				
			}
		}
	}
	
	protected bool isModuleLoaded(typename module){
		return modulesConfigs.Contains(module);
	}
	
	/**
	*	@brief Save modules configuration
	*/
	void save(){
		foreach(SUserConfigBase module : modulesConfigs){
			module.save();
		}
	}
	
	
	bool isValid(){
		foreach(SUserConfigBase module : modulesConfigs){
			if(!module.isValid()) return false;
		}
		return true;
	}
	
	void printLoadedModules(){
		foreach(SUserConfigBase module : modulesConfigs){
			SLog.d(module);
		}
	}
	
}