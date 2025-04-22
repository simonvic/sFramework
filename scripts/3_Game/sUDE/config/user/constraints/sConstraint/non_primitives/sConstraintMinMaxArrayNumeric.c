class SConstraintMinMaxArrayNumeric : SConstraintMinMaxArray<float> {
	
	override void performConstrain(Param param) {
		Param1<array<float>> constrainedParam = Param1<array<float>>.Cast(param);
		if (constrainedParam && constrainedParam.param1) {
			SMath.clamp(constrainedParam.param1, getMin(), getMax());
		}
	}
	
	override bool performValidation(Param param) {
		Param1<array<float>> toValidate = Param1<array<float>>.Cast(param);
		return toValidate && SMath.isInRange(toValidate.param1, getMin(), getMax());
	}
		
}