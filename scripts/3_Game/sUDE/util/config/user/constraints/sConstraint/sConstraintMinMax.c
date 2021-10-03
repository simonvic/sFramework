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
	
	T constrain(T toConstrain);
	
	bool isValid(T toValidate) {
		//return Math.IsInRange(toValidate, getMin(), getMax()); //yikes...
		return toValidate >= getMin() && toValidate <= getMax();
	}

	override string toString() {
		return string.Format(
			"#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_MIN : %1\n#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_MAX : %2",
			getMin(),
			getMax());
	}
	
}