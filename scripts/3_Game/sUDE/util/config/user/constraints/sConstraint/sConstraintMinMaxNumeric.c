class SConstraintMinMaxNumeric : SConstraintMinMax<float> {
		
	override Param performConstrain(Param param) {
		Param1<float> constrainedParam = Param1<float>.Cast(param);
		if (!constrainedParam) return param;
		
		constrainedParam.param1 = Math.Clamp(constrainedParam.param1, getMin(), getMax());
		return constrainedParam;
	}
	
	override bool performValidation(Param param) {
		Param1<float> toValidate = Param1<float>.Cast(param);
		return toValidate && (toValidate.param1 >= getMin() && toValidate.param1 <= getMax());
	}

}