class SUserConfigConstraintsBase : SJSONSerializable {
	
	bool isValid(){
		return SUserConfigConstraintsValidator.isValid(getPath());
	}
	
}