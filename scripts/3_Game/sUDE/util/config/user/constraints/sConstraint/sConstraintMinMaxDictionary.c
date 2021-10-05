class SConstraintMinMaxDictionary : SConstraintMinMax<string> {
	
	override Param performConstrain(Param param) {
		Param1<string> constrainedParam = Param1<string>.Cast(param);
		if (!constrainedParam) return param;
		
		if (constrainedParam.param1 < getMin()) {
			constrainedParam.param1 = getMin();
		} else if (constrainedParam.param1 > getMax()) {
			constrainedParam.param1 = getMax();
		}		
		
		return constrainedParam;
	}
	
}