class SJSONSerializable : Managed {
	
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
	*	 	override void deserialize(string data, out string error){
	*			auto cfg = this;
	*			getSerializer().ReadFromString(cfg, data, error);
	*		}
	*/
	void deserialize(string data, out string error);
	
	
	/**
	*	@brief Abstract. Serialization method to be implemented.
	*	       It's needed because of how Enforce handles script variables
	*	 @code
	*	 	override string serialize(){
	*	 		string result;
	*	 		auto cfg = this;
	*	 		getSerializer().WriteToString(cfg, true, result);
	*	 		return result;
	*	 	}
	*/
	string serialize();
	
	
	/**
	*	@brief Deserialize the file and load it
	*/
	void load(){
		string data = SFileHelper.cat(getPath());
		if(data == string.Empty) return;
		
		string error;
		deserialize(data, error);
		if(error != string.Empty){
			SLog.e(error);
		}
	}
	
	/**
	*	@brief Serialize the object and write it
	*/
	void save(){
		SFileHelper.touch(getPath());
		SFileHelper.echo(serialize(), getPath());
	}
	
	
	protected JsonSerializer getSerializer() {
		return m_serializer;
	}
	
}