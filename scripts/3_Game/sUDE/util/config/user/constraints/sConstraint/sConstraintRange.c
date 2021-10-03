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
	
	override float constrain(float toConstrain) {
		if (!isEnabled()) return toConstrain;
		return Math.Clamp(toConstrain, getMin(), getMax());
	}
		
	override string toString() {
		return super.toString() + "\n#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED_STEP : " + getStep();
	}
}