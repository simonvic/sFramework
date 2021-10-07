class SUserConfigOptionBase : Managed {
	
	protected ref Param m_param;
	protected ref SConstraintBase m_constraint;
	
	Param getParam() {
		return m_param;
	}
	
	void setParam(Param param) {
		if (isConstrained()) {
			getConstraint().constrain(param);
		}
		m_param = param;
	} 
	
	void updateConstraint() {
		if (isConstrained()) {
			getConstraint().constrain(getParam());
		}
	}
	
	SConstraintBase getConstraint() {
		return m_constraint;
	}
	
	void enableConstraint() {
		if (!hasConstraint()) return;
		getConstraint().enable();
		updateConstraint();
	}
	
	void disableConstraint() {
		if (!hasConstraint()) return;
		getConstraint().disable();
		updateConstraint();
	}
	
	void setConstraint(SConstraintBase constraint) {
		m_constraint = constraint;
		updateConstraint();
	}
	
	void removeConstraint() {
		m_constraint = null;
		updateConstraint();
	}
	
	bool isConstrained() {
		return hasConstraint() && getConstraint().isEnabled();
	}
	
	bool hasConstraint() {
		return getConstraint() != null;
	}
	
	void onValueChange();
}