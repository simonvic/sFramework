class SConstraintSimple<Class T> : SConstraintBase {
	
	protected T value;
	
	void SConstraintSimple(T val) {
		value = val;
	}
	
	T getValue() {
		return value;
	}
	
	override string toString() {
		return "#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_SIMPLE : " + getValue();
	}
}