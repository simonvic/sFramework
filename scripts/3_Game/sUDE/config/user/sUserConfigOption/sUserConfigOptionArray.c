class SUserConfigOptionArray<Class T> : SUserConfigOptionBase {
	
	void SUserConfigOptionArray(array<T> value, SConstraintBase constraint = null, SUserConfigOptionInfo info = null) {
		m_constraint = constraint;
		super.setParam(new Param1<array<T>>(value));
		m_info = info;
	}
	
	array<T> get() {
		return Param1<array<T>>.Cast(getParam()).param1;
	}
	
	void set(array<T> value) {
		setParam(new Param1<array<T>>(value));
	}

	override void setParam(Param param) {
		array<T> previousValue = new array<T>();
		previousValue.Copy(get());
		array<T> newValue = get();
		// TODO: the only strong ref of the array is in the UserConfig; refactor this
		newValue.Copy(Param1<array<T>>.Cast(param).param1);
		if (isConstrained()) {
			getConstraint().constrain(m_param);
		}
		if (!equal(previousValue, newValue)) {
			onValueChange(previousValue, newValue);
			if (m_onValueChange) {
				m_onValueChange.Invoke(previousValue, newValue);
			}
		}
	}
	
	protected void onValueChange(array<T> previousValue, array<T> newValue);
}
