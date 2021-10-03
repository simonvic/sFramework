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

}