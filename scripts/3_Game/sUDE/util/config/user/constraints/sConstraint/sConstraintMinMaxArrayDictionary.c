class SConstraintMinMaxArrayDictionary : SConstraintMinMaxArray<string> {
	
	override void performConstrain(Param param) {
		Param1<array<string>> constrainedParam = Param1<array<string>>.Cast(param);
		if (!constrainedParam || !constrainedParam.param1) return;
				
		int toClampCount = constrainedParam.param1.Count();
		if (min.Count() < toClampCount || max.Count() < toClampCount) return;
		
		for (int i = 0; i < toClampCount; i++) {
			string s = constrainedParam.param1[i];
			string minBound = getMin()[i];
			string maxBound = getMax()[i];
			if (s < minBound) {
				s = minBound;
			} else if (s > maxBound) {
				s = maxBound;
			}
			constrainedParam.param1[i] = s;
		}
		
	}
	
	override bool performValidation(Param param) {
		Param1<array<string>> toValidate = Param1<array<string>>.Cast(param);
		if (!toValidate || !toValidate.param1) return false;
		
		int toValidateCount = toValidate.param1.Count();
		if (min.Count() < toValidateCount || max.Count() < toValidateCount) return false;

		for (int i = 0; i < toValidateCount ; i++) {
			string s = toValidate.param1[i];
			if (s < getMin()[i] || s > getMax()[i]) return false;
		}
		
		return true;
	}
	
}