class SGameConfigBase : Managed {
	
	string getModulePath() {
		return SGameConfig.getCfgName() + " " + getModuleName();
	}
	
	string getModuleName() {
		return "";
	}
	
	bool isModuleEnabled() {
		return SGameConfig.getBool(getModulePath() + " enabled"); //@todo add check if module is not present
	}
}