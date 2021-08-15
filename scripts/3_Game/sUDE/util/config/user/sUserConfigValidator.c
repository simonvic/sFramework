class SUserConfigValidator : Managed{
	
	//@todo why not just a damn serial UID >:(
	
	/**
	*	@brief Check if the UserConfig is valid (exists, can be deserialized and it has the required fields)
	*	 @param path \p string - File path of UserConfig to validate
	*	 @param fields \p TStringArray - Fields to test with
	*	 @return if it's valid
	*/
	static bool isValid(string path, TStringArray fields){		
		string stringData = SFileHelper.cat(path);
		
		if(stringData == string.Empty) return false;
		
		string deserializationError;
		if(!canBeDeserialized(stringData, deserializationError)) {
			SLog.e("Error during deserialization of [ " + path + " ] : \n" + deserializationError,"SUserConfigValidator::isValid",1);
			return false;
		}
		
		if(!containsCorrectFields(stringData, fields)){
			SLog.e("The file [ " + path + " ] doesn't contain required fields","SUserConfigValidator::isValid",1);
			return false;
		}
		
		return true;
	
	}
	
	/**
	*	@brief Check if the UserConfig can be deserialized (json)
	*	 @param stringData \p string - string to be parsed (json)
	*	 @param deserializationError \p string - error output of deserialization
	*/
	static bool canBeDeserialized(string stringData, out string deserializationError){
		JsonSerializer serializer = new JsonSerializer;
		SUserConfigBase data = new SUserConfigBase;
		return serializer.ReadFromString( data, stringData, deserializationError );
	}
	
	/**
	*	@brief Check if the UserConfig contains all the required fields
	*	 @param stringData \p string - string to be parsed (json)
	*	 @param fields \p TStringArray - Fields to test with
	*/
	static bool containsCorrectFields(string stringData, TStringArray fields){
		foreach(string field : fields){
			if(!stringData.Contains("\"" + field + "\"")) return false;
		}
		return true;
	}
}