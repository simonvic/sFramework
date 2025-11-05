class SJSONSerializable : Managed {
	
	#ifndef DEVELOPER
	[NonSerialized()]
	#endif
	protected static ref JsonSerializer m_serializer = new JsonSerializer();
	
	/**
	*	@brief Abstract. Get the path of the file that must be serialized on.
	*	       Valid paths:
	*	        - "$profile:\\path\\to\\your\\file.json"
	*	          for clients: AppData\Local\DayZ\path\to\your\file.json
	*	          for server:  YOUR_SERVER_PROFILE_FOLDER\path\to\your\file.json
	*
	*	        - "$saves:\\path\\to\\your\\file.json"
	*	          for clients: Documents\DayZ\path\to\your\file.json
	*	          for server:  YOUR_SERVER_PROFILE_FOLDER\Users\Server\path\to\your\file.json
	*
	*	        - "$mission:\\path\\to\\your\\file.json"
	*	          for server:  YOUR_SERVER_INSTALLATION_PATH\CURRENT_MISSION\path\to\your\file.json  
	*/
	string getPath();
	
	
	/**
	*	@brief Abstract. Deserialization method to be implemented.
	*	       It's needed because of how Enforce handles script variables
	*	 @code
	*	 	override bool deserialize(string data, out string error) {
	*			auto thiz = this;
	*			return getSerializer().ReadFromString(thiz, data, error);
	*		}
	*/
	bool deserialize(string data, out string error);
	
	
	/**
	*	@brief Abstract. Serialization method to be implemented.
	*	       It's needed because of how Enforce handles script variables
	*	 @code
	*	 	override bool serialize(out string result) {
	*	 		auto thiz = this;
	*	 		return getSerializer().WriteToString(thiz, true, result);
	*	 	}
	*/
	bool serialize(out string result);

	/**
	*	@brief Deserialize the file and load it
	*	@return true if loaded and deserialized correctly, false otherwise
	*/
	bool load() {
		string data = SFileHelper.cat(getPath());
		string error;
		if (!deserialize(data, error)) {
			SLog.e("JSON deserialization error: " + error, "SJSONSerializable");
			return false;
		}
		return true;
	}
	
	/**
	 *	@brief Serialize the object and write it
	 *	@return true if succesful, false otherwise
	 */
	bool save() {
		SFileHelper.touch(getPath());
		string data;
		if (!serialize(data)) {
			SLog.e("JSON serialization error", "SJSONSerializable");
			return false;
		}
		SFileHelper.echo(data, getPath());
		return true;
	}
	
	
	protected JsonSerializer getSerializer() {
		return m_serializer;
	}
	
}
