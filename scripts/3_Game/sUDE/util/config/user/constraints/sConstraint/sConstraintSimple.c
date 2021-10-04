class SConstraintSimple<Class T> : SConstraintBase {
	
	protected T value;
	
	void SConstraintSimple(T val) {
		value = val;
	}
	
	T getValue() {
		return value;
	}
	
	T constrain(T toConstrain) {
		if (!isEnabled()) return toConstrain;
		return value;
	}
	
	bool isValid(T toValidate) {
		return toValidate == value;
	}
	
	override string toString() {
		return "<b>#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_SIMPLE : <i>" + getValue() + "</i></b>";
	}
}