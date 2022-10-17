/**
*	@brief Abstract. Base class to constrain a variable given a certain constraint
*	       A Param has been used to allow a Generic constraint to
*	       be extended from other Generic constraints
*/
class SConstraintBase : Managed {
	
	protected bool constrain = false;
	protected string message = "#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED";
				
	bool isEnabled() {
		return constrain;
	}
	
	void setEnabled(bool enable) {
		constrain = enable;
	}
	
	void enable() {
		setEnabled(true);
	}
	
	void disable() {
		setEnabled(false);
	}
	
	string getMessage() {
		return message;
	}
	
	//@todo move this, it doesn't belong here
	string toString();
	
	void setMessage(string msg) {
		message = msg;
	}
	
	
	/**
	*	@brief If the constraint is enabled, constrain the given value
	*	 @param param \p Param - A generic param holding the value to constrain
	*/
	void constrain(Param toConstrain) {
		if (isEnabled()) {
			performConstrain(toConstrain);
		}
	}
	
	/**
	*	@brief Abstract. Perform the actual constrain.
	*	 @param param \p Param - A generic param holding the value to constrain
	*/
	void performConstrain(Param param);
	
	/**
	*	@brief Check if a value respects the constraint
	*	 @param param \p Param - A generic param holding the value to constrain
	*	 @return bool - if the value respect the constraint, true if the constrain is disabled
	*/
	bool isValid(Param param) {
		if (!isEnabled()) return true;
		return performValidation(param);
	}
	
	/**
	*	@brief Abstract. Perform the actual validation.
	*	 @param param \p Param - A generic param holding the value to constrain
	*	 @return bool - if the value respect the constraint
	*/
	bool performValidation(Param param);
}