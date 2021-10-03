
class SUserConfigBase : SJSONSerializable{
	
	void SUserConfigBase() {
		DayZGame.Event_OnRPC.Insert(this.onRPC);
	}
	
	void onRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx);
	
	//protected float serialVersionUID = 69; //@todo use serialVersionUID instead of checking the fields
	
	string getDefaultPath();
	string serializeDefault();
	
	void createDefault(){
		SFileHelper.touch(getDefaultPath());
		SFileHelper.echo(serializeDefault(), getDefaultPath());
	}
		
		
	TStringArray getFields(){
		TStringArray fields = new TStringArray;
		TStringArray nonSerializedFields = new TStringArray;
		getNonSerializedFields().Split(";",nonSerializedFields);
		
		for (int i = 0; i<Type().GetVariableCount(); i++) {
			if (nonSerializedFields.Find(Type().GetVariableName(i)) == -1) { 
				fields.Insert(Type().GetVariableName(i));
			}
		}
		
		return fields;
	}
		
	/**
	*	@brief Get a semicolon separated list of fields that must not be serialized. Little hacky to circumvent NonSerialized() attribute
	*	 @return string - list of fields not to serialize
	*/
	//@todo lol... find a solution for [NonSerialized()]
	string getNonSerializedFields() {
		return "m_serializer";
	}	
	
	bool isValid(){
		return SUserConfigValidator.isValid(getPath(), getFields());
	}
	
	bool isDefaultValid(){
		return SUserConfigValidator.isValid(getDefaultPath(), getFields());
	}
	
}