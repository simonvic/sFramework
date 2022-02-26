class TestUnit_SConstraints : STestUnit {
	
	override void init() {
		registerTestCases({
			"testConstraints"
		});
	}
	
	private void testConstraints() {
		string stringConstrained = "I've been constrained";
		auto simpleConstrainerString = new SConstraintPrimitiveSimple<string>(stringConstrained);
		simpleConstrainerString.setEnabled(true);
		assertEqual(stringConstrained, simpleConstrainerString.constrained("I need to be constrained"));
		
		bool boolConstrained = false;
		auto simpleConstrainerBool = new SConstraintPrimitiveSimple<bool>(boolConstrained);
		simpleConstrainerBool.setEnabled(true);
		assertEqual(boolConstrained, simpleConstrainerBool.constrained(true));
		
		bool boolConstrained2 = true;
		auto simpleConstrainerBool2 = new SConstraintSwitch(boolConstrained2);
		simpleConstrainerBool2.setEnabled(true);
		assertEqual(boolConstrained2, simpleConstrainerBool2.constrained(false));
		
		int intConstrained = 3;
		auto simpleConstrainerInt = new SConstraintPrimitiveSimple<int>(intConstrained);
		simpleConstrainerInt.setEnabled(true);
		assertEqual(intConstrained, simpleConstrainerInt.constrained(69));
				
		float floatConstrained = 0.420;
		auto simpleConstrainerFloat = new SConstraintPrimitiveSimple<float>(floatConstrained);
		simpleConstrainerFloat.setEnabled(true);
		assertEqual(floatConstrained, simpleConstrainerFloat.constrained(6.9));
		
		
		auto s_minmaxConstrainer = new SConstraintPrimitiveMinMaxDictionary("F","U");
		s_minmaxConstrainer.setEnabled(true);
		assertEqual("F", s_minmaxConstrainer.constrained("A"));
		assertEqual("U", s_minmaxConstrainer.constrained("Z"));
		assertEqual("G", s_minmaxConstrainer.constrained("G"));
		
		auto s_minmaxConstrainer2 = new SConstraintPrimitiveMinMaxDictionary("FFFFFF","UUU");
		s_minmaxConstrainer2.setEnabled(true);
		assertEqual("FFFFFF", s_minmaxConstrainer2.constrained("FF"));
		assertEqual("UUU", s_minmaxConstrainer2.constrained("UUUUU"));
		assertEqual("G", s_minmaxConstrainer2.constrained("G"));
		
		auto f_minmaxConstrainer = new SConstraintPrimitiveMinMaxNumeric(0.0, 1.0);
		f_minmaxConstrainer.setEnabled(true);
		assertEqual(0.0, f_minmaxConstrainer.constrained(-1.0));
		assertEqual(1.0, f_minmaxConstrainer.constrained(2.0));
		assertEqual(0.5, f_minmaxConstrainer.constrained(0.5));
		
		
		TFloatArray min = {};
		TFloatArray max = {};
		TFloatArray expected = {};
		
		min = {0,0,0,0};
		max = {1,1,1,1};
		auto f_minmaxArrayConstrainer = new SConstraintMinMaxArrayNumeric({0,0,0,0}, {1,1,1,1});
		f_minmaxArrayConstrainer.setEnabled(true);
		assertEqual(min, f_minmaxArrayConstrainer.getMin());
		assertEqual(max, f_minmaxArrayConstrainer.getMax());
		
				
		TFloatArray s_toConstrain = {};
		s_toConstrain = {0.5, 0.5, 0.5, 0.5};
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(s_toConstrain, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {-1, -1, -1, -1};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(min, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {10, 10, 10, 10};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(max, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {-1, 10, -1, 10};
		expected      = {0,   1,  0,  1};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(expected, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		
		s_toConstrain = {-1, 10};
		expected      = {0,   1};
		assertEqual(false, f_minmaxArrayConstrainer.isValid(s_toConstrain));
		assertEqual(expected, f_minmaxArrayConstrainer.constrained(s_toConstrain));
		assertEqual(true, f_minmaxArrayConstrainer.isValid(s_toConstrain));
	}
	
}