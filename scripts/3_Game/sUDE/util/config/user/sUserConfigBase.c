
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
		for (int i = 0; i<Type().GetVariableCount(); i++) {
			if (Type().GetVariableName(i) != "m_serializer") { //@todo lol... find a solution for [NonSerialized()]
				fields.Insert(Type().GetVariableName(i));
			}
		}
		return fields;
	}
	
	bool isValid(){
		return SUserConfigValidator.isValid(getPath(), getFields());
	}
	
	bool isDefaultValid(){
		return SUserConfigValidator.isValid(getDefaultPath(), getFields());
	}
	
}