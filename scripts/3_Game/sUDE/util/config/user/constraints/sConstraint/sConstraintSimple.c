class SConstraintSimple<Class T> : SConstraintBase {
	
	protected T value;
	
	void SConstraintSimple(T val) {
		value = val;
	}
	
	T getValue() {
		return value;
	}
	
	override Param performConstrain(Param param) {
		if (!Param1<T>.Cast(param)) return param;
		return new Param1<T>(value);
	}
	
	override bool performValidation(Param param) {
		return Param1<T>.Cast(param) && Param1<T>.Cast(param).param1 == value;
	}
	
	/**
	*	@brief A wrapper for the base constrain method, in order to expose a simpler method signature
	*	 @param toConstrain \p T - Generic value to constrain
	*	 @return T - constrained value
	*/
	T constrain(T toConstrain) {
		return Param1<T>.Cast(constrain(new Param1<T>(toConstrain))).param1;
	}
	
	/**
	*	@brief A wrapper for the base isValid method, in order to expose a simpler method signature
	*	 @param toValidate \p T - Generic value to validate
	*	 @return bool - if the value respects the constraint
	*/
	bool isValid(T toValidate) {
		return isValid(new Param1<T>(toValidate));
	}
	
	override string toString() {
		return "<b>#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_SIMPLE : <i>" + getValue() + "</i></b>";
	}
}