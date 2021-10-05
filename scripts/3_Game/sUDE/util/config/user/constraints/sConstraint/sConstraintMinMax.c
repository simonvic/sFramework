class SConstraintMinMax<Class T> : SConstraintBase {
	
	protected T min;
	protected T max;
	
	void SConstraintMinMax(T minimum, T maximum) {
		min = minimum;
		max = maximum;
	}
	
	T getMin() {
		return min;
	}
	
	T getMax() {
		return max;
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
		return string.Format(
			"<b>#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_MIN : <i>%1</i>\n#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_MAX : <i>%2</i></b>",
			getMin(),
			getMax());
	}
	
}