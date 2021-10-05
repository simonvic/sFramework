/**
*	@brief Abstract. Base class to constrain a variable given a certain constraint
*	       A Param has been used to allow a Generic constraint to
*	       be extended from other Generic constraints
*/
class SConstraintBase : Managed {
	
	protected bool enabled = false;
	protected string message = "#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED";
				
	bool isEnabled() {
		return enabled;
	}
	
	void setEnabled(bool enable) {
		enabled = enable;
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
	*	@brief Get the constrained value
	*	 @param param \p Param - A generic param holding the value to constrain
	*	 @return Param - Param containing constrained value, or non-constrained value if the constraint is disabled
	*/
	Param constrain(Param param) {
		if (!isEnabled()) return param;	
		return performConstrain(param);
	}

	/**
	*	@brief Abstract. Perform the actual constrain.
	*	 @param param \p Param - A generic param holding the value to constrain
	*	 @return Param - Param containing constrained value
	*/
	Param performConstrain(Param param);
	
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