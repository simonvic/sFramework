class SUserConfigOption<Class T> : SUserConfigOptionBase {
	
	
	void SUserConfigOption(T value, SConstraintBase constraint = null) {
		setConstraint(constraint);
		setParam(new Param1<T>(value));
	}
	
	T get() {
		return Param1<T>.Cast(getParam()).param1;
	}
	
	void set(T value) {
		super.setParam(new Param1<T>(value));
	}
	
	override void onValueChange() {
		super.onValueChange();
		//SLog.d("Value has changed to " + get(),""+this,1);
	}
}