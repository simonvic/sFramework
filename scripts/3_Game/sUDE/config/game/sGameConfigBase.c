class SGameConfigBase{
			
	string getModulePath(){
		return SGameConfig.getCfgName() + " " + getModuleName();
	}
	
	string getModuleName(){
		return "";
	}
	
	bool getIsEnabled(){
		return SGameConfig.getBool(getModulePath() + " enabled"); //@todo add check if module is not present
	}
}