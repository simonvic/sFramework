/**
 * @brief A set of options and preferences for users to customize
 */
class SUserConfigBase : SJSONSerializable {

	#ifndef DEVELOPER
	[NonSerialized()]
	#endif
	protected ref map<string, ref SUserConfigOptionBase> m_options;

	void SUserConfigBase() {
		DayZGame.Event_OnRPC.Insert(this.onRPC);
		m_options = new	map<string, ref SUserConfigOptionBase>();
		// TODO: what if we receive RPC before load()?
	}

	override bool load() {
		bool success = super.load();
		registerOptions();
		return success;
	}

	void onRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) {
		switch (rpc_type) {
			//case sUDE_RPC.DEBUG: onRPCSyncUserConfigConstraint(ctx); break;
			case SRPCIDs.SYNC_USER_CONFIG_CONSTRAINTS: onConstraintsReceive(ctx); break;
		}
	}

	/**
	 * @brief Invoked when a set of constraints has been received from the server
	 * @param ctx
	 */
	protected void onConstraintsReceive(ParamsReadContext ctx);

	/**
	 * @brief Apply a set of constraints to the options
	 * @param constraints to apply
	 */
	void applyConstraints(SUserConfigConstraintsBase constraints);

	/**
	 * @brief Remove all constraints from all options
	 */
	void removeConstraints() {
		foreach (SUserConfigOptionBase option : m_options) {
			option.removeConstraint();
		}
	}

	///////////////////////////////////////////////////////////////////////
	// OPTIONS

	/**
	 * @brief Perform options registration.
	 * To be implemented with options registration
	 */
	protected void registerOptions();

	/**
	 * @brief Register a new option
	 * @param optionName name of the option you want to register. It must be UNIQUE!
	 * @param option to register
	 */
	protected void registerOption(string optionName, SUserConfigOptionBase option) {
		m_options.Set(optionName, option);
	}

	/**
	 * @brief Get an option based on its name.
	 * @param optionName name of the option
	 * @return option found, NULL otherwise
	 */
	SUserConfigOptionBase getOption(string optionName) {
		return m_options.Get(optionName);
	}

	/**
	 * @brief Get a float option based on its name.
	 * @param optionName
	 * @return option found, null otherwise
	 */
	SUserConfigOption<float> getOptionFloat(string optionName) {
		return SUserConfigOption<float>.Cast(m_options.Get(optionName));
	}

	void getOption(string optionName, out SUserConfigOption<float> option) {
		option = getOptionFloat(optionName);
	}

	/**
	 * @brief Get a int option based on its name.
	 * @param optionName
	 * @return option found, null otherwise
	 */
	SUserConfigOption<int> getOptionInt(string optionName) {
		return SUserConfigOption<int>.Cast(m_options.Get(optionName));
	}

	void getOption(string optionName, out SUserConfigOption<int> option) {
		option = getOptionInt(optionName);
	}

	/**
	 * @brief Get a boolean option based on its name.
	 * @param optionName
	 * @return option found, null otherwise
	 */
	SUserConfigOption<bool> getOptionBool(string optionName) {
		return SUserConfigOption<bool>.Cast(m_options.Get(optionName));
	}

	void getOption(string optionName, out SUserConfigOption<bool> option) {
		option = getOptionBool(optionName);
	}
	
	/**
	 * @brief Get a float array option based on its name.
	 * @param optionName
	 * @return option found, null otherwise
	 */
	SUserConfigOptionArray<float> getOptionArrayFloat(string optionName) {
		return SUserConfigOptionArray<float>.Cast(m_options.Get(optionName));
	}

	void getOption(string optionName, out SUserConfigOptionArray<float> option) {
		option = getOptionArrayFloat(optionName);
	}

	/**
	 * @brief Update the value of the value of an option
	 * @param optionName
	 * @param value
	 * @return new value after being assigned to the option 
	 */
	float updateOptionValue(string optionName, float value) {
		SUserConfigOption<float> option;
		getOption(optionName, option);

		option.set(value);
		return option.get();
	}

	/**
	 * @brief Update the value of the value of an option
	 * @param optionName
	 * @param value
	 * @return new value after being assigned to the option
	 */
	bool updateOptionValue(string optionName, bool value) {
		SUserConfigOption<bool> option;
		getOption(optionName, option);

		option.set(value);
		return option.get();
	}

	/**
	 * @brief Shorthand for getting the value of a float option
	 * @param optionName
	 * @param value
	 * @return true if option value has been set correctly
	 */
	bool get(string optionName, out float value) {
		SUserConfigOption<float> option = SUserConfigOption<float>.Cast(getOption(optionName));
		if (!option) return false;

		value = option.get();
		return true;
	}

	/**
	 * @brief Shorthand for getting the value of a bool option
	 * @param optionName
	 * @param value
	 * @return true if option value has been set correctly
	 */
	bool get(string optionName, out bool value) {
		SUserConfigOption<bool> option = SUserConfigOption<bool>.Cast(getOption(optionName));
		if (!option) return false;

		value = option.get();
		return true;
	}

	/**
	 * @brief Shorthand for setting the value of a float option
	 * @param optionName
	 * @param value
	 * @return true if option value has been set correctly
	 */
	bool set(string optionName, float value) {
		SUserConfigOption<float> option = SUserConfigOption<float>.Cast(getOption(optionName));
		if (!option) return false;

		option.set(value);
		return true;
	}

	/**
	 * @brief Shorthand for setting the value of a bool option
	 * @param optionName
	 * @param value
	 * @return true if option value has been set correctly
	 */
	bool set(string optionName, bool value) {
		SUserConfigOption<bool> option = SUserConfigOption<bool>.Cast(getOption(optionName));
		if (!option) return false;

		option.set(value);
		return true;
	}

}
