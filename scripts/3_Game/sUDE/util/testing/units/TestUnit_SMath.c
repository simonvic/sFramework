class TestUnit_SMath : STestUnit {
	
	override void init() {
		registerTestCases({
			"testEquals",
			"thisShouldFail",
			"testRanges",
			"testEqualsArrays",
			"testClampArray"
		});
	}
	
	void testEquals() {
		assertEqual(true,  SMath.equal(0.1, 0.1));
		assertEqual(false, SMath.equal(0.5, 0.1));
		assertEqual(true,  SMath.equal(0.00000001, 0.00000001));
		assertEqual(false, SMath.equal(0.000000000000001, 0.000000000000002));
		assertEqual(true,  SMath.equal(0.000000000000001, 0.000000000000002, 0.0001));
	}
	
	void testEqualsArrays() {
		TFloatArray a1;
		TFloatArray a2;
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2, 3};
		assertEqual(true, SMath.equal(a1, a2));
		
		a1 = {0, 0, 0, 0};
		a2 = {0, 1, 2, 3};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2};
		a2 = {0, 1, 2, 3};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2};
		assertEqual(false, SMath.equal(a1, a2));
		
		a1 = {0, 1, 2, 3};
		a2 = {0, 1, 2};
		assertEqual(false, SMath.equal(a1, a2));
	}
	
	void testClampArray() {
		TFloatArray toClamp;
		TFloatArray min;
		TFloatArray max;
		TFloatArray expected;
		
		min      = {0,   1,  10,  100};
		toClamp  = {0.5, 5,  50,  500};
		max      = {1,   10, 100, 1000};
		expected = toClamp;
		SMath.clamp(toClamp, min, max);
		assertEqual(true, SMath.equal(expected, toClamp));
		assertEqual(true, SMath.isInRange(toClamp, min, max));
		
		min      = {0,   1,  10,  100};
		toClamp  = {2,   5,  50,  500};
		max      = {1,   10, 100, 1000};
		expected = {1,   5,  50,  500};
		SMath.clamp(toClamp, min, max);
		assertEqual(true, SMath.equal(expected, toClamp));
		assertEqual(true, SMath.isInRange(toClamp, min, max));
		
		min      = {0,   1,  10,  100};
		toClamp  = {2,   5,  5000,  0.1};
		max      = {1,   10, 100, 1000};
		expected = {1,   5,  100,  100};
		SMath.clamp(toClamp, min, max);
		assertEqual(true, SMath.equal(expected, toClamp));
		assertEqual(true, SMath.isInRange(toClamp, min, max));
	}
	
	void testRanges() {
		assertEqual(true,  SMath.isInRange(0.5, 0.0, 1.0));
		assertEqual(false, SMath.isInRange(-100, 0.0, 1.0));
		assertEqual(false, SMath.isInRange(100, 0.0, 1.0));
	}
	
	void thisShouldFail() {
		assertEqual(0, 1);
	}
}