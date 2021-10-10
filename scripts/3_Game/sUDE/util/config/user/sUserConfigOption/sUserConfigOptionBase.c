class SUserConfigOptionBase : Managed {
	
	protected ref Param m_param;
	protected ref SConstraintBase m_constraint;
	protected ref SUserConfigOptionInfo m_info;
	
	Param getParam() {
		return m_param;
	}
	
	/**
	*	@brief Set the option value and constrain it, if the option is being constrained
	*	 @param param \p Param - generic Param which holds the option value
	*/
	void setParam(Param param) {
		if (isConstrained()) {
			getConstraint().constrain(param);
		}
		m_param = param;
	} 
	
	/**
	*	@brief If a constraint is enabled, update the current option value based on it
	*/
	void updateConstraint() {
		if (!isConstrained()) return;
		getConstraint().constrain(getParam());
	}
	
	SConstraintBase getConstraint() {
		return m_constraint;
	}
	
	
	/**
	*	@brief If a constraint is set, enable it and update the option value based on it
	*/
	void enableConstraint() {
		if (!hasConstraint()) return;
		getConstraint().enable();
		updateConstraint();
		onConstraintEnable();
	}
	
	/**
	*	@brief If a constraint is set, disable it
	*/
	void disableConstraint() {
		if (!hasConstraint()) return;
		getConstraint().disable();
		onConstraintDisable();
	}
	
	void setConstraint(SConstraintBase constraint) {
		if (m_constraint == constraint) return;
		m_constraint = constraint;
		updateConstraint();
		onConstraintChange();
	}
	
	void removeConstraint() {
		m_constraint = null;
		onConstraintRemove();
	}
	
	bool isConstrained() {
		return hasConstraint() && getConstraint().isEnabled();
	}
	
	bool hasConstraint() {
		return getConstraint() != null;
	}
	
	void setInfo(SUserConfigOptionInfo info) {
		m_info = info;
	}
	
	SUserConfigOptionInfo getInfo() {
		return m_info;
	}
	
	string getWarningMessage() {
		if (!hasConstraint()) return string.Empty;
		return getConstraint().getMessage() + "\n\n" + getConstraint().toString();
	}
	
	protected void onConstraintChange();
	protected void onConstraintEnable();
	protected void onConstraintDisable();
	protected void onConstraintRemove();
}