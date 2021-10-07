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
		setParam(getParam()); //@todo change this?
	}
	
	SConstraintBase getConstraint() {
		return m_constraint;
	}
	
	void enableConstraint() {
		if (!hasConstraint()) return;
		getConstraint().enable();
		updateConstraint();
		onConstraintEnable();
	}
	
	void disableConstraint() {
		if (!hasConstraint()) return;
		getConstraint().disable();
		updateConstraint();
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
		updateConstraint();
		onConstraintRemove();
	}
	
	bool isConstrained() {
		return hasConstraint() && getConstraint().isEnabled();
	}
	
	bool hasConstraint() {
		return getConstraint() != null;
	}
	
	protected void onConstraintChange();
	protected void onConstraintEnable();
	protected void onConstraintDisable();
	protected void onConstraintRemove();
}