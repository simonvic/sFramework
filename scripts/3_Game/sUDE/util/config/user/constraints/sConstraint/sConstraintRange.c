class SConstraintRange : SConstraintMinMax<float> {
	
	protected float step;
	
	void SConstraintRange(float minimum, float maximum, float stepSize) {
		step = stepSize;
	}
	
	float getStep() {
		return step;
	}
	
	void setStep(float stepSize) {
		step = stepSize;
	}
	
}