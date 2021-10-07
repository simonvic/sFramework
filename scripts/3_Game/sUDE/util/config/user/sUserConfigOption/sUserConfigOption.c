class SUserConfigOption<Class T> : SUserConfigOptionBase {
	
	
	void SUserConfigOption(T value, SConstraintBase constraint = null) {
		super.setParam(new Param1<T>(value));
		setConstraint(constraint);
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
		if (get() != previousValue) {
			onValueChange();
		}
	} 
	
	protected void onValueChange() {
		//SLog.d("Value has changed to " + get(),""+this,1);
	}
}