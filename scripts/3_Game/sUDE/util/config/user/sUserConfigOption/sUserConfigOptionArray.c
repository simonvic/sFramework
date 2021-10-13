class SUserConfigOptionArray<Class T> : SUserConfigOptionBase {
	
	
	void SUserConfigOptionArray(array<T> value, SConstraintBase constraint = null, SUserConfigOptionInfo info = null) {
		m_constraint = constraint;
		super.setParam(new Param1<ref array<T>>(value));
		m_info = info;
	}
	
	array<T> get() {
		return Param1<ref array<T>>.Cast(getParam()).param1;
	}
	
	void set(array<T> value) {
		setParam(new Param1<ref array<T>>(value));
	}
	
	void set(int index, T value) {
		array<T> temp = get();
		temp.Set(index, value);
		this.set(temp);
	}
	
	override void setParam(Param param) {
		array<T> previousValue = get();
		super.setParam(param);
		array<T> newValue = get();
		if (!SMath.equal(previousValue, newValue)) {
			onValueChange(previousValue, newValue);
		}
	} 
	
	protected void onValueChange(array<T> previousValue, array<T> newValue);
}