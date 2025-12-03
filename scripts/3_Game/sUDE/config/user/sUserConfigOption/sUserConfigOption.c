class SUserConfigOption<Class T> : SUserConfigOptionBase {
	
	void SUserConfigOption(T value, SConstraintBase constraint = null, SUserConfigOptionInfo info = null) {
		m_constraint = constraint;
		super.setParam(new Param1<T>(value));
		m_info = info;
	}
	
	T get() {
		return Param1<T>.Cast(getParam()).param1;
	}
	
	void set(T value) {
		setParam(new Param1<T>(value));
	}
	
	override void setParam(Param param) {
		T previousValue = get();
		super.setParam(param);
		T newValue = get();
		if (newValue != previousValue) {
			onValueChange(previousValue, newValue);
			if (m_onValueChange) {
				m_onValueChange.Invoke(previousValue, newValue);
			}
		}
	} 
	
	protected void onValueChange(T previousValue, T newValue);

}
