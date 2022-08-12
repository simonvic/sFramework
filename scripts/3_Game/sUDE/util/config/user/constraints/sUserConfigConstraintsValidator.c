class SUserConfigConstraintsValidator : Managed {
	
	
	/**
	*	@brief Check if the UserConfigConstraints is valid (exists and can be deserialized)
	*	 @param path \p string - File path of UserConfigConstraints to validate
	*	 @param fields \p TStringArray - Fields to test with
	*	 @return if it's valid
	*/
	static bool isValid(string path){		
		string stringData = SFileHelper.cat(path);
		
		if(stringData == string.Empty) return false;
		
		string deserializationError;
		if(!canBeDeserialized(stringData, deserializationError)) {
			SLog.e("Error during deserialization of [ " + path + " ] : \n" + deserializationError,"SUserConfigConstraintsValidator::isValid",1);
			return false;
		}
				
		return true;
	
	}
	
	/**
	*	@brief Check if the UserConfigConstraints can be deserialized (json)
	*	 @param stringData \p string - string to be parsed (json)
	*	 @param deserializationError \p string - error output of deserialization
	*/
	static bool canBeDeserialized(string stringData, out string deserializationError){
		JsonSerializer serializer = new JsonSerializer;
		SUserConfigConstraintsBase data = new SUserConfigConstraintsBase;
		return serializer.ReadFromString( data, stringData, deserializationError );
	}

}