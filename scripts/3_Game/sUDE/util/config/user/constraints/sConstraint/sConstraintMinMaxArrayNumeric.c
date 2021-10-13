class SConstraintMinMaxArrayNumeric : SConstraintMinMaxArray<float> {
	
	override void performConstrain(Param param) {
		Param1<ref array<float>> constrainedParam = Param1<ref array<float>>.Cast(param);
		if (constrainedParam && constrainedParam.param1) {
			SMath.clamp(constrainedParam.param1, getMin(), getMax());
		}
	}
	
	override bool performValidation(Param param) {
		Param1<ref array<float>> toValidate = Param1<ref array<float>>.Cast(param);
		return toValidate && SMath.isInRange(toValidate.param1, getMin(), getMax());
	}
		
}