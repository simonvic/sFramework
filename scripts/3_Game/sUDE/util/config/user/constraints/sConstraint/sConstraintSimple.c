class SConstraintSimple<Class T> : SConstraintBase {
	
	protected T value;
	
	void SConstraintSimple(T val) {
		value = val;
	}
	
	T getValue() {
		return value;
	}
}

typedef SConstraintSimple<bool> SConstraintSwitch;