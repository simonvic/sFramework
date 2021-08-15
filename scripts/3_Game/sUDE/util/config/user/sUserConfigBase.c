
class SUserConfigBase : Managed{
	
	protected static ref JsonSerializer m_serializer = new JsonSerializer;
	
	//protected float serialVersionUID = 69; //@todo use serialVersionUID instead of checking the fields
	
	string getPath(){
		return "";
	}
	
	string getDefaultPath(){
		return "";
	}
	
	void deserialize(string data, out string error){}
	
	string serialize(bool serializeDefault = false){
		return "";
	}
	
	void load(){
		string data = SFileHelper.cat(getPath());
		if(data == string.Empty) return;
		
		string error;
		deserialize(data, error);
		if(error != string.Empty){
			SLog.e(error);
		}
	}		
	
	void save(){
		SFileHelper.touch(getPath());
		SFileHelper.echo(serialize(), getPath());
	}	
		
	void createDefault(){
		SFileHelper.touch(getDefaultPath());
		SFileHelper.echo(serialize(true), getDefaultPath());
	}
		
		
	TStringArray getFields(){
		TStringArray fields = new TStringArray;
		for(int i = 0; i<Type().GetVariableCount(); i++){
			if(Type().GetVariableName(i) != "m_serializer"){ //@todo lol... find a solution for [NonSerialized()]
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