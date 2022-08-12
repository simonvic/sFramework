
class SUserConfigBase : SJSONSerializable{
	//protected float serialVersionUID = 69; //@todo use serialVersionUID instead of checking the fields
	
	#ifndef DEVELOPER
	[NonSerialized()]
	#endif
	protected ref map<string, ref SUserConfigOptionBase> m_options;
	
	void SUserConfigBase() {
		DayZGame.Event_OnRPC.Insert(this.onRPC);
		m_options = new	map<string, ref SUserConfigOptionBase>();
	}
	
	override void load() {
		super.load();
		registerOptions();
	}
	
	/**
	*	@brief Abstract. Get the path of the file on which the default config will be serialized on.
	*	       Valid paths:
	*	        - "$profile:\\path\\to\\your\\file.json"
	*	        - "$saves:\\path\\to\\your\\file.json"
	*/
	string getDefaultPath();
	
	/**
	*	@brief Abstract. Serialization method to be implemented.
	*	       It's needed because of how Enforce handles script variables
	*	 @code
	*	 	override string serialize(){
	*	 		string result;
	*	 		auto cfg = new MyUserConfig();
	*	 		getSerializer().WriteToString(cfg, true, result);
	*	 		return result;
	*	 	}
	*/
	string serializeDefault();
	
	
	void onRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		switch (rpc_type) {
			//case sUDE_RPC.DEBUG: onRPCSyncUserConfigConstraint(ctx); break;
			case SRPCIDs.SYNC_USER_CONFIG_CONSTRAINTS: onConstraintsReceive(ctx); break;
		}
	}
	
	protected void onConstraintsReceive(ParamsReadContext ctx);
	
	/**
	*	@brief Abstract. Apply a set of constraints to the options
	*	 @param constraints \p SUserConfigConstraintsBase - constraints to apply
	*/
	void applyConstraints(SUserConfigConstraintsBase constraints);
	
	/**
	*	@brief Remove all constraints from all options
	*/
	void removeConstraints() {
		foreach (SUserConfigOptionBase option : m_options) {
			option.removeConstraint();
		}
	}
	
	void createDefault() {
		SFileHelper.touch(getDefaultPath());
		SFileHelper.echo(serializeDefault(), getDefaultPath());
	}
		
		
	TStringArray getFields() {
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
		return "m_serializer;m_options";
	}	
	
	bool isValid(){
		return SUserConfigValidator.isValid(getPath(), getFields());
	}
	
	bool isDefaultValid(){
		return SUserConfigValidator.isValid(getDefaultPath(), getFields());
	}
	
	
	///////////////////////////////////////////////////////////////////////
	// OPTIONS
	
	
	/**
	*	@brief This will handle all options registering
	*/
	protected void registerOptions();
	
	/**
	*	@brief Register a new option
	*	 @param optionName \p string - name of the option you want to register. It must be UNIQUE!
	*	 @param option \p SUserConfigOptionBase - option to register
	*/
	protected void registerOption(string optionName, SUserConfigOptionBase option) {
		m_options.Set(optionName, option);
	}
	
	/**
	*	@brief Get an option based on its name.
	*	 @param optionName \p string - Name of the option
	*	 @return SUserConfigOptionBase - option found, NULL otherwise
	*/
	SUserConfigOptionBase getOption(string optionName) {
		return m_options.Get(optionName);
	}
	
	
	
	/**
	*	@brief Get a float option based on its name.
	*	 @param optionName \p string - Name of the option
	*	 @return SUserConfigOption<float> - option found, NULL otherwise
	*/
	SUserConfigOption<float> getOptionFloat(string optionName) {
		return SUserConfigOption<float>.Cast(m_options.Get(optionName));
	}
	
	void getOption(string optionName, out SUserConfigOption<float> option) {
		option = getOptionFloat(optionName);
	}
	
	/**
	*	@brief Get a int option based on its name.
	*	 @param optionName \p string - Name of the option
	*	 @return SUserConfigOption<int> - option found, NULL otherwise
	*/
	SUserConfigOption<int> getOptionInt(string optionName) {
		return SUserConfigOption<int>.Cast(m_options.Get(optionName));
	}
	
	void getOption(string optionName, out SUserConfigOption<int> option) {
		option = getOptionInt(optionName);
	}
	
	/**
	*	@brief Get a bool option based on its name.
	*	 @param optionName \p string - Name of the option
	*	 @return SUserConfigOption<bool> - option found, NULL otherwise
	*/
	SUserConfigOption<bool> getOptionBool(string optionName) {
		return SUserConfigOption<bool>.Cast(m_options.Get(optionName));
	}
	
	void getOption(string optionName, out SUserConfigOption<bool> option) {
		option = getOptionBool(optionName);
	}
	
	/**
	*	@brief Get a float array option based on its name.
	*	 @param optionName \p string - Name of the option
	*	 @return SUserConfigOption<TFloatArray> - option found, NULL otherwise
	*/
	SUserConfigOptionArray<float> getOptionArrayFloat(string optionName) {
		return SUserConfigOptionArray<float>.Cast(m_options.Get(optionName));
	}
	
	void getOption(string optionName, out SUserConfigOptionArray<float> option) {
		option = getOptionArrayFloat(optionName);
	}
	
	
	/**
	*	@brief Update the value of the value of an option
	*	 @param optionName \p string - Name of the option
	*	 @param value \p float - value to assign
	*	 @return float - new value after being assigned to the option
	*/
	float updateOptionValue(string optionName, float value) {
		SUserConfigOption<float> option;
		getOption(optionName, option);
		
		option.set(value);
		return option.get();
	}
	
	/**
	*	@brief Update the value of the value of an option
	*	 @param optionName \p string - Name of the option
	*	 @param value \p bool - value to assign
	*	 @return bool - new value after being assigned to the option
	*/
	bool updateOptionValue(string optionName, bool value) {
		SUserConfigOption<bool> option;
		getOption(optionName, option);
		
		option.set(value);
		return option.get();
	}
	
	
	/**
	*	@brief Shorthand for getting the value of a float option
	*	 @param optionName \p string - Name of the option
	*	 @param value \p float [out] - value of the option
	*	 @return bool \p - true if option value has been set correctly
	*/
	bool get(string optionName, out float value) {
		SUserConfigOption<float> option = SUserConfigOption<float>.Cast(getOption(optionName));
		if (!option) return false;
		
		value = option.get();
		return true;
	}
	
	/**
	*	@brief Shorthand for getting the value of a bool option
	*	 @param optionName \p string - Name of the option
	*	 @param value \p bool [out] - value of the option
	*	 @return bool \p - true if option value has been set correctly
	*/
	bool get(string optionName, out bool value) {
		SUserConfigOption<bool> option = SUserConfigOption<bool>.Cast(getOption(optionName));
		if (!option) return false;
		
		value = option.get();
		return true;
	}
	
	/**
	*	@brief Shorthand for setting the value of a float option
	*	 @param optionName \p string - Name of the option
	*	 @param value \p float - value of the option
	*	 @return bool \p - true if option value has been set correctly
	*/
	bool set(string optionName, float value) {
		SUserConfigOption<float> option = SUserConfigOption<float>.Cast(getOption(optionName));
		if (!option) return false;
		
		option.set(value);
		return true;
	}
	
	/**
	*	@brief Shorthand for setting the value of a bool option
	*	 @param optionName \p string - Name of the option
	*	 @param value \p bool - value of the option
	*	 @return bool \p - true if option value has been set correctly
	*/
	bool set(string optionName, bool value) {
		SUserConfigOption<bool> option = SUserConfigOption<bool>.Cast(getOption(optionName));
		if (!option) return false;
		
		option.set(value);
		return true;
	}
	
	
}