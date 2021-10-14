class SConstraintPrimitiveMinMaxDictionary : SConstraintPrimitiveMinMax<string> {
	
	override void performConstrain(Param param) {
		Param1<string> constrainedParam = Param1<string>.Cast(param);
		if (!constrainedParam) return;
		
		if (constrainedParam.param1 < getMin()) {
			constrainedParam.param1 = getMin();
		} else if (constrainedParam.param1 > getMax()) {
			constrainedParam.param1 = getMax();
		}
		
	}
	
}