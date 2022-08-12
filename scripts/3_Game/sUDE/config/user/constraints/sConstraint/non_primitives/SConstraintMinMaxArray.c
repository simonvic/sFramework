class SConstraintMinMaxArray<Class T> : SConstraintNonPrimitiveBase {
	
	protected ref array<T> min;
	protected ref array<T> max;
	
	void SConstraintMinMaxArray(array<T> minimum, array<T> maximum) {
		min = minimum;
		max = maximum;
	}
	
	array<T> getMin() {
		return min;
	}
	
	array<T> getMax() {
		return max;
	}
	
	/**
	*	@brief A wrapper for the base constrain method, in order to expose a simpler method signature
	*	 @param toConstrain \p T - Generic value to constrain
	*	 @return T - constrained value
	*/
	array<T> constrained(array<T> toConstrain) {
		Param1<ref array<T>> p = new Param1<ref array<T>>(toConstrain);
		constrain(p);
		return p.param1;
	}
	
	/**
	*	@brief A wrapper for the base isValid method, in order to expose a simpler method signature
	*	 @param toValidate \p T - Generic value to validate
	*	 @return bool - if the value respects the constraint
	*/
	bool isValid(array<T> toValidate) {
		return isValid(new Param1<ref array<T>>(toValidate));
	}
	

	override string toString() {
		return string.Format(
			"<b>#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_MIN : <i>%1</i>\n#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_MAX : <i>%2</i></b>",
			getMin(),
			getMax());
	}
	
}